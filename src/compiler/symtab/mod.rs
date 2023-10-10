#[allow(dead_code)]
mod scope;
#[allow(dead_code)]
mod symbol;

#[cfg(test)]
mod tests;

pub use scope::ScopeTree;
pub use symbol::Symbol;
use super::typesystem::FunctionSignature;