#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 100

// Stack structure for characters (used in Infix to Postfix conversion)
typedef struct {
    char items[MAX];
    int top;
} CharStack;

// Stack structure for integers (used in Postfix evaluation)
typedef struct {
    int items[MAX];
    int top;
} IntStack;

// --- Char Stack Operations ---
void initCharStack(CharStack *s) { s->top = -1; }
int isCharFull(CharStack *s) { return s->top == MAX - 1; }
int isCharEmpty(CharStack *s) { return s->top == -1; }
void pushChar(CharStack *s, char c) {
    if (!isCharFull(s)) s->items[++(s->top)] = c;
}
char popChar(CharStack *s) {
    if (!isCharEmpty(s)) return s->items[(s->top)--];
    return '\0';
}
char peekChar(CharStack *s) {
    if (!isCharEmpty(s)) return s->items[s->top];
    return '\0';
}

// --- Int Stack Operations ---
void initIntStack(IntStack *s) { s->top = -1; }
int isIntFull(IntStack *s) { return s->top == MAX - 1; }
int isIntEmpty(IntStack *s) { return s->top == -1; }
void pushInt(IntStack *s, int val) {
    if (!isIntFull(s)) s->items[++(s->top)] = val;
}
int popInt(IntStack *s) {
    if (!isIntEmpty(s)) return s->items[(s->top)--];
    return 0;
}

// Function to determine operator precedence
int precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Function to check if a character is an operator
int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Function to convert Infix expression to Postfix expression
void infixToPostfix(char *infix, char *postfix) {
    CharStack s;
    initCharStack(&s);
    int i = 0, j = 0;

    while (infix[i] != '\0') {
        char token = infix[i];

        // Skip whitespace
        if (token == ' ' || token == '\t') {
            i++;
            continue;
        }

        // If the token is an operand, add it to the postfix output
        if (isalnum(token)) {
            postfix[j++] = token;
        }
        // If the token is '(', push it to the stack
        else if (token == '(') {
            pushChar(&s, token);
        }
        // If the token is ')', pop and output until '(' is found
        else if (token == ')') {
            while (!isCharEmpty(&s) && peekChar(&s) != '(') {
                postfix[j++] = popChar(&s);
            }
            popChar(&s); // Remove '(' from stack
        }
        // If an operator is encountered
        else if (isOperator(token)) {
            while (!isCharEmpty(&s) && precedence(peekChar(&s)) >= precedence(token)) {
                postfix[j++] = popChar(&s);
            }
            pushChar(&s, token);
        }
        i++;
    }

    // Pop all remaining operators from the stack
    while (!isCharEmpty(&s)) {
        postfix[j++] = popChar(&s);
    }
    postfix[j] = '\0';
}

// Function to evaluate a Postfix expression
int evaluatePostfix(char *postfix) {
    IntStack s;
    initIntStack(&s);
    int i = 0;

    while (postfix[i] != '\0') {
        char token = postfix[i];

        // Skip whitespace
        if (token == ' ' || token == '\t') {
            i++;
            continue;
        }

        // If token is a digit, convert to integer and push to stack
        if (isdigit(token)) {
            pushInt(&s, token - '0');
        }
        // If token is an operator, pop two operands, compute, and push result
        else if (isOperator(token)) {
            int val2 = popInt(&s);
            int val1 = popInt(&s);
            switch (token) {
                case '+': pushInt(&s, val1 + val2); break;
                case '-': pushInt(&s, val1 - val2); break;
                case '*': pushInt(&s, val1 * val2); break;
                case '/': 
                    if (val2 == 0) {
                        printf("Error: Division by zero!\n");
                        exit(1);
                    }
                    pushInt(&s, val1 / val2); 
                    break;
            }
        }
        i++;
    }
    return popInt(&s);
}

// Main function
int main() {
    char infix[MAX], postfix[MAX];

    printf("========================================\n");
    printf("   EXPRESSION EVALUATION SYSTEM (STACK) \n");
    printf("========================================\n");
    printf("Enter a valid infix expression (single-digit numbers, e.g., 3 + 5 * (2 - 1)):\n> ");
    
    fgets(infix, MAX, stdin);
    infix[strcspn(infix, "\n")] = 0; // Remove trailing newline

    // Step 1: Convert Infix to Postfix
    infixToPostfix(infix, postfix);
    printf("\n[+] Postfix Expression : %s\n", postfix);

    // Step 2: Evaluate Postfix Expression
    int result = evaluatePostfix(postfix);
    printf("[+] Final Evaluated Result: %d\n", result);

    return 0;
}
