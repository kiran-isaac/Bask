use super::CompilerError;
use super::TypeTable;
use crate::compiler::ASTNode;
use crate::compiler::parser::Rule;
use crate::compiler::AST;

impl TypeTable {
    // Validate a type from an ASTNode
    pub fn validate_from_ast(&self, ast: &ASTNode) -> Result<(), ()> {
        assert_eq!(ast.rule, Rule::typeID);
        let mut ast = ast;
        if ast.children.len() == 1 {
            ast = &ast.children[0];
        }
        if ast.rule == Rule::array {
            self.validate_from_ast(&ast.children[0])
        } else {
            match self.get_type(&ast.value) {
                    Some(_) => Ok(()),
                    None => Err(()),
            }
        }
    }

    fn declaration_enforce(&self, ast: &AST) -> Result<(), CompilerError> {
        let root = &ast.root;
        let declarations = root.recursive_dig(Rule::Declaration);
        for declaration in declarations {
            let type_ = &declaration.children[0];
            match self.validate_from_ast(type_) {
                Ok(_) => {}
                Err(_) => {
                    return Err(CompilerError::TypecheckError(
                        format!("Invalid type {}", type_.value)
                        , declaration.line
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
        let file = "fn main() -> int {
            Bruh a = 1;
        }";

        let ast = parse_file(file)?;
        let type_table = TypeTable::from_ast(&ast);

        assert!(type_table.is_err());

        Ok(())
    }
}