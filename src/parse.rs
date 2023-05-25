#[derive(Parser)]
#[grammar = "bask.pest"]
pub struct BaskParser;

#[derive(Debug)]
pub enum DebugASTNodeType {
  TypeID,
  VarID,

  File,
  Function,
  FunctionName,
  FunctionArgs,
  FunctionArg,
  
  CodeBlock,
  Statement,
  Assignement,

  Enum,
  EnumName,
  EnumTypeList,
  EnumValueList,
  EnumValue,

  Expression,
  Num,
  Integer,
  Float,
} 

pub struct DebugAST {
  str: String,
  node_type: DebugASTNodeType,
  children: Vec<DebugAST>,
}

impl DebugAST {
  pub fn print(&self, indent_level : i32)  {
    let mut indent = String::new();
    for _ in 0..indent_level {
      indent.push_str("  ");
    }
    println!("{}{}", indent, self.str);
    for child in &self.children {
      child.print(indent_level + 1);
    }
  }
}

pub fn generate_ast(node: &mut pest::iterators::Pair<Rule>) -> DebugAST {
  let mut children = Vec::new();
  for mut child in node.clone().into_inner() {
    if child.as_rule() == Rule::EOI {
      continue;
    }
    children.push(generate_ast(&mut child));
  }

  match node.as_rule() {
    Rule::typeID => DebugAST { str: node.as_str().to_string(), node_type: DebugASTNodeType::TypeID, children },
    Rule::varID => DebugAST { str: node.as_str().to_string(), node_type: DebugASTNodeType::VarID, children },

    Rule::File => DebugAST { str: "File".to_string(), node_type: DebugASTNodeType::File, children },
    Rule::Function => DebugAST { str: "Function".to_string(), node_type: DebugASTNodeType::Function, children },
    Rule::FunctionName => DebugAST { str: "FunctionName".to_string(), node_type: DebugASTNodeType::FunctionName, children },
    Rule::FunctionArgList => DebugAST { str: "FunctionArgs".to_string(), node_type: DebugASTNodeType::FunctionArgs, children },
    Rule::FunctionArg => DebugAST { str: "Arg".to_string(), node_type: DebugASTNodeType::FunctionArg, children },


    Rule::CodeBlock => DebugAST { str: "CodeBlock".to_string(), node_type: DebugASTNodeType::CodeBlock, children },
    Rule::Statement => DebugAST { str: "Statement".to_string(), node_type: DebugASTNodeType::Statement, children },
    Rule::Assignment => DebugAST { str: format!("Assignement: {}", node.as_str()), node_type: DebugASTNodeType::Assignement, children },

    Rule::Enum => DebugAST { str: "Enum".to_string(), node_type: DebugASTNodeType::Enum, children },
    Rule::EnumTypeList => DebugAST { str: "EnumTypeList".to_string(), node_type: DebugASTNodeType::EnumTypeList, children },
    Rule::EnumValueList => DebugAST { str: "EnumValueList".to_string(), node_type: DebugASTNodeType::EnumValueList, children },
    Rule::EnumValue => DebugAST { str: "EnumValue".to_string(), node_type: DebugASTNodeType::EnumValue, children },
    Rule::EnumValueID => DebugAST { str: "EnumValueID".to_string(), node_type: DebugASTNodeType::EnumValue, children },
    Rule::FuncBlock => DebugAST { str: "FuncBlock".to_string(), node_type: DebugASTNodeType::CodeBlock, children },

    Rule::Expression => DebugAST { str: "Expression".to_string(), node_type: DebugASTNodeType::Expression, children },
    Rule::Num => DebugAST { str: "Num".to_string(), node_type: DebugASTNodeType::Num, children },
    Rule::Integer => DebugAST { str: "Integer".to_string(), node_type: DebugASTNodeType::Integer, children },
    Rule::Float => DebugAST { str: "Float".to_string(), node_type: DebugASTNodeType::Float, children },
    _ => DebugAST { str: format!("UNKNOWN: {:?}{}", node.as_rule(), node.as_str()), node_type: DebugASTNodeType::Expression, children },
  }
}