#[derive(Debug, PartialEq, Clone, Eq, Hash)]
pub struct FunctionSignature {
    name: String,
    return_type: Option<String>,
    arguments: Vec<String>,    
}

impl FunctionSignature {
    fn new(name: String, return_type: Option<String>, arguments: Vec<String>) -> FunctionSignature {
        FunctionSignature {
            name,
            return_type,
            arguments,
        }
    }
}

impl ToString for FunctionSignature {
    fn to_string(&self) -> String {
        let mut string = format!("fn {}(", self.name);
        for (i, arg) in self.arguments.iter().enumerate() {
            string.push_str(arg);
            if i != self.arguments.len() - 1 {
                string.push_str(", ");
            }
        }
        string.push_str(")");
        if let Some(return_type) = &self.return_type {
            string.push_str(" -> ");
            string.push_str(return_type);
        }
        string
    }
}