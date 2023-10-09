#[allow(dead_code)]
mod scope;
#[allow(dead_code)]
mod symbol;

#[cfg(test)]
mod tests;

use scope::Scope;
use symbol::Symbol;
use super::typesystem::FunctionSignature;
