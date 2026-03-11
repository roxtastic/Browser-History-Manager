/* TOLOLOI ILINCA-ROXANA 321CB*/
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#define MAX_URL_LENGTH 50
#define MAX_PAGES 50
#define ERROR_MSG "403 Forbidden"

typedef struct page {
    int id;
    char url[MAX_URL_LENGTH + 1];
    char *desc;
} Page;

typedef struct stack_node {
    Page *page;
    struct stack_node *next;
} StackNode;

typedef struct stack {
    StackNode *top;
    int dim;
} Stack;

typedef struct tab {
    int id;
    Page *currentPage;
    Stack *backwardStack;
    Stack *forwardStack;
} Tab;

typedef struct tab_node {
    Tab *tab;
    struct tab_node *next;
    struct tab_node *prev;
} TabNode;

typedef struct browser {
    Tab *currentTab;
    TabNode *sent;
} Browser;

Stack* createStack();
void push(Stack *stack, Page *page);
Page* pop(Stack *stack);
bool isEmpty(Stack *stack);
void freeStack(Stack *stack);
Page* createPage(int id, const char *url, const char *desc);
Tab* createTab(int id, Page *defaultPage);
Browser* createBrowser(Page *defaultPage);
void newTab(Browser *browser, Page *defaultPage);
void closeTab(Browser *browser);
void openTab(Browser *browser, int id);
void nextTab(Browser *browser);
void prevTab(Browser *browser);
void changePage(Tab *tab, Page *newPage);
void page(Browser *browser, int pageId, Page *pages, int pageCount);
void backward(Tab *tab);
void forward(Tab *tab);
void print(Browser *browser);
void printHistory(Browser *browser, int tabId);
void freeBrowser(Browser *browser);
TabNode* findTab(Browser *browser, int id);