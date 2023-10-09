use crate::compiler::{ASTNode, Rule};

use super::Symbol;

#[derive(Debug, PartialEq, Clone)]
enum ScopeType {
    Global,
    Function,
    Block
}

#[derive(Debug, Clone)]
pub struct Scope {
    children: Vec<Scope>,
    symbols: Vec<Symbol>,
    scope_type: ScopeType,
}

impl From<ASTNode> for Scope {
    fn from(ast: ASTNode) -> Scope { 
        let mut scope = Scope {
            children: Vec::new(),
            symbols: Vec::new(),
            scope_type: ScopeType::Global,
        };

        for child in ast.children {
            match child.rule {
                Rule::Function => {
                    let mut function_scope = Scope::from(child);
                    function_scope.scope_type = ScopeType::Function;
                    scope.children.push(function_scope);
                },
                Rule::CodeBlock => {
                    let mut block_scope = Scope::from(child);
                    block_scope.scope_type = ScopeType::Block;
                    scope.children.push(block_scope);
                },
                Rule::Declaration => {
                    let symbol = Symbol::from(child);
                    scope.symbols.push(symbol);
                },
                _ => {
                    println!("Unhandled rule: {:?}", child.rule)
                }
            }
        }

        scope
    }
}