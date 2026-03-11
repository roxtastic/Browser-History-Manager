# Browser-History-Manager
A simulation of a web browser navigation system implementing tabs, page navigation, and history management using stacks and circular doubly linked lists with a sentinel node.

# Browser Navigation System (C)

A low-level simulation of a web browser navigation system implemented in **C**, focusing on **data structure design, memory management, and modular architecture**.

The project models how a browser manages **tabs, page navigation, and browsing history**, using classic data structures such as **stacks** and **circular doubly linked lists with a sentinel node**.

This project was developed as part of a **Data Structures and Algorithms assignment** and demonstrates practical implementation of dynamic memory structures and navigation logic.

---

# Features

* Multiple browser **tabs**
* **Backward / Forward navigation history**
* **Dynamic tab creation and deletion**
* Page navigation between up to **50 stored pages**
* **Circular tab navigation**
* Memory-safe dynamic allocation and deallocation
* Fully modular architecture

---

# System Architecture

The browser system is organized in three main levels:

```
Browser
 └── Tabs (Circular Doubly Linked List with Sentinel)
      └── Pages
           └── Navigation History (Stacks: Back / Forward)
```

### Browser

The browser maintains:

* a pointer to the **current tab**
* a **circular doubly linked list with a sentinel node** that stores all open tabs

This structure allows efficient tab insertion, deletion, and navigation.

---

# Data Structures

## Page

Represents a webpage opened in a tab.

```c
typedef struct Page {
    int id;
    char *url;
    char *description;
} Page;
```

Fields:

* `id` – unique page identifier
* `url` – webpage address
* `description` – textual description of the page

Default page:

```
ID: 0
URL: https://acs.pub.ro/
Description: Computer Science
```

---

## StackNode

Node used in stack implementation.

```c
typedef struct StackNode {
    Page *page;
    struct StackNode *next;
} StackNode;
```

Each node stores a page and a pointer to the next node.

---

## Stack

Used to store browsing history.

```c
typedef struct Stack {
    StackNode *top;
    int size;
} Stack;
```

Each tab contains two stacks:

* **Back Stack** – pages previously visited
* **Forward Stack** – pages available for forward navigation

---

## Tab

Represents an open browser tab.

```c
typedef struct Tab {
    int id;
    Page *currentPage;
    Stack *back;
    Stack *forward;
} Tab;
```

Responsibilities:

* store the current page
* manage backward and forward navigation history

---

## TabNode

Node used in the circular doubly linked list of tabs.

```c
typedef struct TabNode {
    Tab *tab;
    struct TabNode *prev;
    struct TabNode *next;
} TabNode;
```

This structure allows:

* navigation between tabs
* dynamic insertion and deletion

---

# Circular Doubly Linked List with Sentinel

The browser uses a **circular doubly linked list with a sentinel node** to store tabs.

Characteristics:

* each node has `prev` and `next` pointers
* the list is **circular**
* a **sentinel node** simplifies insertion and deletion

Structure:

```
Sentinel <-> Tab1 <-> Tab2 <-> Tab3
   ^                                |
   |________________________________|
```

Advantages:

* no NULL edge cases
* efficient insertion and deletion
* easy circular navigation between tabs

---

# Core Functionalities

## Stack Operations

### createStack()

Creates an empty stack.

### push()

Adds a page to the top of the stack.

### pop()

Removes the top page from the stack.

### isEmpty()

Checks whether the stack contains elements.

### freeStack()

Releases all memory allocated for the stack.

---

## Page Management

### createPage()

Allocates and initializes a page structure.

### freePage()

Releases memory allocated for a page.

---

## Tab Management

### createTab()

Creates a new tab with empty navigation stacks.

### freeTab()

Releases memory associated with a tab.

### newTab()

Creates a new tab and inserts it into the browser's tab list.

### closeTab()

Closes the current tab (except the default tab).

### openTab(ID)

Switches to a specific tab.

### nextTab()

Moves to the next tab in the circular list.

### prevTab()

Moves to the previous tab.

---

## Navigation Operations

### visitPage()

Opens a new page in the current tab.

Actions performed:

1. current page is pushed into the **back stack**
2. **forward stack is cleared**
3. new page becomes the current page

---

### goBack()

Moves backward in the browsing history.

Steps:

1. current page → forward stack
2. pop page from back stack
3. popped page becomes current page

---

### goForward()

Moves forward in the browsing history.

Steps:

1. current page → back stack
2. pop page from forward stack
3. popped page becomes current page

---

# Additional Commands

### printOpenTabs()

Displays all open tabs and their current pages.

The active tab is marked with `*`.

---

### printHistory()

Displays browsing history for the current tab.

Order:

1. current page
2. pages stored in the back stack

---

# Project Structure

```
browser-history-system/
│
├── main.c
├── functii.h
├── Makefile
└── README.md
```

### main.c

Contains the full implementation of all data structures and browser logic.

### functii.h

Header file containing function declarations and structure definitions.

### Makefile

Used to compile the project automatically.

---

# Compilation

Compile the project using:

```bash
make
```

Run the program:

```bash
./browser
```

Clean compiled files:

```bash
make clean
```

---

# Concepts Demonstrated

This project demonstrates several important computer science concepts:

* Stack-based history management
* Circular doubly linked lists
* Sentinel nodes
* Dynamic memory allocation in C
* Modular program design
* Manual memory management
* Pointer manipulation

---

# Learning Objectives

Through this project I practiced:

* designing complex data structures in C
* implementing browser-like navigation logic
* managing memory safely
* building modular and maintainable code
* working with linked data structures

---

# Possible Future Improvements

* URL search functionality
* tab reordering
* persistent browsing history
* page caching
* interactive command-line interface
* unit tests

---

# Author

Developed as a **Data Structures and Algorithms project** focused on simulating browser navigation mechanics using fundamental data structures in C.

