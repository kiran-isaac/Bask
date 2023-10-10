use super::{parser::AST, symtab::ScopeTree, TypeTable, CompilerError, Rule};

pub fn generate_LLVM(ast: &AST, type_table: &TypeTable) -> Result<(), CompilerError> {
    let scope_tree = ScopeTree::new(&ast, &type_table);

    for top_level in &ast.root.children {
        match top_level.rule {
            Rule::Enum => {
                println!("enum {:?} ", top_level);
            },
            _ => {}
        }
    }

    Ok(())
}