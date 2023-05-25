extern crate pest;
#[macro_use]
extern crate pest_derive;

mod parse;

use pest::Parser;

fn main(){
  let test_file = std::fs::read_to_string("src/fnParseTest.bsk").unwrap();
  let mut parser = parse::BaskParser::parse(parse::Rule::File, &test_file).unwrap().next().unwrap();

  let ast = parse::generate_ast(&mut parser);
  ast.print(0);
}
