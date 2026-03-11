#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "functii.h"

// Operatii pe stiva
Stack* createStack() {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    if (!stack) return NULL;
    stack->top = NULL;
    stack->dim = 0;
    return stack;
}

void push(Stack *stack, Page *page) {
    StackNode *newNode = (StackNode*)malloc(sizeof(StackNode));
    if (!newNode) return;
    newNode->page = page;
    newNode->next = stack->top;
    stack->top = newNode;
    stack->dim++;
}

Page* pop(Stack *stack) {
    if (isEmpty(stack)) return NULL;
    StackNode *temp = stack->top;
    Page *page = temp->page;
    stack->top = temp->next;
    free(temp);
    stack->dim--;
    return page;
}

bool isEmpty(Stack *stack) {
    return stack->top == NULL;
}

void freeStack(Stack *stack) {
    if (!stack) return;
    while (!isEmpty(stack)) {
        pop(stack);
    }
    free(stack);
}

// Operatii pe pagini
Page* createPage(int id, const char *url, const char *desc) {
    Page *page = (Page*)malloc(sizeof(Page));
    if (!page) return NULL;
    page->id = id;
    strncpy(page->url, url, MAX_URL_LENGTH);
    page->url[MAX_URL_LENGTH] = '\0';

    size_t descLen = strlen(desc) + 1;
    page->desc = (char*)malloc(descLen);
    if (!page->desc) {
        free(page);
        return NULL;
    }
    strcpy(page->desc, desc);
    return page;
}

void freePage(Page *page) {
    if (page) {
        free(page->desc);
        free(page);
    }
}

// Operatii pe taburi
Tab* createTab(int id, Page *defaultPage) {
    Tab *tab = (Tab*)malloc(sizeof(Tab));
    if (!tab) return NULL;
    tab->id = id;
    tab->currentPage = defaultPage;
    tab->backwardStack = createStack();
    tab->forwardStack = createStack();

    if (!tab->backwardStack || !tab->forwardStack) {
        if (tab->backwardStack) freeStack(tab->backwardStack);
        if (tab->forwardStack) freeStack(tab->forwardStack);
        free(tab);
        return NULL;
    }
    return tab;
}

void freeTab(Tab *tab) {
    if (tab) {
        freeStack(tab->backwardStack);
        freeStack(tab->forwardStack);
        free(tab);
    }
}

// Operatii pe browser
Browser* createBrowser(Page *defaultPage) {
    Browser *browser = (Browser*)malloc(sizeof(Browser));
    if (!browser) return NULL;

    // Creez nodul santinela
    TabNode *sent = (TabNode*)malloc(sizeof(TabNode));
    if (!sent) {
        free(browser);
        return NULL;
    }

    // Creez tabul initial
    Tab *initialTab = createTab(0, defaultPage);
    if (!initialTab) {
        free(sent);
        free(browser);
        return NULL;
    }

    // Creez lista dublu inlantuita circulara cu santinela
    sent->next = sent;
    sent->prev = sent;

    // Adaug tabul initial
    TabNode *newNode = (TabNode*)malloc(sizeof(TabNode));
    if (!newNode) {
        freeTab(initialTab);
        free(sent);
        free(browser);
        return NULL;
    }

    newNode->tab = initialTab;
    newNode->next = sent;
    newNode->prev = sent->prev;
    sent->prev->next = newNode;
    sent->prev = newNode;

    browser->sent = sent;
    browser->currentTab = initialTab;

    return browser;
}

void newTab(Browser *browser, Page *defaultPage) {
    if (!browser || !browser->sent) return;

    // Caut ultimul tab pentru a determina noul ID
    int newId = 0;
    TabNode *current = browser->sent->next;
    while (current != browser->sent) {
        if (current->tab->id >= newId) {
            newId = current->tab->id + 1;
        }
        current = current->next;
    }

    // Creez tabul nou
    Tab *newTab = createTab(newId, defaultPage);
    if (!newTab) return;

    // Adaug tabul la sfarsitul listei (inainte de santinela)
    TabNode *newNode = (TabNode*)malloc(sizeof(TabNode));
    if (!newNode) {
        freeTab(newTab);
        return;
    }

    newNode->tab = newTab;
    newNode->next = browser->sent;
    newNode->prev = browser->sent->prev;
    browser->sent->prev->next = newNode;
    browser->sent->prev = newNode;

    browser->currentTab = newTab;
}

void closeTab(Browser *browser) {
    if (!browser || !browser->sent || !browser->currentTab) return;

    // Nu se poate inchide tabul cu ID-ul 0
    if (browser->currentTab->id == 0) {
        printf("%s\n", ERROR_MSG);
        return;
    }

    TabNode *toRemove = NULL;
    TabNode *current = browser->sent->next;

    // Caut tabul de eliminat
    while (current != browser->sent) {
        if (current->tab == browser->currentTab) {
            toRemove = current;
            break;
        }
        current = current->next;
    }

    if (!toRemove) return;

    // Setez tabul curent ca fiind cel anterior (sau urmatorul daca e la inceput)
    TabNode *newCurrent = toRemove->prev;
    if (newCurrent == browser->sent) {
        newCurrent = browser->sent->prev;
    }
    browser->currentTab = newCurrent->tab;

    // Elimin din lista
    toRemove->prev->next = toRemove->next;
    toRemove->next->prev = toRemove->prev;

    // Eliberez memoria
    freeTab(toRemove->tab);
    free(toRemove);
}

TabNode* findTab(Browser *browser, int id) {
    if (!browser || !browser->sent) return NULL;
    TabNode *current = browser->sent->next;
    while (current != browser->sent) {
        if (current->tab->id == id) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void openTab(Browser *browser, int id) {
    if (!browser || !browser->sent) return;
    TabNode *found = findTab(browser, id);
    if (!found) {
        printf("%s\n", ERROR_MSG);
        return;
    }
    browser->currentTab = found->tab;
}

void nextTab(Browser *browser) {
    if (!browser || !browser->sent || !browser->currentTab) return;
    TabNode *current = browser->sent->next;
    while (current != browser->sent) {
        if (current->tab == browser->currentTab) {
            browser->currentTab = (current->next != browser->sent) ? current->next->tab : browser->sent->next->tab;
            return;
        }
        current = current->next;
    }
}

void prevTab(Browser *browser) {
    if (!browser || !browser->sent || !browser->currentTab) return;
    TabNode *current = browser->sent->next;
    while (current != browser->sent) {
        if (current->tab == browser->currentTab) {
            browser->currentTab = (current->prev != browser->sent) ? current->prev->tab : browser->sent->prev->tab;
            return;
        }
        current = current->next;
    }
}

void changePage(Tab *tab, Page *newPage) {
    if (!tab || !newPage) return;
    if (tab->currentPage) {
        push(tab->backwardStack, tab->currentPage);
    }
    while (!isEmpty(tab->forwardStack)) {
        pop(tab->forwardStack);
    }
    tab->currentPage = newPage;
}

void page(Browser *browser, int pageId, Page *pages, int pageCount) {
    if (!browser || !browser->currentTab || !pages) return;
    Page *found = NULL;
    for (int i = 0; i < pageCount; i++) {
        if (pages[i].id == pageId) {
            found = &pages[i];
            break;
        }
    }
    if (!found) {
        printf("%s\n", ERROR_MSG);
        return;
    }
    changePage(browser->currentTab, found);
}

void backward(Tab *tab) {
    if (!tab || isEmpty(tab->backwardStack)) {
        printf("%s\n", ERROR_MSG);
        return;
    }
    if (tab->currentPage) {
        push(tab->forwardStack, tab->currentPage);
    }
    tab->currentPage = pop(tab->backwardStack);
}

void forward(Tab *tab) {
    if (!tab || isEmpty(tab->forwardStack)) {
        printf("%s\n", ERROR_MSG);
        return;
    }
    if (tab->currentPage) {
        push(tab->backwardStack, tab->currentPage);
    }
    tab->currentPage = pop(tab->forwardStack);
}

void print(Browser *browser) {
    if (!browser || !browser->currentTab) return;
    TabNode *start = NULL;
    TabNode *current = browser->sent->next;
    while (current != browser->sent) {
        if (current->tab == browser->currentTab) {
            start = current;
            break;
        }
        current = current->next;
    }

    if (!start) return;

    TabNode *temp = start;
    do {
        printf("%d ", temp->tab->id);
        temp = temp->next;
        if (temp == browser->sent) {
            temp = temp->next; // sar peste santinela
        }
    } while (temp != start);

    printf("\n%s\n", browser->currentTab->currentPage->desc);
}

void printStackURLs(Stack *stack) {
    Stack *tempStack = createStack();
    if (!tempStack) return;

    while (!isEmpty(stack)) {
        Page *page = pop(stack);
        push(tempStack, page);
    }

    while (!isEmpty(tempStack)) {
        Page *page = pop(tempStack);
        printf("%s\n", page->url);
        push(stack, page);
    }

    freeStack(tempStack);
}

void printHistory(Browser *browser, int tabId) {
    if (!browser || !browser->sent) return;
    TabNode *found = findTab(browser, tabId);
    if (!found) {
        printf("%s\n", ERROR_MSG);
        return;
    }

    Tab *tab = found->tab;
    printStackURLs(tab->forwardStack);
    if (tab->currentPage) printf("%s\n", tab->currentPage->url);
    printStackURLs(tab->backwardStack);
}

void freeBrowser(Browser *browser) {
    if (!browser) return;
    if (browser->sent) {
        TabNode *current = browser->sent->next;
        while (current != browser->sent) {
            TabNode *next = current->next;
            freeTab(current->tab);
            free(current);
            current = next;
        }
        free(browser->sent);
    }
    free(browser);
}


int main() {
    FILE *input = fopen("tema1.in", "r");
    FILE *output = fopen("tema1.out", "w");
    if (!input || !output) {
        if (input) fclose(input);
        if (output) fclose(output);
        return 1;
    }
    if (dup2(fileno(output), STDOUT_FILENO) == -1) {
        fclose(input);
        fclose(output);
        return 1;
    }
    Page *defaultPage = createPage(0, "https://acs.pub.ro/", "Computer Science");
    if (!defaultPage) {
        fclose(input);
        fclose(output);
        fprintf(stderr, "Eroare");
        return 1;
    }
    int pageCount;
    fscanf(input, "%d", &pageCount);
    Page pages[MAX_PAGES];
    for (int i = 0; i < pageCount; i++) {
        fscanf(input, "%d", &pages[i].id);
        fscanf(input, "%s", pages[i].url);
        char desc[1024];
        fgetc(input);
        fgets(desc, sizeof(desc), input);
        size_t len = strlen(desc);
        if (len > 0 && desc[len-1] == '\n') {
            desc[len-1] = '\0';
        }
        pages[i].desc = strdup(desc);
    }
    Browser *browser = createBrowser(defaultPage);
    if (!browser) {
        freePage(defaultPage);
        for (int i = 0; i < pageCount; i++) {
            free(pages[i].desc);
        }
        fclose(input);
        fclose(output);
        fprintf(stderr, "Eroare");
        return 1;
    }
    int commandCount;
    fscanf(input, "%d", &commandCount);
    fgetc(input);
    char command[20];
    for (int i = 0; i < commandCount; i++) {
        fscanf(input, "%s", command);
        if (strcmp(command, "NEW_TAB") == 0) {
            newTab(browser, defaultPage);
        } else if (strcmp(command, "CLOSE") == 0) {
            closeTab(browser);
        } else if (strcmp(command, "OPEN") == 0) {
            int id;
            fscanf(input, "%d", &id);
            openTab(browser, id);
        } else if (strcmp(command, "NEXT") == 0) {
            nextTab(browser);
        } else if (strcmp(command, "PREV") == 0) {
            prevTab(browser);
        } else if (strcmp(command, "PAGE") == 0) {
            int pageId;
            fscanf(input, "%d", &pageId);
            page(browser, pageId, pages, pageCount);
        } else if (strcmp(command, "BACKWARD") == 0) {
            backward(browser->currentTab);
        } else if (strcmp(command, "FORWARD") == 0) {
            forward(browser->currentTab);
        } else if (strcmp(command, "PRINT") == 0) {
            print(browser);
        } else if (strcmp(command, "PRINT_HISTORY") == 0) {
            int tabId;
            fscanf(input, "%d", &tabId);
            printHistory(browser, tabId);
        }
        while (fgetc(input) != '\n' && !feof(input));
    }
    for (int i = 0; i < pageCount; i++) {
        free(pages[i].desc);
    }
    freeBrowser(browser);
    fclose(input);
    fclose(output);
    freePage(defaultPage);
    return 0;
}
