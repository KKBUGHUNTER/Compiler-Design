%{
#include "y.tab.h"
int yywrap(void) { return 1; }
%}

%%
id      { return ID; }       
"+"     { return '+'; }   
"*"     { return '*'; } 
"("     { return '('; }
")"     { return ')'; }
[ \t\n]+ ;
.       { return yytext[0]; }
<<EOF>> { return 0; }
%%
