#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#define MAX_PATH 256     // For file path
#define MAX_STACK 1024   // Stack size for brackets

#define NORMAL 0
#define IN_STRING 1
#define IN_CHAR 2
#define IN_COMMENT 3
#define IN_LINE_COMMENT 4

//#define DEBUG

typedef struct {
    char arr[MAX_STACK];
    int top;
} Stack;

void initialize(Stack *stack){
    stack->top = -1;
}

bool isEmpty(Stack *stack) {
    return stack->top == -1;
}

bool isFull(Stack *stack) {
    return stack->top == MAX_STACK - 1;
}

void push(Stack *stack, char value) {
    if (isFull(stack)) {
        printf("Stack Overflow\n");
        return;
    }

    stack->arr[++stack->top] = value;

#ifdef DEBUG
    printf("Char %c added to stack\n", value);
#endif
}

char pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack Underflow\n");
        return -1;
    }

    char popped  = stack->arr[stack->top--];

#ifdef DEBUG
    printf("Popped %c from the stack\n", popped);
#endif

    return popped;
}

char peek(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Stack is empty\n");
        return -1;
    }

    return stack->arr[stack->top];
}

int stateMachine(char current, int prev, int state){
    if (state == NORMAL) {
        if (prev == '/' && current == '/') return IN_LINE_COMMENT;
        if (prev == '/' && current == '*') return IN_COMMENT;
        if (current == '\"') return IN_STRING;
        if (current == '\'') return IN_CHAR;
    } else {
        if (state == IN_LINE_COMMENT && current == '\n') return NORMAL;
        if (state == IN_COMMENT && prev == '*' && current == '/') return NORMAL;
        if (state == IN_STRING && current == '\"' && prev != '\\') return NORMAL;
        if (state == IN_CHAR && current == '\'' && prev != '\\') return NORMAL;
    }

    return state;
}

FILE* openFile(char filePath[]){
    FILE* fptr = fopen(filePath, "r");

    if (fptr == NULL){
        printf("File opening failed\n");
        return NULL;
    }

    return fptr;
}

void readFile(FILE* ptr){
    Stack stack;
    initialize(&stack);

    int c, prev = 0;
    int state = NORMAL;
    int line = 1;

    while ((c = fgetc(ptr)) != EOF) {
        int newState = stateMachine(c, prev, state);

        if (c == '\n') ++line;

        if (state == NORMAL) {
            if (c == '(' || c == '{' || c == '[') {
                push(&stack, c);
            } else if (c == ')' || c == '}' || c == ']') {
                char open = pop(&stack);

                if ((c == ')' && open != '(') ||
                    (c == '}' && open != '{') ||
                    (c == ']' && open != '[')) {
                    printf("Mismatched %c at line %d\n", c, line);
                }
            }
        }

        state = newState;
        prev = c;
    }

    // Handle unmatched opening brackets
    if (!isEmpty(&stack)) {
        printf("Error: Unmatched opening brackets remain:\n");
        while (!isEmpty(&stack)) {
            char unmatched = pop(&stack);
            printf("  Unmatched opening '%c'\n", unmatched);
        }
    }

    fclose(ptr); // Important!
}

void handleFile(){
    char str[MAX_PATH];
    printf("Please enter file path to scan:\n");

    fgets(str, MAX_PATH, stdin);
    str[strcspn(str, "\n")] = '\0'; // remove newline

    FILE *ptr = openFile(str);
    if (ptr != NULL) {
        readFile(ptr);
    }
}

int main(){
    handleFile();
    return 0;
}
