#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX 50

typedef struct node {
    struct node *lchild;
    char info;
    struct node *rchild;
} Node;

// Global stack
static Node* stack[MAX];
static int top = -1;

// Function prototypes
static inline int isOperator(char c);
static Node* createNode(char x);
Node* buildTree(const char postfix[]);
void prefix(const Node *root);
void postfix(const Node *root);
void parenthesizedInfix(const Node *root);
void display(const Node *root, int level);
int evaluate(const Node *root);
void freeTree(Node *root);

// Stack operations
static inline int isEmpty(void) { return top == -1; }
static inline int isFull(void) { return top == MAX - 1; }

static inline void push(Node* x) {
    if (isFull()) {
        fprintf(stderr, "Stack Overflow\n");
        exit(EXIT_FAILURE);
    }
    stack[++top] = x;
}

static inline Node* pop(void) {
    if (isEmpty()) {
        fprintf(stderr, "Stack Underflow\n");
        exit(EXIT_FAILURE);
    }
    return stack[top--];
}

int main(void) {
    const char postfixExpr[] = "54+12*3*-";
    
    Node *root = buildTree(postfixExpr);
    
    display(root, 0);
    printf("\n");
    
    printf("Prefix : ");
    prefix(root);
    
    printf("Postfix : ");
    postfix(root);
    
    printf("Infix : ");
    parenthesizedInfix(root);
    
    printf("Value : %d\n", evaluate(root));
    
    freeTree(root);
    return 0;
}

static inline int isOperator(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/' || c == '^');
}

static Node* createNode(char x) {
    Node *p = (Node*)malloc(sizeof(Node));
    if (!p) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    p->info = x;
    p->lchild = NULL;
    p->rchild = NULL;
    return p;
}

Node* buildTree(const char postfix[]) {
    int len = strlen(postfix);
    
    for (int i = 0; i < len; i++) {
        Node *t = createNode(postfix[i]);
        
        if (isOperator(postfix[i])) {
            t->rchild = pop();
            t->lchild = pop();
        }
        push(t);
    }
    
    return pop();
}

static void preorder(const Node *p) {
    if (!p) return;
    
    printf("%c", p->info);
    preorder(p->lchild);
    preorder(p->rchild);
}

void prefix(const Node *root) {
    preorder(root);
    printf("\n");
}

static void postorder(const Node *p) {
    if (!p) return;
    
    postorder(p->lchild);
    postorder(p->rchild);
    printf("%c", p->info);
}

void postfix(const Node *root) {
    postorder(root);
    printf("\n");
}

static void inorder(const Node *p) {
    if (!p) return;
    
    if (isOperator(p->info))
        printf("(");
    
    inorder(p->lchild);
    printf("%c", p->info);
    inorder(p->rchild);
    
    if (isOperator(p->info))
        printf(")");
}

void parenthesizedInfix(const Node *root) {
    inorder(root);
    printf("\n");
}

void display(const Node *p, int level) {
    if (!p) return;
    
    display(p->rchild, level + 1);
    printf("\n");
    
    for (int i = 0; i < level; i++)
        printf("    ");
    
    printf("%c", p->info);
    display(p->lchild, level + 1);
}

int evaluate(const Node *p) {
    if (!p) return 0;
    
    if (!isOperator(p->info))
        return p->info - '0';
    
    int leftValue = evaluate(p->lchild);
    int rightValue = evaluate(p->rchild);
    
    switch (p->info) {
        case '+': return leftValue + rightValue;
        case '-': return leftValue - rightValue;
        case '*': return leftValue * rightValue;
        case '/': 
            if (rightValue == 0) {
                fprintf(stderr, "Division by zero\n");
                exit(EXIT_FAILURE);
            }
            return leftValue / rightValue;
        case '^': {
            int result = 1;
            for (int i = 0; i < rightValue; i++)
                result *= leftValue;
            return result;
        }
        default: return 0;
    }
}

void freeTree(Node *root) {
    if (!root) return;
    
    freeTree(root->lchild);
    freeTree(root->rchild);
    free(root);
}
