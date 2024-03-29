use super::{parser::parse_file, CompilerError, TypeTable};

pub fn compile(file: &str) -> Result<i32, CompilerError> {
    let ast = parse_file(file)?;

    let types = TypeTable::from_ast(&ast)?;

    types.enforce(&ast)?; 

    println!("{:?}", ast);

    Ok(1)
}
