use pest::Parser;

use crate::compiler::CompilerError;

use super::ast::AST;

#[derive(Parser)]
#[grammar = "compiler/parser/bask.pest"]
pub struct BaskParser;

pub fn parse_file(file: &str) -> Result<AST, CompilerError> {
    let parse_result = BaskParser::parse(Rule::File, file);
    if parse_result.is_err() {
        return Err(CompilerError::PestError(parse_result.unwrap_err().to_string()));
    }
    let parse_result = parse_result.unwrap();
    let root = parse_result.clone().next().unwrap();

    let ast = AST::new(root)?;

    Ok(ast)
}
