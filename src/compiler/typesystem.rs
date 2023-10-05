use std::collections::HashMap;

use super::CompilerError;

const ENUM_SIZE: usize = 4;

#[derive(Debug, PartialEq, Clone)]
pub enum Type {
    Array {
        of: String,
        elem_size: usize,
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

    pub fn get_size(&self, name : String) -> usize {
        let type_ = self.get_type(&name).unwrap();

        match type_ {
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

    pub fn add_type(&mut self, name: &str, type_: Type) {
        self.types.insert(name.to_string(), type_);
    }

    pub fn get_type(&self, name: &str) -> Option<&Type> {
        return self.types.get(name);
    }

    pub fn finalize(&mut self) -> Result<(), CompilerError> {
        self.check_struct_fields_and_get_size()?;

        
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
