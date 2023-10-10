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
    let file = "alias Hello = string;

    struct Vector2 {
      x : float,
      y : float
    }.{
      fn length() -> float {
        return sqrt(x * x + y * y);
      }
    
      fn normalized() -> Vector2 {
        float len = length();
        return new(x / len, y / len);
      }
    
      fn normalize() {
        float len = length();
        x = x / len;
        y = y / len;
      }
    }
    
    Hello.{
      fn say() {
      }
    }";

    let ast = parse_file(file)?;

    let table = TypeTable::from_ast(&ast)?;

    assert_eq!(table.get_size("Vector2".to_string()), 8);

    // let type_table = from_ast(&ast)?;

    Ok(())
}

#[test]
fn array_test() -> Result<(), CompilerError> {
    let file = "fn main() {
        [[int]; 64] x;
    }";

    let ast = parse_file(file)?;

    println!("{:#?}", ast);

    let table = TypeTable::from_ast(&ast)?;

    println!("{:#?}", table);

    Ok(())
}

#[test]
fn chess_typecheck_test() -> Result<(), CompilerError> {
    let file = "enum Piece {
        Pawn,
        Knight,
        Bishop,
        Rook,
        Queen,
        King
    }

    struct Square {
        is_white: bool
    }

    struct Board {
        squares: [Square; 64]
    }";

    let ast = parse_file(file)?;

    let table = TypeTable::from_ast(&ast)?;

    println!("{:#?}", table);
    Ok(())
}
