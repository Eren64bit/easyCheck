#include <stdio.h>
#include <string.h>
#include <stdbool.h>



#define MAX_LIMIT 40
#define NORMAL 0
#define IN_STRING 1
#define IN_CHAR 2
#define IN_COMMENT 3
#define IN_LINE_COMMENT 4
/*exercise 1-24. Write a program to check a C program for rudimentary syntax errors like
unmatched parentheses, brackets and braces. Don't forget about quotes, both single and
double, escape sequences, and comments. (This program is hard if you do it in full generality.)*/

//char size = 1 byte


typedef struct {
    char arr[MAX_LIMIT];
    int top;
} Stack;

void initialize(Stack *stack){
    stack->top = -1;
}

bool isEmpty(Stack *stack) {

    return stack->top == -1;
}

bool isFull(Stack *stack) {
    return stack->top == MAX_LIMIT - 1;
}

void push(Stack *stack, char value) {

    if (isFull(stack)) {
        printf("Stack Overflow\n");
        return;
    }

    stack->arr[++stack->top] = value;
    printf("Char %c added to stack", value);
}

char pop(Stack *stack) {

    if (isEmpty(stack)) {
        printf("Stack Underflow\n");
        return -1;
    }

    char popped  = stack->arr[stack->top];

    stack->top--;

    printf("Popped %c from the stack \n", popped);
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
    }else if (state != NORMAL) {
        if (state == IN_LINE_COMMENT && current == '\n') return NORMAL; // end of comment 
        if (state == IN_COMMENT && prev == '*' && current == '/') return NORMAL;
        if (state == IN_STRING && current == '\"' && prev != '//') return NORMAL;
        if (state == IN_CHAR && current == '\''&&  prev != '//') return NORMAL;
    }
    return state;
}


FILE* openFile(char filePath[]){
    FILE* fptr = fopen(filePath, "r");

    if (fptr == NULL){
        printf("File openning failed");
        return NULL;
    }

    return fptr;
}

void readFile(FILE* ptr){
    int c, prev = 0;
    int state = NORMAL;

    while ((c = fgetc(ptr)) != EOF) {
        state = stateMachine(c, prev, state);
        prev = c;
    }
}

void handleFile(){

    char str[MAX_LIMIT];
    printf("Please enter file path which you want to scan\n");

    fgets(str, MAX_LIMIT, stdin);

    str[strcspn(str, "\n")] = '\0';

    FILE *ptr = openFile(str);

    readFile(ptr);

}

int main(){

    handleFile();
    return 0;
}

