#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_TOKENS 100
#define MAX_LEXEME_LEN 50

// Token structure
typedef struct {
    char lexeme[MAX_LEXEME_LEN];
    char type[30];
} Token;

// Symbol table entry structure
typedef struct {
    char identifier[MAX_LEXEME_LEN];
    char type[10];
    int no_of_bytes;
    int address;
    int value;
} SymbolTableEntry;

Token tokens[MAX_TOKENS];
int token_count = 0;
SymbolTableEntry symbol_table[MAX_TOKENS];
int symbol_count = 0;

void addToken(const char *lexeme, const char *type) {
    if (token_count < MAX_TOKENS) {
        strncpy(tokens[token_count].lexeme, lexeme, MAX_LEXEME_LEN - 1);
        tokens[token_count].lexeme[MAX_LEXEME_LEN - 1] = '\0';
        strncpy(tokens[token_count].type, type, 29);
        tokens[token_count].type[29] = '\0';
        token_count++;
    }
}

void addSymbol(const char *identifier, const char *type, int no_of_bytes, int address, int value) {
    if (symbol_count < MAX_TOKENS) {
        strncpy(symbol_table[symbol_count].identifier, identifier, MAX_LEXEME_LEN - 1);
        symbol_table[symbol_count].identifier[MAX_LEXEME_LEN - 1] = '\0';
        strncpy(symbol_table[symbol_count].type, type, 9);
        symbol_table[symbol_count].type[9] = '\0';
        symbol_table[symbol_count].no_of_bytes = no_of_bytes;
        symbol_table[symbol_count].address = address;
        symbol_table[symbol_count].value = value;
        symbol_count++;
    }
}

const char *keywords[] = {
    "auto", "break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum",
    "extern", "float", "for", "goto", "if", "inline", "int", "long", "register", "restrict", "return",
    "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void",
    "volatile", "while", "_Alignas", "_Alignof", "_Atomic", "_Bool", "_Complex", "_Generic", "_Imaginary",
    "_Noreturn", "_Static_assert", "_Thread_local"
};

const char *operators[] = {
    "+", "-", "*", "/", "%", "++", "--", "==", "!=", ">", "<", ">=", "<=", "&&", "||", "!", "&", "|", "^",
    "~", "<<", ">>", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<=", ">>=", "->", ".", "?"
};

const char *specials[] = {
    "{", "}", "[", "]", "(", ")", ",", ";", ":", ".", "&", "*", "+", "-", "~", "!", "/", "%", "<", ">", "^",
    "|", "?", ":", "=", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<", ">>", "<<=", ">>=", "&&", "||",
    "++", "--", "->", "."
};

void tokenize(const char *code) {
    const char *ptr = code;
    char buffer[MAX_LEXEME_LEN];
    int i = 0;

    while (*ptr) {
        if (isspace(*ptr)) {
            ptr++;
            continue;
        }

        if (*ptr == '#') {
            while (*ptr && *ptr != '\n') {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
            addToken(buffer, "preprocessor directive");
            i = 0;
            continue;
        }

        if (isalpha(*ptr) || *ptr == '_') {
            while (isalnum(*ptr) || *ptr == '_') {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
            int is_keyword = 0;
            for (int j = 0; j < sizeof(keywords) / sizeof(keywords[0]); j++) {
                if (strcmp(buffer, keywords[j]) == 0) {
                    addToken(buffer, "keyword");
                    is_keyword = 1;
                    break;
                }
            }
            if (!is_keyword) {
                addToken(buffer, strcmp(buffer, "printf") == 0 ? "function call" : "identifier");
            }
            i = 0;
            continue;
        }

        if (isdigit(*ptr)) {
            while (isdigit(*ptr)) {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
            addToken(buffer, "integer constant");
            i = 0;
            continue;
        }

        int is_operator = 0;
        for (int j = 0; j < sizeof(operators) / sizeof(operators[0]); j++) {
            if (strncmp(ptr, operators[j], strlen(operators[j])) == 0) {
                addToken(operators[j], "operator");
                ptr += strlen(operators[j]);
                is_operator = 1;
                break;
            }
        }
        if (is_operator) continue;

        int is_special = 0;
        for (int j = 0; j < sizeof(specials) / sizeof(specials[0]); j++) {
            if (strncmp(ptr, specials[j], strlen(specials[j])) == 0) {
                addToken(specials[j], "special character");
                ptr += strlen(specials[j]);
                is_special = 1;
                break;
            }
        }
        if (is_special) continue;

        if (*ptr == '\"') {
            buffer[i++] = *ptr++;
            while (*ptr && *ptr != '\"') {
                buffer[i++] = *ptr++;
            }
            if (*ptr == '\"') {
                buffer[i++] = *ptr++;
            }
            buffer[i] = '\0';
            addToken(buffer, "string literal");
            i = 0;
            continue;
        }
        ptr++;
    }
}

void buildSymbolTable() {
    int address_counter = 1000;
    for (int i = 0; i < token_count; i++) {
        if (strcmp(tokens[i].type, "keyword") == 0 && strcmp(tokens[i].lexeme, "int") == 0) {
            while (i < token_count && strcmp(tokens[i].lexeme, ";") != 0) {
                if (strcmp(tokens[i].type, "identifier") == 0) {
                    char *identifier = tokens[i].lexeme;
                    int value = 0;
                    if (i + 1 < token_count && strcmp(tokens[i + 1].lexeme, "=") == 0) {
                        value = atoi(tokens[i + 2].lexeme);
                        i += 2;
                    }
                    addSymbol(identifier, "int", 2, address_counter, value);
                    address_counter += 2;
                }
                i++;
            }
        }
    }
}

void printTokens() {
    for (int i = 0; i < token_count; i++) {
        printf("%s - %s\n", tokens[i].lexeme, tokens[i].type);
    }
}

void printSymbolTable() {
    printf("Content of Symbol Table\n");
    printf("--------------------------------------------------\n");
    printf("|Identifier| Type | No of bytes | Address | Value |\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < symbol_count; i++) {
        printf("| %-9s | %-4s | %-11d | %-7d | %-5d |\n",
               symbol_table[i].identifier,
               symbol_table[i].type,
               symbol_table[i].no_of_bytes,
               symbol_table[i].address,
               symbol_table[i].value);
    }
    printf("--------------------------------------------------\n");
}

int main() {
    FILE *file = fopen("input.c", "r");
    if (file == NULL) {
        perror("Error opening file");
        return EXIT_FAILURE;
    }
    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char *code = malloc(length + 1);
    if (code == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return EXIT_FAILURE;
    }
    fread(code, 1, length, file);
    code[length] = '\0';
    fclose(file);

    tokenize(code);
    buildSymbolTable();
    printTokens();
    printSymbolTable();

    free(code);
    return 0;
}
