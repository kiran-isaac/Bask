mod parser;
mod compile;
mod error;
mod typecheck;

pub use compile::compile;
pub use error::CompilerError;
pub use parser::AST;