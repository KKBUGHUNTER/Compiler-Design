%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(char *s);
int yylex(void);
extern int yy_scan_string(const char *str);
%}

%token ID

%%
E       : T EPrime ;
EPrime  : '+' T EPrime | /* epsilon */ ;
T       : F TPrime ;
TPrime  : '*' F TPrime | /* epsilon */ ;
F       : '(' E ')' | ID ;
%%

void yyerror(char *s) {
    fprintf(stderr, "Error: %s\n", s);
}

int main(void) {
    char input[100];
    printf("Enter expression: ");
    fgets(input, sizeof(input), stdin);  // Read a full line of input

    // Set the input for the lexer
    yy_scan_string(input);

    if (yyparse() == 0) {
        printf("Input is valid.\n");
    } else {
        printf("Input is invalid.\n");
    }
    return 0;
}

