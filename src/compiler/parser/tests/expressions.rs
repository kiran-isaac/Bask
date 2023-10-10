use crate::compiler::parser::parse::parse_file;
use crate::compiler::parser::parse::Rule;

#[test]
fn expression_test_1(){
    let file = "fn main() {
        int x = a + b * c;
    }";
    let ast = parse_file(file);
    assert!(ast.is_ok());

    let ast = ast.unwrap();

    assert_eq!(ast.root.get_child_recursively(Rule::Expression).unwrap().to_string(), "
    Expression{
        Add{
            a,
            Mul{
                b,
                c
            }
        }
    }".replace("\n", "").replace(" ", ""));
}

#[test]
fn expression_test_2(){
    let file = "fn main() {
        int x = a + b * - c;
    }";
    let ast = parse_file(file);
    assert!(ast.is_ok());

    let ast = ast.unwrap();

    assert!(ast.root.get_child_recursively(Rule::Expression).unwrap().to_string() == "
    Expression{
        Add{
            a,
            Mul{
                b,
                Neg{
                    c
                }
            }
        }
    }".replace("\n", "").replace(" ", ""));
}

#[test]
fn expression_test_cast(){
    let file = "fn main() {
        int x = (int) a + b * - c;
    }";
    let ast = parse_file(file);

    let ast = ast.unwrap();

    assert!(ast.root.get_child_recursively(Rule::Expression).unwrap().to_string() == "
    Expression{
        Add{
            Cast{
                int,
                a
            },
            Mul{
                b,
                Neg{
                    c
                }
            }
        }
    }".replace("\n", "").replace(" ", ""));
}