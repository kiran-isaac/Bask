use crate::compiler::parser::parse::Rule;
use crate::compiler::parser::parse_file;

#[test]
fn assignment_test() {
    let file = "fn main() {
        int x;
        int y = 10;
        x = y;
    }";

    let ast = parse_file(file);
    assert!(ast.is_ok());
    let ast = ast.unwrap();

    assert_eq!(ast.root.get_child_recursively(Rule::Declaration).unwrap().to_string(), "Declaration{
        int,
        x
    }".replace("\n", "").replace(" ", ""));

    assert_eq!(ast.root.get_child_recursively(Rule::Assignment).unwrap().to_string(), "Assignment{
        x,
        Expression{
            y
        }
    }".replace("\n", "").replace(" ", ""));
}
