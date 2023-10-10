use crate::compiler::parser::parse_file;
use crate::compiler::parser::parse::Rule;

#[test]
fn test_array_definition() {
    let file = "fn main() {
        [i8; 64] x = 10;
    }";
    
    let ast = parse_file(file);
    assert!(ast.is_ok());

    // let ast = ast.unwrap();
}
