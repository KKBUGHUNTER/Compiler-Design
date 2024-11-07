%{
#include "y.tab.h"
#include <stdio.h>
#include <stdlib.h>
%}

%% 
[0-9]+      { yylval = atoi(yytext); return NUMBER; }    // Recognizes numbers and sets yylval
"+"         { return '+'; }                              // Recognizes the '+' operator
"-"         { return '-'; }                              // Recognizes the '-' operator
"*"         { return '*'; }                              // Recognizes the '*' operator
"/"         { return '/'; }                              // Recognizes the '/' operator
"^"         { return '^'; }                              // Recognizes the '^' operator (exponentiation)
"("         { return '('; }                              // Recognizes the opening parenthesis
")"         { return ')'; }                              // Recognizes the closing parenthesis
[ \t\n]+     { /* ignore whitespace */ }                 // Ignores spaces, tabs, and newlines
.           { printf("Unknown character: '%s'\n", yytext); } // Prints an error message for unknown characters

%%
int yywrap() {
    return 1;  // Necessary for Lex to signal the end of input
}

