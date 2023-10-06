use super::CompilerError;
use super::{TypeTable, Type};
use crate::compiler::AST;
use crate::compiler::parser::Rule;

impl TypeTable {
    pub fn from_ast(ast: &AST) -> Result<TypeTable, CompilerError> {
        let mut types = TypeTable::new();

        let root = &ast.root;

        // Get all the top level type definitions and add them to the type table
        for top_level_node in root.children.iter() {
            match top_level_node.rule {
                Rule::Enum => {
                    let name = top_level_node.children[0].value.clone();
                    let mut variants = Vec::new();
                    for variant in &top_level_node.children {
                        variants.push(variant.value.clone());
                    }
                    types.types.insert(
                        name.clone(),
                        Type::Enum {
                            name: name,
                            variants,
                        },
                    );
                }
                Rule::Alias => {
                    let name = top_level_node.children[0].value.clone();
                    let target = top_level_node.children[1].value.clone();
                    types.types.insert(
                        name.clone(),
                        Type::Alias {
                            name: name,
                            target: target,
                        },
                    );
                }
                Rule::Struct => {
                    let name = top_level_node.children[0].value.clone();
                    let mut fields = Vec::new();
                    for field in &top_level_node.children[1].children {
                        match field.rule {
                            Rule::StructField => {
                                let field_name = field.children[0].value.clone();
                                let field_type = field.children[1].value.clone();
                                fields.push((field_name, field_type));
                            }
                            _ => {
                                unreachable!();
                            }
                        }
                    }
                    types.types.insert(
                        name.clone(),
                        Type::Struct {
                            name: name,
                            size: 0,
                            fields: fields,
                        },
                    );
                }

                // not invalid but has no bearing on le type table construction
                Rule::ImplForType | Rule::Function | Rule :: EOI => {}

                // Shouldnt be anything else???
                _ => {
                    unreachable!("Invalid top level node in type table construction: {:?}", top_level_node.rule);
                }
            }
        }

        types.finalize()?;

        Ok(types)
    }
}
