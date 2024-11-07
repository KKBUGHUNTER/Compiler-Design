#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char input[100];
int pos = 0;

// Forward declarations for each non-terminal function
void E();
void EPrime();
void T();
void TPrime();
void F();

void error()
{
    printf("Error: syntax error\n");
    exit(1);
}

// Check if the current character matches the expected token
void match(char expected)
{
    if (input[pos] == expected)
    {
        pos++;
    }
    else
    {
        error();
    }
}

// Non-terminal functions for each rule
void E()
{
    T();
    EPrime();
}

void EPrime()
{
    if (input[pos] == '+')
    {
        match('+');
        T();
        EPrime();
    }
    // epsilon case - do nothing
}

void T()
{
    F();
    TPrime();
}

void TPrime()
{
    if (input[pos] == '*')
    {
        match('*');
        F();
        TPrime();
    }
    // epsilon case - do nothing
}

void F()
{
    if (input[pos] == '(')
    {
        match('(');
        E();
        match(')');
    }
    else if (strncmp(&input[pos], "id", 2) == 0)
    {
        pos += 2; // Advance position for "id"
    }
    else
    {
        error();
    }
}

int main()
{
    printf("Implementation of Recursive Descent Parser\n");
    printf("Enter expression: ");
    scanf("%s", input);

    pos = 0;
    E();

    // Check if we've reached the end of input
    if (input[pos] == '\0')
    {
        printf("Input is valid.\n");
    }
    else
    {
        printf("Input is invalid.\n");
    }

    return 0;
}
