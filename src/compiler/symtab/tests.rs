use crate::compiler::{parser::parse_file, CompilerError};

use super::scope::Scope;

#[test]
fn scope_tree_init_test() -> Result<(), CompilerError> {
    let file = "fn hello() {
        int a = 1;
        int b = 2;
        int c = 3;
    }";

    let ast = parse_file(file)?;

    println!("{:#?}", ast);

    let scope = Scope::from(ast.root);

    println!("{:#?}", scope);

    Ok(())
}