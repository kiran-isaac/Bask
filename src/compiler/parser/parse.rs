use pest::Parser;

use crate::compiler::CompilerError;

use super::AST::AST;

#[derive(Parser)]
#[grammar = "compiler/parser/bask.pest"]
pub struct BaskParser;

pub fn parse_file(file: &str) -> Result<AST, CompilerError> {
    let parse_result = BaskParser::parse(Rule::File, file);
    if parse_result.is_err() {
        return Err(CompilerError::ParseError(parse_result.unwrap_err().to_string()));
    }
    
    let ast = AST::new(parse_result.unwrap().into_iter().next().unwrap())?;

    Ok(ast)
}
