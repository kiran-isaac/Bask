use crate::compiler::{ASTNode, Rule, TypeTable, AST};

use super::Symbol;

#[derive(Debug, Clone)]
pub struct ScopeTree {
    root: Scope,
}

#[derive(Debug, PartialEq, Clone)]
enum ScopeType {
    Global,
    Function,
    Block
}

#[derive(Debug, Clone)]
pub struct Scope {
    pub scope_id: String,
    children: Vec<Scope>,
    symbols: Vec<Symbol>,
    scope_type: ScopeType,
}

impl Scope {
    fn new(ast: &ASTNode, types: &TypeTable, scope_id: String) -> Scope { 
        let mut children = Vec::new();
        let mut symbols = Vec::new();

        let mut i = 0;
        for child in ast.children.iter() {
            match child.rule {
                Rule::CodeBlock => {
                    let mut block_scope = Scope::new(&child, types, format!("{}:{}", scope_id, i));
                    block_scope.scope_type = ScopeType::Block;
                    children.push(block_scope);
                    i += 1;
                },
                Rule::Declaration => {
                    let symbol = Symbol::new(child, types);
                    symbols.push(symbol);
                },
                Rule::Function => {
                    let codeblock = child.get_child(Rule::CodeBlock).unwrap();
                    let block_scope = Scope::new(codeblock, types, format!("{}:{}", scope_id, i));
                    children.push(block_scope);
                    i += 1;
                }
                _ => {}
            }
        }

        return Scope {
            scope_id,
            children,
            symbols,
            scope_type : match ast.rule {
                Rule::File => ScopeType::Global,
                Rule::Function => ScopeType::Function,
                Rule::CodeBlock => ScopeType::Block,
            _ => unreachable!("Cannot convert {:?} to Scope", ast.rule),
        },
        }
    }
}

impl Scope {
    pub fn get_scope(&self, scope_id: String) -> Result<&Scope, ()> {        
        // Collect to a vector and remove the first element
        let scope_id: Vec<&str> = scope_id.split(":").collect();
        let scope_id: Vec<&str> = scope_id[1..].to_vec();
        let scope_id_length: usize = scope_id.len();

        return match scope_id_length {
            0 => {
                Ok(self)
            },
            _ => {
                let next_scope_id = scope_id.join(":");
                return match self.children.get(scope_id[0].parse::<usize>().unwrap()) {
                    Some(scope) => scope.get_scope(next_scope_id),
                    None => Err(()),
                }
            }
        }
    }
}

impl ScopeTree {
    pub fn new(ast: &AST, types: &TypeTable) -> ScopeTree {
        let root = Scope::new(&ast.root, types, "".to_string());
        ScopeTree {
            root,
        }
    }

    pub fn get_scope(&self, scope_id: String) -> &Scope {
        match self.root.get_scope(scope_id.clone()) {
            Ok(scope) => scope,
            Err(_) => panic!("Cannot find scope with id: {}", scope_id),
        }
    }
}

