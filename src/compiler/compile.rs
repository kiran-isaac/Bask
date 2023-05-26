use super::parser::parse_file;

enum Bytecode {
  
}

pub fn compile(file : &str) {
  let ast = parse_file(&file).unwrap();
  ast.print(0);
}