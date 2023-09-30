extern crate pest;
#[macro_use]
extern crate pest_derive;

mod compiler;
use compiler::compile;

fn help() {
  println!("Usage: bask [command] [options] <file>");
  println!("Commands:");
  println!("  run\t\t\t\tRun the specified file");
  println!("  compile\t\t\t\tBuild the specified file");
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
      let file = match std::fs::read_to_string(&args[2]) {
        Ok(f) => f,
        Err(e) => {
          println!("Error reading file: {}", e);
          return;
        }
      };
      compile(file.as_str());
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
