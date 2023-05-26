use super::parser::{parse_file, print_parse};

enum Bytecode {
  
}

pub fn compile(file : &str) {
  let pairs = parse_file(&file);
  print_parse(pairs.clone().next().unwrap().into_inner(), 0);
}