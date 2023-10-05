typedef enum PieceType {
	PAWN,
	ROOK,
	KNIGHT,
	BISHOP,
	QUEEN,
	KING
} PieceType;

char PieceType__toChar(PieceType, int);
