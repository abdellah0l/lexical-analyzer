#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_RELATIONAL_OP,
    TOKEN_HYPHEN,
    TOKEN_COMMA,
    TOKEN_LBRACKET,
    TOKEN_RBRACKET,
    TOKEN_EOF,
    TOKEN_ERROR
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256];
} Token;

const char *input;
int pos = 0;

Token get_next_token() {
    Token token;
    token.lexeme[0] = '\0';

    while (input[pos] != '\0' && isspace(input[pos])) {
        pos++;
    }

    if (input[pos] == '\0') {
        token.type = TOKEN_EOF;
        return token;
    }

    // Identifiers
    if (isalpha(input[pos])) {
        int i = 0;
        while (isalnum(input[pos])) {
            token.lexeme[i++] = input[pos++];
        }
        token.lexeme[i] = '\0';
        token.type = TOKEN_IDENTIFIER;
        return token;
    }

    // Relational Operators
    if (input[pos] == '<' || input[pos] == '>' || input[pos] == '=' || input[pos] == '!') {
        int i = 0;
        token.lexeme[i++] = input[pos++];
        if (input[pos] == '=' || (token.lexeme[0] == '<' && input[pos] == '>')) {
            token.lexeme[i++] = input[pos++];
        }
        token.lexeme[i] = '\0';
        
        if (strcmp(token.lexeme, "<") == 0 || strcmp(token.lexeme, ">") == 0 ||
            strcmp(token.lexeme, "<=") == 0 || strcmp(token.lexeme, ">=") == 0 ||
            strcmp(token.lexeme, "==") == 0 || strcmp(token.lexeme, "<>") == 0) {
            token.type = TOKEN_RELATIONAL_OP;
            return token;
        }
        // If it's just '=', it might not be a relational op based on the list, but the list says '=='
        // The list: < | >= | <= | <> | == | > | >= (Note: >= is listed twice)
    }

    // Symbols
    if (input[pos] == '-') {
        token.lexeme[0] = input[pos++];
        token.lexeme[1] = '\0';
        token.type = TOKEN_HYPHEN;
        return token;
    }
    if (input[pos] == ',') {
        token.lexeme[0] = input[pos++];
        token.lexeme[1] = '\0';
        token.type = TOKEN_COMMA;
        return token;
    }
    if (input[pos] == '[') {
        token.lexeme[0] = input[pos++];
        token.lexeme[1] = '\0';
        token.type = TOKEN_LBRACKET;
        return token;
    }
    if (input[pos] == ']') {
        token.lexeme[0] = input[pos++];
        token.lexeme[1] = '\0';
        token.type = TOKEN_RBRACKET;
        return token;
    }

    token.lexeme[0] = input[pos++];
    token.lexeme[1] = '\0';
    token.type = TOKEN_ERROR;
    return token;
}

void print_token(Token t) {
    switch (t.type) {
        case TOKEN_IDENTIFIER: printf("IDENTIFIER: %s\n", t.lexeme); break;
        case TOKEN_RELATIONAL_OP: printf("RELATIONAL_OP: %s\n", t.lexeme); break;
        case TOKEN_HYPHEN: printf("HYPHEN: %s\n", t.lexeme); break;
        case TOKEN_COMMA: printf("COMMA: %s\n", t.lexeme); break;
        case TOKEN_LBRACKET: printf("LBRACKET: %s\n", t.lexeme); break;
        case TOKEN_RBRACKET: printf("RBRACKET: %s\n", t.lexeme); break;
        case TOKEN_ERROR: printf("ERROR: Unknown lexeme '%s'\n", t.lexeme); break;
        case TOKEN_EOF: printf("EOF\n"); break;
    }
}

int main() {
    char buffer[1024];
    printf("Enter a command or expression (Ctrl+D to exit):\n");
    while (fgets(buffer, sizeof(buffer), stdin)) {
        input = buffer;
        pos = 0;
        Token t;
        do {
            t = get_next_token();
            if (t.type != TOKEN_EOF) {
                print_token(t);
            }
        } while (t.type != TOKEN_EOF);
        printf("\nEnter next command:\n");
    }
    return 0;
}
