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

fn shunting_yard(mut expr : Pair<Rule>) -> Pair<Rule> {
  let mut output: Vec<Pair<Rule>> = Vec::new();
  let mut operator: Vec<Pair<Rule>> = Vec::new();
  for mut pair in expr.clone().into_inner() {
    match pair.as_rule() {
      Rule::Expression => {
        shunting_yard(pair);
      },
      Rule::Primary => {
        output.push(pair);
      },
      Rule::Add | Rule::Sub | Rule::Mul | Rule::Div | Rule::Mod | Rule::Pow | Rule::Or  | Rule::And | Rule::Eq  | Rule::Neq | Rule::GT  | Rule::LT  | Rule::GTE | Rule::LTE | Rule::Neg | Rule::Not => {
        loop {
          let top_of_op_stack = operator.last();
          if top_of_op_stack.is_none() {break;}
          let top_of_op_stack = top_of_op_stack.unwrap();
          let top_of_op_stack_precedence = get_precedence(top_of_op_stack.clone());
          let current_precedence = get_precedence(pair.clone());
          if top_of_op_stack_precedence.is_err() || current_precedence.is_err() {
            unreachable!("Cannot get precedence of operator in SYA");
          }
          if top_of_op_stack_precedence.unwrap() <= current_precedence.unwrap() {
            break;
          }
        }
        operator.push(pair);
      },
      _ => {
        unreachable!("Unrecognised operator in SYA");
      }
    }
  }
  while !operator.is_empty() {
    output.push(operator.pop().unwrap());
  }
  for pair in output {
    println!("{:?}", pair.as_rule());
  }
  expr
}

fn to_postfix(mut parse_result : Pair<Rule>) -> Pair<Rule> {
  for mut pair in parse_result.clone().into_inner() {
    match pair.as_rule() {
      Rule::Expression => {
        shunting_yard(pair);
      },
      _ => {}
    }
  }
  parse_result
}

pub fn parse_file(file: &str) -> Pair<Rule> {
  let parse_result = BaskParser::parse(Rule::File, file);
  if parse_result.is_err() {
    handle_error(parse_result.unwrap_err());
    std::process::exit(1);
  }
  to_postfix(parse_result.unwrap().next().unwrap())
}