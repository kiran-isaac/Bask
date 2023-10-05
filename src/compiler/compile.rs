use super::parser::parse_file;

pub fn compile(file : &str) -> Result<i32, ()> {
  match parse_file(file) {
    Ok(ast) => {
      println!("{:?}", ast);
    },
    Err(e) => {
      e.throw();
    }
  }
  Ok(1)
}