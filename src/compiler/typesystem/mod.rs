use std::collections::HashMap;

use super::CompilerError;

mod from_ast;
mod functions;

pub use functions::FunctionSignature;

mod enforce;
#[cfg(test)]
mod tests;

const ENUM_SIZE: usize = 4;
const PTR_SIZE: usize = 8;

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
    Alias {
        name: String,
        target: String,
    },
}

#[derive(Debug, Clone)]
pub struct TypeTable {
    types: HashMap<String, Type>,
}

impl TypeTable {
    fn new() -> TypeTable {
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
                name: "char".to_string(),
                target: "u8".to_string(),
            },
        );
        types.insert(
            "bool".to_string(),
            Type::Alias {
                name: "bool".to_string(),
                target: "u8".to_string(),
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
            "int".to_string(),
            Type::Alias {
                name: "int".to_string(),
                target: "i32".to_string(),
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

    pub fn get_size(&self, name: String) -> usize {
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

    fn add_type(&mut self, name: &str, type_: Type) {
        self.types.insert(name.to_string(), type_);
    }

    pub fn get_type(&self, name: &str) -> Option<&Type> {
        return self.types.get(name);
    }

    fn finalize(&mut self) -> Result<(), CompilerError> {
        let mut table_2 = self.clone();

        for (name, type_) in self.types.iter_mut() {
            match type_ {
                Type::Struct { fields, .. } => {
                    let mut struct_size = 0;
                    for (_, field_type_name) in fields.iter_mut() {
                        let field_type = table_2.get_type(field_type_name);
                        if field_type.is_none() {
                            return Err(CompilerError::UnknownType(format!(
                                "{} in struct {}",
                                field_type_name, name
                            )));
                        }
                        match field_type.unwrap() {
                            Type::Fundamental { size, .. } => {
                                struct_size += *size;
                            }
                            Type::Enum { .. } => {
                                struct_size += ENUM_SIZE;
                            }
                            _ => {
                                struct_size += PTR_SIZE;
                            }
                        }
                    }
                    table_2.types.insert(
                        name.to_string(),
                        Type::Struct {
                            name: (*name).clone(),
                            size: struct_size,
                            fields: fields.clone(),
                        },
                    );
                }
                _ => {}
            }
        }

        self.types = table_2.types.clone();

        Ok(())
    }
}
