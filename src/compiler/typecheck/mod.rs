use super::parser::AST;
use super::error::CompilerError;

struct Type {
  name : String,
  size : usize
}

struct TypeTable {}

fn type_check_AST(ast : &AST) -> Result<TypeTable, CompilerError> {
  Ok(TypeTable{})
}