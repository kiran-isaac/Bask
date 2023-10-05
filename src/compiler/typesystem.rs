pub struct Type {
  name : String,
  size : Option<usize>,
  builtin : bool,
  alias : Option<String>
}

pub struct TypeTable {
  types : HashMap<String, Type>
}

impl TypeTable {
  pub fn new() -> TypeTable {
    let mut types = HashMap::new();
    // Ints
    types.insert("i8".to_string(), Type{name: "i8".to_string(), size: 1, builtin: true, alias: None});
    types.insert("u8".to_string(), Type{name: "u8".to_string(), size: 1, builtin: true, alias: None});
    types.insert("char".to_string(), Type{name: "char".to_string(), size: None, builtin: true, alias: "u8".to_string()});
    types.insert("bool".to_string(), Type{name: "bool".to_string(), size: None, builtin: true, alias: "u8".to_string()});

    types.insert("i16".to_string(), Type{name: "i16".to_string(), size: 2, builtin: true, alias: None});
    types.insert("u16".to_string(), Type{name: "u16".to_string(), size: 2, builtin: true, alias: None});

    types.insert("i32".to_string(), Type{name: "i32".to_string(), size: 4, builtin: true, alias: None});
    types.insert("u32".to_string(), Type{name: "u32".to_string(), size: 4, builtin: true, alias: None});

    types.insert("i64".to_string(), Type{name: "i64".to_string(), size: 8, builtin: true, alias: None});
    types.insert("u64".to_string(), Type{name: "u64".to_string(), size: 8, builtin: true, alias: None});
    
    // Floats
    types.insert("float".to_string(), Type{name: "float".to_string(), size: 4, builtin: true, alias: None});
    types.insert("double".to_string(), Type{name: "double".to_string(), size: 8, builtin: true, alias: None});
    
    // String
    types.insert("string".to_string(), Type{name: "string".to_string(), size: None, builtin: true, alias: "u8".to_string()});

    TypeTable{types}
  }
  

  pub fn get_type(&self, name : &str) -> Option<&Type> {
    self.types.get(name)
  }

  pub fn add_type(&mut self, name : &str, size : usize) {
    self.types.insert(name.to_string(), Type{name: name.to_string(), size, builtin: false});
  }

  pub fn register_enum(&mut self, name : &str, variants : &Vec<String>) {
    let mut size = 0;
    for variant in variants {
      size += 4;
    }
    self.add_type(name, size);
  }
}