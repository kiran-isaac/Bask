mod parser;
mod compile;
mod error;
mod typesystem;

pub use compile::compile;
pub use error::CompilerError;
pub use parser::AST;
pub use typesystem::TypeTable;