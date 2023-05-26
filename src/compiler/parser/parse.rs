use pest::{Parser, pratt_parser::{PrattParser, Assoc, Op}, iterators::Pairs};

#[derive(Parser)]
#[grammar = "compiler/parser/bask.pest"]
struct BaskParser;

pub fn print_parse(parse : Pairs<Rule>, indent: usize) {
  let indent_str = " ".repeat(indent);
  for pair in parse {
    print!("{}{:?}", indent_str, pair.as_rule());
    let children = pair.clone().into_inner();
    match pair.as_rule() {
      Rule::FunctionName | Rule::ValueLiteral | Rule::varID => {
        print!(" : {}", pair.as_str());
      },
      _ => {}
    }
    println!();
    print_parse(children, indent + 2);
  }
}

pub fn parse_file(file: &str) -> Result<Pairs<Rule>, pest::error::Error<Rule>> {
  return BaskParser::parse(Rule::File, &file);
}