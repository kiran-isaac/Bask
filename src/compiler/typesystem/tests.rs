use super::*;
use crate::compiler::parser::parse_file;

#[test]
fn struct_test() -> Result<(), CompilerError> {
    let mut type_table = TypeTable::new();

    type_table.add_type(
        "test",
        Type::Struct {
            name: "test".to_string(),
            size: 0,
            fields: vec![(String::from("Int"), String::from("i32"))],
        },
    );

    // Should have size 8
    type_table.add_type(
        "test2",
        Type::Struct {
            name: "test2".to_string(),
            size: 0,
            fields: vec![("Bruh".to_string(), "test".to_string())],
        },
    );
    type_table.finalize()?;

    assert_eq!(type_table.get_size("test".to_string()), 4);
    assert_eq!(type_table.get_size("test2".to_string()), 8);

    Ok(())
}

#[test]
fn from_ast_test() -> Result<(), CompilerError> {
    let file = "enum Piece {
      Pawn,
      Knight,
      Bishop,
      Rook,
      Queen,
      King
  }";
    let ast = parse_file(file);

    println!("AST: {:?}", ast.unwrap().root);

    // let type_table = from_ast(&ast)?;

    Ok(())
}
