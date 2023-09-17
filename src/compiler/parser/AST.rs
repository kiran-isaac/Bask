use pest::{iterators::Pair, RuleType};
use std::fmt::Debug;
use super::parse::Rule;

pub struct AST {
    root: ASTNode,
}

#[derive(Clone)]
struct ASTNode {
    children: Vec<ASTNode>,
    rule: Rule,
    value: String,
}

struct ASTNodeStack {
    stack: Vec<ASTNode>,
}

impl ASTNodeStack {
    pub fn new() -> Self {
        ASTNodeStack {
            stack: Vec::new(),
        }
    }

    pub fn push(&mut self, node: ASTNode) {
        self.stack.push(node);
    }

    pub fn pop(&mut self) -> Option<ASTNode> {
        self.stack.pop()
    }

    pub fn peek(&self) -> Option<&ASTNode> {
        self.stack.last()
    }
}

impl ASTNode {
    pub fn new(pair: Pair<Rule>) -> Result<Self, ()> {
        let mut children: Vec<ASTNode> = Vec::new();
        let rule = pair.as_rule();
        let value = pair.as_str().to_string();
        for child in pair.into_inner() {
            children.push(ASTNode::new(child)?);
        }
        Ok(ASTNode {
            children,
            rule,
            value,
        })
    }

    fn get_precedence(&self) -> Result<i32, ()> {
        match self.rule {
            Rule::Eq | Rule::Neq | Rule::GT | Rule::LT | Rule::GTE | Rule::LTE => Ok(1),
            Rule::Add | Rule::Sub | Rule::Or => Ok(3),
            Rule::Mul | Rule::Div | Rule::Mod | Rule::And => Ok(4),
            Rule::Neg | Rule::Not => Ok(5),
            Rule::Pow => Ok(6),
            _ => Err(()),
        }
    }

    fn is_primary(&self) -> bool {
        self.rule == Rule::varID
            || self.rule == Rule::IntLit
            || self.rule == Rule::FloatLit
            || self.rule == Rule::BoolLit
            || self.rule == Rule::CharLit
            || self.rule == Rule::StringLit
            || self.rule == Rule::MethodCall
    }

    fn is_operator(&self) -> bool {
        self.rule == Rule::Add
            || self.rule == Rule::Sub
            || self.rule == Rule::Mul
            || self.rule == Rule::Div
            || self.rule == Rule::Mod
            || self.rule == Rule::Pow
            || self.rule == Rule::Eq
            || self.rule == Rule::Neq
            || self.rule == Rule::GT
            || self.rule == Rule::LT
            || self.rule == Rule::GTE
            || self.rule == Rule::LTE
            || self.rule == Rule::And
            || self.rule == Rule::Or
            || self.rule == Rule::Neg
            || self.rule == Rule::Not
    }

    // Shunting Yard Algorithm
    // from https://en.wikipedia.org/wiki/Shunting-yard_algorithm
    fn sya(&mut self) {
        let mut op_stack = ASTNodeStack::new();
        let mut output_stack : Vec<ASTNode> = Vec::new();

        for child in &self.children {
            if child.rule == Rule::Expression {
                let mut cloned_child = child.clone();
                cloned_child.sya();
                output_stack.push(cloned_child);
                continue;
            }

            if child.rule == Rule::MethodCall {
                let mut cloned_child = child.clone();
                cloned_child.convert_to_postfix_if_expression();
            }

            if child.is_primary() {
                output_stack.push(child.clone());
            } 
            
            if child.is_operator() {
                let precedence = child.get_precedence();
                if precedence.is_err() {
                    panic!("Unknown operator: {:?}", child.rule);
                }
                let precedence = precedence.unwrap();
                let op = child.clone();

                if op.rule == Rule::Neg || op.rule == Rule::Not {
                    op_stack.push(op);
                    continue;
                }

                while op_stack.peek().is_some() && precedence <= op_stack.peek().unwrap().get_precedence().unwrap() {
                    output_stack.push(op_stack.pop().unwrap());
                }
                op_stack.push(op);
            }
        }

        while op_stack.peek().is_some() {
            assert!(op_stack.peek().unwrap().is_operator());
            output_stack.push(op_stack.pop().unwrap());
        }
        self.children = output_stack;
    }

    pub fn convert_to_postfix_if_expression(&mut self) {
        match self.rule {
            Rule::Expression => {
                self.sya();
            },
            _ => {
                for child in self.children.iter_mut() {
                    child.convert_to_postfix_if_expression();
                }
            }
        }
    }
}

impl AST {
    pub fn new(pair: Pair<Rule>) -> Result<Self, ()> {
        Ok(AST {
            root: ASTNode::new(pair)?,
        })
    }

    pub fn re_jig(&mut self) {
        self.root.convert_to_postfix_if_expression();
    }

    pub fn print(&self) -> std::fmt::Result {
        fn recursive_print(node : &ASTNode, indent : usize) {
            for _ in 0..indent {
                print!(" ");
            }
            println!("{:?}", node);
            for child in &node.children {
                recursive_print(child, indent + 2);
            }
        }

        recursive_print(&self.root, 0);
        Ok(())
    }
}

impl Debug for ASTNode {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        match self.rule {
            Rule::typeID
            | Rule::EnumName
            | Rule::MethodCall
            | Rule::FunctionName
            | Rule::varID
            | Rule::StringLit
            | Rule::CharLit
            | Rule::BoolLit
            | Rule::IntLit
            | Rule::FloatLit => {
                write!(f, "{:?} : {}", self.rule, self.value)
            }
            _ => {
                write!(f, "{:?}", self.rule)
            }
        }
    }
}