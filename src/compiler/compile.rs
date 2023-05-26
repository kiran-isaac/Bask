use super::parser::{parse_file, print_parse};

enum Bytecode {
  
}

pub fn compile(file : &str) {
  let pairs = parse_file(&file).unwrap();
  print_parse(pairs, 0);
}