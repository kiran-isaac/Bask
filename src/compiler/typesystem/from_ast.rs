use super::CompilerError;
use super::TypeTable;
use crate::compiler::{AST, ASTNode};

pub fn from_ast(ast : &AST) -> Result<TypeTable, CompilerError> {
    let mut types = TypeTable::new();

    println!("AST: {:?}", ast.root);

    Ok(types)
}