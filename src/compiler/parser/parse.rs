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
      Rule::typeID | Rule::EnumName | Rule::MethodCall | Rule::FunctionName | Rule::varID | Rule::StringLit | Rule::CharLit | Rule::BoolLit | Rule::IntLit | Rule::FloatLit => {
        print!(" : {}", pair.as_str());
      },
      _ => {}
    }
    println!();
    print_parse(children, indent + 2);
  }
}

fn handle_error(e: pest::error::Error<Rule>){
  println!("Error: {}", e);
}

pub fn parse_file(file: &str) -> Pairs<Rule> {
  let parse_result = BaskParser::parse(Rule::File, file);
  if parse_result.is_err() {
    handle_error(parse_result.unwrap_err());
    std::process::exit(1);
  }
  parse_result.unwrap()
}