%{
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Function prototypes
int yylex(void);
void yyerror(const char *s);
%}

%token NUMBER

%left '+' '-'       // Left associativity for + and -
%left '*' '/'       // Left associativity for * and /
%right '^'          // Right associativity for ^ (exponentiation)

%%
// Grammar rules
expr: expr '+' expr  { $$ = $1 + $3; printf("Result: %d\n", $$); }
    | expr '-' expr  { $$ = $1 - $3; printf("Result: %d\n", $$); }
    | expr '*' expr  { $$ = $1 * $3; printf("Result: %d\n", $$); }
    | expr '/' expr  { 
          if ($3 == 0) {
              yyerror("Error: Division by zero!");
              $$ = 0; // Return zero in case of error
          } else {
              $$ = $1 / $3; 
              printf("Result: %d\n", $$);
          }
        }
    | expr '^' expr  { $$ = pow($1, $3); printf("Result: %d\n", $$); }
    | '(' expr ')'   { $$ = $2; }
    | NUMBER         { $$ = $1; }
    ;

%% 

void yyerror(const char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
    printf("Enter expression:\n");
    while (1) {
        yyparse();
    }
    return 0;
}

