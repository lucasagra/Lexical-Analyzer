#include <stdio.h>
#include <stdlib.h>

const int test_num = 1; // 0 | 1

// ----------------------------------------------------------------------------

typedef enum cat {
    EndOfFile, Dot, Semicolon, If, Then, Else, For, To, Repeat, End,
    Id, Equals, BoolOp, AritOp, Not, True, False, OpenPar, ClosePar,
    CteInt, CteReal
} Category;

typedef struct token {
    Category category;
    char* lexeme;
    int row, col;
    struct token* next;
} Token;

// ----------------------------------------------------------------------------

Token* newToken(Category cat, char* lex, int row, int col);
void err(char* msg, Token* token);
void fPgm();
void fSent();
void fLSent();
void fLSentR();
void fAritExpr();
void fAritExprR();
void fAritTerm();
void fAritFactor();
void nextToken();

// ----------------------------------------------------------------------------

Token* tk;

// ----------------------------------------------------------------------------

int main() {

	if(test_num == 0) {
		tk = newToken(If, "if", 1, 1);
		tk->next = newToken(True, "true", 1, 3);
		tk->next->next = newToken(Then, "then", 1, 7);
		tk->next->next->next = newToken(Id, "a", 1, 11);
		tk->next->next->next->next = newToken(Equals, "=", 1, 12);
		tk->next->next->next->next->next = newToken(CteInt, "100", 1, 13);
		tk->next->next->next->next->next->next = newToken(End, "end", 1, 16);
		tk->next->next->next->next->next->next->next = newToken(Dot, ".", 1, 19);
		tk->next->next->next->next->next->next->next->next = newToken(EndOfFile, "EOF", 1, 20);
		// Simbols  If -> True -> Then -> Id -> Equals -> CteInt -> End -> Point -> EndOfFile
		// tokens 	if true then a = 100 end . EOF
	}
	if(test_num == 1) {
		tk = newToken(Id, "a", 1, 1);
		tk->next = newToken(Equals, "=", 1, 3);
		tk->next->next = newToken(OpenPar, "(", 1, 5);
		tk->next->next->next = newToken(CteInt, "50", 1, 7);
		tk->next->next->next->next = newToken(AritOp, "+", 1, 10);
		tk->next->next->next->next->next = newToken(CteInt, "20", 1, 12);
		tk->next->next->next->next->next->next = newToken(ClosePar, ")", 1, 14);
		tk->next->next->next->next->next->next->next = newToken(Semicolon, ";", 1, 15);
		tk->next->next->next->next->next->next->next->next = newToken(Id, "b", 2, 1);
		tk->next->next->next->next->next->next->next->next->next = newToken(Equals, "=", 2, 3);
		tk->next->next->next->next->next->next->next->next->next->next = newToken(CteInt, "4", 2, 5);
		tk->next->next->next->next->next->next->next->next->next->next->next = newToken(Dot, ".", 2, 7);
		tk->next->next->next->next->next->next->next->next->next->next->next->next = newToken(EndOfFile, "EOF", 3, 1);
		/*
			a = (50 + 20);
			b = 4 .
			EOF
		*/
	}

	printf("Running...\n");
	fPgm();

	while (tk != NULL) nextToken();

	return 0;
}

// ----------------------------------------------------------------------------

Token* newToken(Category cat, char* lex, int row, int col) {
    Token* new_token = malloc(sizeof(Token));
    new_token->category = cat;
    new_token->lexeme = lex;
    new_token->row = row;
    new_token->col = col;
    return new_token;
}

void nextToken() {
    Token* aux = tk;
    tk = tk->next;

	printf("%s ", aux->lexeme);
    free(aux);
}

void err(char* msg, Token* tk) {
	printf("line %d colunm %d: Error: %s\n", tk->row, tk->col, msg);
}

// ----------------------------------------------------------------------------

void fAritFactor() {
	if(tk->category == OpenPar) {
		nextToken();
		fAritExpr();

		if(tk->category == ClosePar) {
			nextToken();
			return;
		} else err("Expected ')'", tk);

	} else if(tk->category == Id) {
		nextToken();
		return;
	} else if(tk->category == CteInt) {
		nextToken();
		return;
	} else if(tk->category == CteReal) {
		nextToken();
		return;
	}
}

void fAritTermR() {
	if(tk->category == AritOp) {
		nextToken();
		fAritTerm();
		fAritTermR();
	}
}

void fAritTerm(){
	fAritFactor();
	fAritTermR();
}

void fAritExprR() {
	if(tk->category == AritOp) {
		nextToken();
		fAritTerm();
		fAritExprR();
	}
}

void fAritExpr() {
	fAritTerm();
	fAritExprR();
}

void fBoolTerm() {
	if(tk->category == Not) {
		nextToken();
		fBoolTerm();
	} else if(tk->category == True) {
		nextToken();
		return;
	} else if(tk->category == False) {
		nextToken();
		return;
	} else {
		fAritExpr();
	}
}

void fBoolExpR() {
	if(tk->category == BoolOp) {
		nextToken();
		fBoolTerm();
		fBoolExpR();
	}
}

void fBoolExp() {
	fBoolTerm();
	fBoolExpR();
}

void fAtr() {
	if(tk->category == Id) {
		nextToken();

		if(tk->category == Equals) {
			nextToken();
			fAritExpr();
		} else err("Expected '='", tk);

	} else err("Expected 'id'", tk);
}

void fLSent() {
	fSent();
	fLSentR();
}

void fLSentR() {
	if(tk->category == Semicolon) {
		nextToken();
		fSent();
		fLSentR();
	}
}

void fSent () {
	if(tk->category == If) {
		nextToken();
		fBoolExp();

		if(tk->category == Then) {
			nextToken();
			fLSent();

			if(tk->category == Else) {
				nextToken();
				fLSent();

				if(tk->category == End) {
					nextToken();
					return;

				} else return err("Expected 'end'", tk);

			} else if (tk->category == End) {
				nextToken();
				return;

			} else return err("Expected 'end'", tk);

		} else return err("Expected 'then'", tk);

	} else if(tk->category == For) {
		nextToken();
		fAtr();

		if(tk->category == To) {
			nextToken();
			fAritExpr();

			if(tk->category == Repeat) {
				nextToken();
				fLSent();

				if(tk->category == End) {
					nextToken();
					return;

				} else return err("Expected 'end'", tk);

			} else return err("Expected 'repeat'", tk);

		} else return err("Expected 'to'", tk);

	} else {
		fAtr();
		return;
	}
}

void fPgm(){

	fLSent();

	if(tk->category == Dot) {
		nextToken();

		if(tk->category == EndOfFile) {
			printf("\nOK!\n");
			return;
		} else return err("Expected 'EOF'", tk);

	} else return err("Expected '.'", tk);
}
