use std::process::exit;
use colored::Colorize;

#[allow(dead_code)]
#[derive(Debug)]
pub enum CompilerError {
  PestError(String),

  BadExpression(usize),

  // Parse errors
  ParseError(String, usize),

  // Typecheck errors
  TypecheckError(String, usize),

  // TypeTable erros
  UnknownType(String),
}

impl CompilerError {
  pub fn throw(self) {
    print!("{} ", "Error:".red());
    println!("{}", match self {
      CompilerError::PestError(s) => format!("Parse error: {}", s),
      CompilerError::BadExpression(l) => format!("Bad expression on line {}", l),
      CompilerError::ParseError(s, l) => format!("Parse error on line {}: {}", l, s),
      CompilerError::TypecheckError(s, l) => format!("Typecheck error on line {}: {}", l, s),
      CompilerError::UnknownType(s) => format!("Unknown type: {}", s),
    });
    exit(1);
  }
}