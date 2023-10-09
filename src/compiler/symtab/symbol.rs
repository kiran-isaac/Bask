use crate::compiler::{Rule, ASTNode, TypeTable};

#[derive(Debug, Clone)]
pub struct Symbol {
    pub name: String,
    type_id: String,
    stmt_id: usize,
}

impl Symbol {
    pub fn new(ast: &ASTNode, types: &TypeTable) -> Symbol {
        match ast.rule {
            Rule::Declaration => {
                let name = ast.children[1].value.clone();
                let type_id = ast.children[0].value.clone();

                // Assert that the type exists
                assert!(types.get_type(type_id.as_str()).is_some());

                // Assert that the statement has an id
                assert!(ast.stmt_id.is_some());

                Symbol {
                    name,
                    type_id,
                    stmt_id: ast.stmt_id.unwrap(),
                }
            },
            _ => unreachable!("Cannot convert {:?} to Symbol", ast.rule),
        }
    }
}