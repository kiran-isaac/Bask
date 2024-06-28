## Binary expressions
These are in order of matching precedence.

### Arithmetic: +, -, *, /, %, **
FLOAT + FLOAT = FLOAT
FLOAT + INT = FLOAT
INT + FLOAT = FLOAT
INT + INT = INT

## String concatenation: +
STRING + STRING = STRING

## Comparison: ==, !=, <, <=, >, >=
FLOAT == FLOAT = BOOL
FLOAT == INT = BOOL
INT == FLOAT = BOOL
INT == INT = BOOL
STRING == STRING = BOOL
CHAR == CHAR = BOOL
BOOL == BOOL = BOOL

## Logical: &&, ||
BOOL && BOOL = BOOL
BOOL || BOOL = BOOL

## Bitwise: &, |, ^, <<, >>
BOOL & BOOL = BOOL
BOOL | BOOL = BOOL
BOOL ^ BOOL = BOOL
INT << INT = INT
INT >> INT = INT