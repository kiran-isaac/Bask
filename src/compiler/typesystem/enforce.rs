use super::CompilerError;
use super::TypeTable;
use crate::compiler::parser::Rule;
use crate::compiler::{AST, ASTNode};

impl TypeTable {
    fn declaration_enforce(&self, ast: &AST) -> Result<(), CompilerError> {
        let root = &ast.root;
        let declarations = root.recursive_dig(Rule::Declaration);
        for declaration in declarations {
            let type_id = declaration.children[0].value.clone();
            match self.get_type(&type_id) {
                Some(_) => {}
                None => {
                    return Err(CompilerError::TypecheckError(
                        format!("Type '{}' not found", type_id)
                    ));
                }
            }
        }
        Ok(())
    }

    // Enforce the type system rules
    pub fn enforce(&self, ast: &AST) -> Result<(), CompilerError> {
        self.declaration_enforce(ast)?;
        Ok(())
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    use crate::compiler::parser::parse_file;

    #[test]
    fn declaration_enforce_non_existant_type() -> Result<(), CompilerError>{
        let file = "fn main() {
            Bruh a = 1;
        }";

        let ast = parse_file(file)?;
        let type_table = TypeTable::from_ast(&ast);
        assert!(type_table.is_err());

        Ok(())
    }
}