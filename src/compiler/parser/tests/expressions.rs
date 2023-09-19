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
    let expr = ast.root.dig_for(Rule::Expression).unwrap();

    assert!(expr.to_string() == "Expression{Add{a,Mul{b,c}}}");
}

#[test]
fn expression_test_2(){
    let file = "fn main() {
        int x = a + b * - c;
    }";
    let ast = parse_file(file);
    assert!(ast.is_ok());

    let ast = ast.unwrap();
    let expr = ast.root.dig_for(Rule::Expression).unwrap();

    assert!(expr.to_string() == "Expression{Add{a,Mul{b,Neg{c}}}}");
}

#[test]
fn expression_test_cast(){
    let file = "fn main() {
        int x = (int) a + b * - c;
    }";
    let ast = parse_file(file);
    println!("{:?}", ast);

    let ast = ast.unwrap();
    let expr = ast.root.dig_for(Rule::Expression).unwrap();

}