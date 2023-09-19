use super::parser::parse_file;

enum Bytecode {
  
}

pub fn compile(file : &str) {
  let ast = parse_file(&file);
  // Apply rules to AST

  match ast {
    Ok(ast) => {
      println!("AST: {:?}", ast);
    },
    Err(_) => {
      println!("Error parsing file");
    }
  }
}