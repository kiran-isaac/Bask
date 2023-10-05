use super::{parser::parse_file, CompilerError};

pub fn compile(file : &str) -> Result<i32, CompilerError> {
  let ast = parse_file(file)?;
  Ok(1)
}