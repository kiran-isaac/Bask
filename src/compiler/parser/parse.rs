use pest::{Parser, pratt_parser::{PrattParser, Assoc, Op}, iterators::{Pairs, Pair}};

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

fn get_precedence(pair: Pair<Rule>) -> Result<i32, ()> {
  match pair.as_rule() {
    Rule::Eq | Rule::Neq | Rule::GT | Rule::LT | Rule::GTE | Rule::LTE => Ok(1),
    Rule::Add | Rule::Sub | Rule::Or => Ok(3),
    Rule::Mul | Rule::Div | Rule::Mod | Rule::And => Ok(4),
    Rule::Neg | Rule::Not => Ok(5),
    Rule::Pow => Ok(6),
    _ => Err(())
  }
}

pub fn parse_file(file: &str) -> Pair<Rule> {
  let parse_result = BaskParser::parse(Rule::File, file);
  if parse_result.is_err() {
    handle_error(parse_result.unwrap_err());
    std::process::exit(1);
  }
  let pratt = PrattParser::new()
      .op(Op::Infix(Rule::Eq, Assoc::Left) | Op::Infix(Rule::Neq, Assoc::Left) | Op::Infix(Rule::GT, Assoc::Left) | Op::Infix(Rule::LT, Assoc::Left) | Op::Infix(Rule::GTE, Assoc::Left) | Op::Infix(Rule::LTE, Assoc::Left))
      .op(Op::Infix(Rule::Add, Assoc::Left) | Op::Infix(Rule::Sub, Assoc::Left) | Op::Infix(Rule::Or, Assoc::Left))
      .op(Op::Infix(Rule::Mul, Assoc::Left) | Op::Infix(Rule::Div, Assoc::Left) | Op::Infix(Rule::Mod, Assoc::Left) | Op::Infix(Rule::And, Assoc::Left))
      .op(Op::Prefix(Rule::Neg) | Op::Prefix(Rule::Not))
      .op(Op::Infix(Rule::Pow, Assoc::Left));

  pratt.map_primary(

  )
}