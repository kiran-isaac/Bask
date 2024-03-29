use crate::compiler::{parser::parse_file, CompilerError, symtab::scope::ScopeTree};

use crate::compiler::TypeTable;

#[test]
fn get_scope_test() -> Result<(), CompilerError> {
    let file = "
    fn main() {
        int d = 17;
       { int a = d;}
        int b = 2;
        int c = 3;
    }";

    let ast = parse_file(file)?;
    let type_table = TypeTable::from_ast(&ast)?;
    let derived_scope_tree = ScopeTree::new(&ast, &type_table);
    assert_eq!(derived_scope_tree.get_scope(":0".to_string()).scope_id, ":0".to_string());
    assert_eq!(derived_scope_tree.get_scope(":0:0".to_string()).scope_id, ":0:0".to_string());

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

    let scope = ScopeTree::new(&ast, &type_table);

    Ok(())
}