mod parse;

#[allow(non_snake_case)]
mod AST;

#[cfg(test)]
mod tests;

pub use parse::parse_file;
