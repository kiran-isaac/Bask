extern crate pest;
#[macro_use]
extern crate pest_derive;

mod parse;

use pest::Parser;
use std::collections::HashMap;
use pest::iterators::Pair;

fn main() -> Result<(), pest::error::Error<parse::Rule>> {
  let test_file = std::fs::read_to_string("src/fnParseTest.bsk").unwrap();
  let parser = parse::BaskParser::parse(parse::Rule::File, &test_file)?.next().unwrap();

  let functions = parser.into_inner().find(|p| p.as_rule() == parse::Rule::Enum).unwrap()
  .into_inner().find(|p| p.as_rule() == parse::Rule::FuncBlock).unwrap()
  .into_inner();

  for function in functions {
    print!("{:?}\n", function.into_inner().find(|p| p.as_rule() == parse::Rule::FuncName).unwrap().as_str());
  }

  Ok(())
}
