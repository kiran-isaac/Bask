use pest::Parser;

use super::AST::AST;

#[derive(Parser)]
#[grammar = "compiler/parser/bask.pest"]
pub struct BaskParser;

pub fn parse_file(file: &str) -> Result<AST, String> {
    let parse_result = BaskParser::parse(Rule::File, file);
    if parse_result.is_err() {
        println!("{:?}", parse_result);
        return Err("Error parsing file".to_string());
    }
    
    let ast = AST::new(parse_result.unwrap().into_iter().next().unwrap()).unwrap();
    
    Ok(ast)
}
