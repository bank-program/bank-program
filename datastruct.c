#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct Node {
    float data;
    struct Node* next;
} Node;

typedef struct Stack {
    Node* top;
} Stack;

Stack* initialize() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = NULL;
    return s;
}

int isEmpty(Stack* s) {
    if (s->top == NULL) {
        return 1;
    } else {
        return 0;
    }
}

void push(Stack* s, float value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->next = s->top;
    s->top = newNode;
}

float pop(Stack* s) {
    if (isEmpty(s)) {
        return -1;
    }
    Node* temp = s->top;
    float poppedValue = temp->data;
    s->top = temp->next;
    free(temp);
    return poppedValue;
}

float peek(Stack* s) {
    if (isEmpty(s)) {
        return -1;
    }
    return s->top->data;
}

int precedence(char op) {
    switch(op) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return 0;
    }
}

int isOperator(char ch) {
    return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
}

char* infixToPostfix(char* infix) {
    Stack* s = initialize();
    int len = strlen(infix);
    char* postfix = (char*)malloc(len * 2);
    int k = 0;

    for (int i = 0; i < len; i++) {
        char ch = infix[i];

        if (isalnum(ch)) {
            postfix[k++] = ch;
        } else if (ch == '(') {
            push(s, ch);
        } else if (ch == ')') {
            while (!isEmpty(s) && peek(s) != '(') {
                postfix[k++] = pop(s);
            }
            pop(s);
        } else if (isOperator(ch)) {
            while (!isEmpty(s) && precedence(peek(s)) >= precedence(ch)) {
                postfix[k++] = pop(s);
            }
            push(s, ch);
        }
    }

    while (!isEmpty(s)) {
        postfix[k++] = pop(s);
    }

    postfix[k] = '\0';
    return postfix;
}

float evaluatePostfix(char* postfix) {
    Stack* s = initialize();
    for (int i = 0; postfix[i] != '\0'; i++) {
        char ch = postfix[i];

        if (isdigit(ch)) {
            push(s, ch - '0');
        } else {
            float val2 = pop(s);
            float val1 = pop(s);
            switch (ch) {
                case '+': push(s, val1 + val2); break;
                case '-': push(s, val1 - val2); break;
                case '*': push(s, val1 * val2); break;
                case '/': push(s, val1 / val2); break;
                case '^': {
                    float result = 1;
                    for (int j = 0; j < (int)val2; j++) result *= val1;
                    push(s, result);
                    break;
                }
            }
        }
    }
    return pop(s);
}
