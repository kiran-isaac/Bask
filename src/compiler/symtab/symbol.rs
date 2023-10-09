use crate::compiler::{Rule, ASTNode};

#[derive(Debug, Clone)]
pub struct Symbol {
    name: String,
    type_id: String,
    stmt_id: usize,
}

impl From<ASTNode> for Symbol {
    fn from(ast: ASTNode) -> Symbol {
        match ast.rule {
            Rule::Declaration => {
                let name = ast.children[1].value.clone();
                let type_id = ast.children[0].value.clone();
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