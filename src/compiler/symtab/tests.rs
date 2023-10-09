use crate::compiler::{parser::parse_file, CompilerError, symtab::scope::ScopeTree};

use super::scope::Scope;
use crate::compiler::TypeTable;

#[test]
fn get_scope_test() -> Result<(), CompilerError> {
    let file = "
    fn main() {
        int a = 1;
        int b = 2;
        int c = 3;
    }";

    let ast = parse_file(file)?;

    let type_table = TypeTable::from_ast(&ast)?;

    let scope = ScopeTree::new(&ast, &type_table);

    println!("{:#?}", scope.get_scope(":".to_string()).unwrap());

    Ok(())
}

#[test]
fn scope_tree_init_test() -> Result<(), CompilerError> {
    let file = "
    alias Hello = int

    fn hello() {
        Hello a = 1;
        int b = 2;
        int c = 3;
    }";

    let ast = parse_file(file)?;

    let type_table = TypeTable::from_ast(&ast)?;

    println!("{:#?}", ast);

    let scope = ScopeTree::new(&ast, &type_table);

    println!("{:#?}", scope);

    Ok(())
}