extern crate pest;
#[macro_use]
extern crate pest_derive;

mod compiler;
use std::io::Read;

use compiler::compile;

fn help() {
  println!("Usage: bask [command] [options] <file>");
  println!("Commands:");
  println!("  run\t\t\t\tRun the specified files");
  println!("  compile\t\t\t\tBuild the specified files");
  println!("Options:");
  println!("  -h, --help\t\t\tPrint this help message");
  println!("  -v, --version\t\t\tPrint the version of the compiler");
  println!("  -o, --output <file>\t\tSpecify the output file");
}

fn main() {
  let args = std::env::args().collect::<Vec<String>>();
  let version = env!("CARGO_PKG_VERSION");
  if args.len() < 2 {
    help();
    return;
  }

  match args[1].as_str() {
    "run" => {
      println!("Running file")
    },
    "compile" => {
      if args.len() < 3 {
        println!("No file specified");
        return;
      }
      // Read all files and combine them into one string
      let mut file = String::new();
      for i in 2..args.len() {
        let mut f = std::fs::File::open(&args[i]).unwrap();
        let mut contents = String::new();
        f.read_to_string(&mut contents).unwrap();
        file.push_str(contents.as_str());
        file.push('\n');
      }

      match compile(file.as_str()) {
        Ok(_) => println!("Compiled successfully"),
        Err(e) => e.throw()
      }
    },
    "-h" | "--help" => {
      help();
      return
    },
    "-v" | "--version" => {
      println!("{}", version);
    },
    _ => {
      println!("Unknown command");
      help();
      return
    }
  }
}
