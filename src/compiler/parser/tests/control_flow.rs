use crate::compiler::parser::parse_file;
use crate::compiler::parser::parse::Rule;

#[test]
fn test_switch() {
    let file = "fn main() {
        int x = 0;
        int y = switch x {
            0 => 1,
            1 => 0
        };
    }";
    
    let ast = parse_file(file);
    assert!(ast.is_ok());

    let ast = ast.unwrap();

    assert!(ast.root.get_child_recursively(Rule::Switch).unwrap().to_string() == "Switch{
        Expression{
            x
        },
        Case{
            0,
            Expression{
                1
            }
        },
        Case{
            1,
            Expression{
                0
            }
        }
    }".replace("\n", "").replace(" ", ""));
}
