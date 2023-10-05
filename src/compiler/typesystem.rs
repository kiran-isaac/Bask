use std::{collections::HashMap, sync::Arc};

use super::CompilerError;

const ENUM_SIZE: usize = 4;

#[derive(Debug, PartialEq, Clone)]
pub enum Type {
    Array {
        of: String,
        elem_size: usize,
    },
    Alias {
        to: String,
        size: usize,
    },
    Fundamental {
        name: String,
        size: usize,
    },
    Struct {
        name: String,
        size: usize,
        fields: Vec<(String, String)>,
    },
    Enum {
        name: String,
        variants: Vec<String>,
    },
}

impl Type {
    pub fn get_size(&self) -> usize {
        match self {
            Type::Array { elem_size, .. } => *elem_size,
            Type::Alias { size, .. } => *size,
            Type::Fundamental { size, .. } => *size,
            Type::Struct { size, .. } => *size,
            Type::Enum { .. } => ENUM_SIZE,
        }
    }
}

#[derive(Debug, Clone)]
pub struct TypeTable {
    types: HashMap<String, Type>,
}

impl TypeTable {
    pub fn new() -> TypeTable {
        let mut types = HashMap::new();

        // ints
        types.insert(
            "i8".to_string(),
            Type::Fundamental {
                name: "i8".to_string(),
                size: 1,
            },
        );
        types.insert(
            "u8".to_string(),
            Type::Fundamental {
                name: "u8".to_string(),
                size: 1,
            },
        );
        types.insert(
            "char".to_string(),
            Type::Alias {
                to: "u8".to_string(),
                size: 1,
            },
        );
        types.insert(
            "bool".to_string(),
            Type::Alias {
                to: "u8".to_string(),
                size: 1,
            },
        );

        types.insert(
            "i16".to_string(),
            Type::Fundamental {
                name: "i16".to_string(),
                size: 2,
            },
        );
        types.insert(
            "u16".to_string(),
            Type::Fundamental {
                name: "u16".to_string(),
                size: 2,
            },
        );

        types.insert(
            "i32".to_string(),
            Type::Fundamental {
                name: "i32".to_string(),
                size: 4,
            },
        );
        types.insert(
            "u32".to_string(),
            Type::Fundamental {
                name: "u32".to_string(),
                size: 4,
            },
        );

        types.insert(
            "i64".to_string(),
            Type::Fundamental {
                name: "i64".to_string(),
                size: 8,
            },
        );
        types.insert(
            "u64".to_string(),
            Type::Fundamental {
                name: "u64".to_string(),
                size: 8,
            },
        );

        // floats
        types.insert(
            "float".to_string(),
            Type::Fundamental {
                name: "float".to_string(),
                size: 4,
            },
        );
        types.insert(
            "double".to_string(),
            Type::Fundamental {
                name: "double".to_string(),
                size: 8,
            },
        );

        // string
        types.insert(
            "string".to_string(),
            Type::Array {
                of: "char".to_string(),
                elem_size: 1,
            },
        );

        TypeTable { types }
    }

    pub fn add_type(&mut self, name: &str, type_: Type) {
        self.types.insert(name.to_string(), type_);
    }

    pub fn get_type(&self, name: &str) -> Option<&Type> {
        return self.types.get(name);
    }

    fn check_alias_dest_and_get_size(&mut self) {
        let mut unregistered: Vec<String>;
        let last_unregistered_length = 0;

        loop {
            unregistered = self
                .types
                .iter()
                .filter(|(_, type_)| {
                    match type_ {
                        Type::Alias { to, .. } => {
                            let to_type = self.get_type(to);
                            if to_type.is_none() {
                                return true;
                            }
                        }
                        _ => {}
                    }
                    return false;
                })
                .map(|(name, _)| name.to_string())
                .collect();

            if unregistered.len() == 0 {
                break;
            }

            if unregistered.len() == last_unregistered_length {
                panic!("Unregistered types: {:?}", unregistered);
            }
        }

        fn recursively_get_size(type_: &Type, type_table: &TypeTable) -> usize {
            match type_ {
                Type::Alias { to, .. } => {
                    let to_type = type_table.get_type(to).unwrap();
                    return recursively_get_size(to_type, type_table);
                }
                Type::Fundamental { size, .. } => {
                    return *size;
                }
                Type::Struct { size, .. } => {
                    return *size;
                }
                _ => {
                    unreachable!();
                }
            }
        }

        let unsized_aliases = self.clone()
                    .types
                    .iter()
                    .filter(|(_, type_)| {
                        match type_ {
                            Type::Alias { size , ..} => {
                                return size == &0;
                            }
                            _ => {}
                        }
                        return false;
                    })
                    .map(|(name, _)| name.to_string())
                    .collect::<Vec<String>>();

        for name in unsized_aliases.iter() {
            let type_ = self.get_type(name).unwrap();

            let size = recursively_get_size(type_, self);
            self.types.insert(name.to_string(), Type::Alias {
                to: match type_ {
                    Type::Alias { to, .. } => to.to_string(),
                    _ => unreachable!()
                },
                size
            });
        }
    }

    pub fn finalize(&mut self) -> Result<(), CompilerError> {
        self.check_alias_dest_and_get_size();

        let mut unregistered: Vec<String>;
        let last_unregistered_length = 0;

        loop {
            unregistered = self
                .types
                .iter()
                .filter(|(_, type_)| {
                    match type_ {
                        Type::Alias { to, .. } => {
                            let to_type = self.get_type(to);
                            if to_type.is_none() {
                                return true;
                            }
                        }
                        Type::Struct { fields, .. } => {
                            for field in fields {
                                let field_type = self.get_type(&field.1);
                                if field_type.is_none() {
                                    return true;
                                }
                            }
                        }
                        _ => {}
                    }
                    return false;
                })
                .map(|(name, _)| name.to_string())
                .collect();

            if unregistered.len() == 0 {
                break;
            }

            if unregistered.len() == last_unregistered_length {
                return Err(CompilerError::TypecheckError(format!(
                    "Unregistered types: {:?}",
                    unregistered
                )));
            }
        }

        Ok(())
    }
    /*
    // pub fn register_enum(&mut self, name : &str, variants : &Vec<String>) {
    //   let mut size = 0;
    //   for variant in variants {
    //     size += 4;
    //   }
    //   self.types.insert(name.to_string(), Type::Alias(name.to_string()));
    // }

    // fn register_unresolved(&mut self, name : &str) {
    //   self.unresolved.push(name.to_string());
    // }

    // fn follow_alias(&self, name : &str) -> Option<&Type> {
    //   let search_result = self.types.get(name);
    //   if search_result.is_some() {
    //     return search_result;
    //   }
    //   return None;
    // }

    // pub fn register_struct(&mut self, name : &str, fields : Vec<(String, String)>) {
    //   let mut size = 0;
    //   for field in fields {
    //     let field_type = self.get_type(&field.1);
    //     if field_type.is_none() {
    //       self.register_unresolved(&field.1);
    //       continue;
    //     }

    //   }
    //   self.types.insert(name.to_string(), Type::Struct { name : name.to_string(), size, fields });
    // }

    // pub fn finalize(self) -> Result<(), CompilerError> {
    //   for unresolved in self.unresolved {
    //     let unresolved_type = self.get_type(&unresolved).unwrap();
    //   }
    //   Ok(())
    // } */
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn alias_test() {
        let mut type_table = TypeTable::new();
        type_table.add_type(
            "test",
            Type::Alias {
                to: "i32".to_string(),
                size: 4,
            },
        );
        assert!(
            type_table.get_type("test").unwrap()
                == &Type::Alias {
                    to: "i32".to_string(),
                    size: 4,
                }
        );

        type_table.add_type(
            "test2",
            Type::Alias {
                to: "test".to_string(),
                size: 0,
            },
        );

        type_table.finalize();
        assert!(
            type_table.get_type("test2").unwrap().get_size() == 4
        );
    }

    #[test]
    fn struct_test() {
        let mut type_table = TypeTable::new();

        type_table.add_type(
            "test",
            Type::Struct {
                name: "test".to_string(),
                size: 0,
                fields: vec![(String::from("Int"), String::from("i32"))],
            },
        );
        type_table.add_type(
            "test2", 
            Type::Struct {
                name: "test2".to_string(),
                size: 0,
                fields: vec![("Bruh".to_string(), "test".to_string())]
            }
        );
        type_table.finalize().unwrap();

        println!("{:?}", type_table);
    }
}
