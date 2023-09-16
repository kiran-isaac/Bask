use pest::{
    iterators::{Pair, Pairs},
    pratt_parser::{Assoc, Op, PrattParser},
    Parser,
};

use super::AST::AST;

#[derive(Parser)]
#[grammar = "compiler/parser/bask.pest"]
pub struct BaskParser;

fn handle_error(e: pest::error::Error<Rule>) {
    println!("Error: {}", e);
}

pub fn parse_file(file: &str) -> Pair<Rule> {
    let parse_result = BaskParser::parse(Rule::File, file);
    if parse_result.is_err() {
        handle_error(parse_result.unwrap_err());
        std::process::exit(1);
    }

    let res = parse_result.unwrap().next().unwrap();
    let mut ast = AST::new(res.clone()).unwrap();
    ast.print().unwrap();
    ast.re_jig();
    ast.print().unwrap();
    res
}
