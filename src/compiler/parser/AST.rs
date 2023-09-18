use pest::iterators::Pair;
use std::fmt::Debug;
use super::parse::Rule;

pub struct AST {
    pub root: ASTNode,
}

#[derive(Clone)]
pub struct ASTNode {
    pub children: Vec<ASTNode>,
    pub rule: Rule,
    pub value: String,
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
        for child in pair.clone().into_inner() {
            if pair.as_rule() == Rule::EOI {
                continue;
            }
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
            if child.rule == Rule::MethodCall {
                let mut cloned_child = child.clone();
                cloned_child.make_expressions_gooder();
            }

            if child.is_primary() || child.rule == Rule::Expression {
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

    pub fn make_expressions_gooder(&mut self) {
        match self.rule {
            Rule::Expression => {
                self.sya();

                let mut stack = ASTNodeStack::new();
                for mut child in self.children.clone() {
                    if child.is_primary() {
                        stack.push(child);
                        continue;
                    }

                    if child.rule == Rule::Expression {
                        child.make_expressions_gooder();
                        assert!(child.children.len() == 1);
                        stack.push(child.children.pop().unwrap());
                        continue;
                    }

                    let operand = stack.pop().unwrap();
                    if child.rule == Rule::Neg || child.rule == Rule::Not {
                        child.children = vec![operand];
                        stack.push(child)
                    } else {
                        let operand2 = stack.pop().unwrap();
                        child.children = vec![operand2, operand];
                        stack.push(child);
                    }
                }

                assert!(stack.stack.len() == 1);
                self.children = stack.stack;
            },
            _ => {
                for child in self.children.iter_mut() {
                    child.make_expressions_gooder();
                }
            }
        }
    }

    #[cfg(test)]
    pub fn dig_for(&self, rule : Rule) -> Option<&ASTNode> {
        fn recursive_dig(node : &ASTNode, rule : Rule) -> Option<&ASTNode> {
            if node.rule == rule {
                return Some(node);
            }

            for child in &node.children {
                let result = recursive_dig(child, rule);
                if result.is_some() {
                    return result;
                }
            }
            None
        }

        recursive_dig(&self, rule)
    }
}

impl AST {
    pub fn new(pair: Pair<Rule>) -> Result<Self, ()> {
        let mut root = AST {
            root: ASTNode::new(pair)?,
        };

        root.root.make_expressions_gooder();
        Ok(root)
    }

    #[cfg(test)]
    pub fn find_function(&self, name : &str) -> Option<&ASTNode> {
        for child in &self.root.children {
            if child.rule == Rule::Function {
                if child.children[0].value == name {
                    return Some(child);
                }
            }
        }
        None
    }
}

impl ToString for ASTNode {
    fn to_string(&self) -> String {
        let mut string = String::new();

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
                string.push_str(format!("{}", self.value).as_str())
            }
            _ => {
                string.push_str(format!("{:?}", self.rule).as_str())
            }
        };

        if self.children.len() > 0 {
            string.push_str("{");
            for child in &self.children {
                string.push_str(child.to_string().as_str());
                string.push_str(",");
            }
            string.pop();
            string.push_str("}");
        }

        string
    }
}

impl Debug for ASTNode {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        fn recursive_print(node : &ASTNode, indent : usize, f: &mut std::fmt::Formatter<'_>) {
            for _ in 0..indent {
                let _ = write!(f, " ");
            }

            let _ = match node.rule {
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
                    write!(f, "{:?} : {}\n", node.rule, node.value)
                }
                _ => {
                    write!(f, "{:?}\n", node.rule)
                }
            };

            for child in &node.children {
                recursive_print(child, indent + 2, f);
            }
        }

        recursive_print(self, 0, f);
        Ok(())
    }
}

impl Debug for AST {
    fn fmt(&self, f: &mut std::fmt::Formatter<'_>) -> std::fmt::Result {
        self.root.fmt(f)
    }
}