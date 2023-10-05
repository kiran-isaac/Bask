use std::process::exit;
use colored::Colorize;

#[allow(dead_code)]
pub enum CompilerError {
  ParseError(String),
  TypecheckError(String),
  Other(String, String)
}

impl CompilerError {
  pub fn throw(self) {
    print!("{} ", "Error:".red());
    println!("{}", match self {
      CompilerError::ParseError(s) => format!("Parse error: {}", s),
      CompilerError::TypecheckError(s) => format!("Typecheck error: {}", s),
      CompilerError::Other(s, t) => format!("{}: {}", s, t)
    });
    exit(1);
  }
}