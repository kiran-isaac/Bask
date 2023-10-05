use std::process::exit;
use colored::Colorize;

#[allow(dead_code)]
#[derive(Debug)]
pub enum CompilerError {
  ParseError(String),
  TypecheckError(String)
}

impl CompilerError {
  pub fn throw(self) {
    print!("{} ", "Error:".red());
    println!("{}", match self {
      CompilerError::ParseError(s) => format!("Parse error: {}", s),
      CompilerError::TypecheckError(s) => format!("Typecheck error: {}", s),
    });
    exit(1);
  }
}