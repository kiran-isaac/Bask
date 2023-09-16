use super::parse::{parse_file, print_parse};

#[test]
fn enum_parse_tests() {
  let bruh = "enum Bruh (
    ONE,
    TWO,
    THREE
  )";
  let bruh2 = "enum Bruh with int (
    ONE,
    TWO,
    THREE
  )";

  print_parse(parse_file(bruh).into_inner(), 0)
}