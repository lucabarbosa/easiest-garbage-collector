# 🗑️ Garbage Collector in C Language

A lightweight, linked-list-based garbage collector for C programs. Built to eliminate manual `free()` calls, avoid memory leaks, and make memory management transparent.  
*This is specially useful for 42 students.*

---

## Table of Contents

- [What Problem Does This Solve?](#what-problem-does-this-solve)
- [How It Works](#how-it-works)
- [Project Structure](#project-structure)
- [The API — Four Functions](#the-api--four-functions)
  - [gc\_malloc](#gc_malloc)
  - [gc\_add](#gc_add)
  - [gc\_free](#gc_free)
  - [gc\_clear](#gc_clear)
- [How the Functions Are Built](#how-the-functions-are-built)
- [Integration with libft](#integration-with-libft)
- [Quick Start](#quick-start)
- [Full Usage Example](#full-usage-example)
- [Common Patterns](#common-patterns)
- [Known Bugs & Limitations](#known-bugs--limitations)

---

## What Problem Does This Solve?

In C, every `malloc()` must have a matching `free()`. In complex programs, especially those with many early returns, error paths, or recursive functions. It is easy to leak memory or double-free a pointer. This garbage collector solves that by tracking every allocation in a linked list.  
When you're done, you can make one call to `gc_clear()` and frees everything at once.

```c
// Without the GC — you must track and free every pointer manually
char *a = malloc(10);
char *b = malloc(20);
char *c = malloc(30);
// ... if an error happens here, a and b leak memory
free(a);
free(b);
free(c);

// With the GC — one call handles everything
char *a = gc_malloc(&gc, 10);
char *b = gc_malloc(&gc, 20);
char *c = gc_malloc(&gc, 30);
// ... on error or on exit:
gc_clear(&gc); // frees a, b, and c — and the list nodes themselves
```

---
## How It Works

The collector is a **singly linked list** where each node holds one pointer returned by `malloc`. Every time you allocate memory through `gc_malloc`, a new node is prepended to the list. It's something like  The list is stored as a `t_gc **` (pointer to pointer), so modifications to the head are reflected everywhere.

```
collector ──► [ node3 | ptr=C | next ] ──► [ node2 | ptr=B | next ] ──► [ node1 | ptr=A | next=NULL ]
```

When `gc_clear` is called, it walks the entire list, frees each stored pointer, frees the list node itself, and sets the head to `NULL`.

---

## Project Structure

```
.
├── garbage_colector.h       # Public header — the only file you need to include
├── garbage_colector.c       # Implementation of the four GC functions
├── main.c                   # Standalone demo (no libft dependency)
└── libft/
    ├── libft.h              # libft header (includes garbage_colector.h)
    ├── garbage_colector.h   # GC header copy inside libft
    ├── garbage_colector.c   # GC implementation copy inside libft
    ├── get_next_line.c      # GNL adapted to use GC
    ├── get_next_line_bonus.c
    ├── ft_strdup.c          # libft functions adapted to use GC
    ├── ft_substr.c
    ├── ft_strjoin.c
    ├── ft_strtrim.c
    ├── ft_split.c
    ├── ft_calloc.c
    ├── ft_itoa.c
    ├── ft_strmapi.c
    └── ...                  # All other standard libft files
```

---

## The API — Four Functions

Include the header and declare a `t_gc *` initialized to `NULL` at the start of any scope where you want tracked allocation.

```c
#include "garbage_colector.h"

t_gc *gc = NULL; // Always initialize to NULL
```

---

### `gc_malloc`

```c
void *gc_malloc(t_gc **gc, size_t size);
```

A drop-in replacement for `malloc`. Allocates `size` bytes, registers the pointer in the collector, and returns it. Returns `NULL` on failure.

```c
int *numbers = (int *)gc_malloc(&gc, 10 * sizeof(int));
char *str    = (char *)gc_malloc(&gc, 64);
```

---

### `gc_add`

```c
void gc_add(t_gc **gc, void *ptr);
```

Registers an **already-allocated** pointer into the collector. Use this if you allocated memory with a plain `malloc` (e.g., from a third-party library) and still want the GC to manage its lifetime.

```c
void *external = malloc(256); // plain malloc
gc_add(&gc, external);        // now the GC owns it
```

> **Note:** if `gc_add` itself fails to allocate its list node, it immediately frees `ptr` to avoid a silent leak.

---

### `gc_free`

```c
void gc_free(t_gc **gc, void *ptr);
```

Frees a single specific pointer and removes it from the collector's list. Equivalent to a tracked `free()`. Safe to call even if `ptr` is not in the list (it simply does nothing).

```c
char *tmp = (char *)gc_malloc(&gc, 128);
// ... use tmp ...
gc_free(&gc, tmp); // frees tmp now, before gc_clear
```

---

### `gc_clear`

```c
void gc_clear(t_gc **gc);
```

Frees **every** tracked pointer and every list node, then sets the collector head to `NULL`. Call this at program exit, on error, or at the end of any scope where you used the GC.

```c
gc_clear(&gc); // frees all remaining allocations
```

---

## How the Functions Are Built

### The `t_gc` struct

```c
typedef struct s_gc
{
    void        *ptr;   // The allocated pointer being tracked
    struct s_gc *next;  // Next node in the list
}   t_gc;
```

Each node owns exactly one pointer. The list grows by **prepending** (new nodes go to the front), so insertion is O(1).

---

### `gc_malloc` — internals

```c
void *gc_malloc(t_gc **gc, size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        return (NULL);
   if (gc_add(gc, ptr))   // register the new pointer
        return (NULL);
    return (ptr);
}
```

The allocation and the registration are two separate steps. This means you can also register external pointers with `gc_add` directly.

---

### `gc_add` — internals

```c
int gc_add(t_gc **gc, void *ptr)
{
    t_gc *new_node;

    if (!ptr)
        return (1);
    new_node = malloc(sizeof(t_gc));
    if (!new_node)
    {
        free(ptr);   // if we can't track it, don't leak it
        return (1);
    }
    new_node->ptr  = ptr;
    new_node->next = *gc;  // prepend to the list
    *gc = new_node;
    return (0);
}
```

Notice the **prepend pattern**: `new_node->next = *gc; *gc = new_node;`. This gives O(1) insertion and means the most recently allocated pointer is always at the head of the list.

---

### `gc_free` — internals

```c
void gc_free(t_gc **gc, void *ptr)
{
    t_gc *current = *gc;
    t_gc *prev    = NULL;

    while (current)
    {
        if (current->ptr == ptr)       // found the node
        {
            if (prev)
                prev->next = current->next;  // unlink
            else
                *gc = current->next;         // was the head
            free(current->ptr);
            free(current);
            return ;
        }
        prev    = current;
        current = current->next;
    }
}
```

This is a standard **linked list deletion** by value. The `prev` pointer tracks the previous node so we can unlink without a doubly-linked list.

---

### `gc_clear` — internals

```c
void gc_clear(t_gc **gc)
{
    t_gc *current;
    t_gc *next;

    if (!gc || !*gc)
        return ;
    current = *gc;
    while (current)
    {
        next = current->next;
        if (current->ptr)
            free(current->ptr);  // free the tracked allocation
        free(current);           // free the list node itself
        current = next;
    }
    *gc = NULL;  // reset the collector
}
```

Two things are freed per iteration: the **payload pointer** and the **list node** that was tracking it. After the loop, `*gc = NULL` makes the collector ready for reuse.

---

## Integration with libft

The libft inside this repo has been adapted so that all functions which normally allocate memory now accept a `t_gc **collector` parameter instead of using plain `malloc`. This means every allocation made by libft functions is automatically tracked.

| Original signature | Adapted signature |
|---|---|
| `char *ft_strdup(const char *s)` | `char *ft_strdup(const char *s, t_gc **collector)` |
| `char *ft_substr(char *s, unsigned int start, size_t len)` | `char *ft_substr(char *s, unsigned int start, size_t len, t_gc **collector)` |
| `char *ft_strjoin(char const *s1, char const *s2)` | `char *ft_strjoin(char const *s1, char const *s2, t_gc **collector)` |
| `char *ft_strtrim(const char *s1, const char *set)` | `char *ft_strtrim(const char *s1, const char *set, t_gc **collector)` |
| `char **ft_split(char const *s, char c)` | `char **ft_split(char const *s, char c, t_gc **collector)` |
| `char *ft_itoa(int n)` | `char *ft_itoa(int n, t_gc **collector)` |
| `void *ft_calloc(size_t nmemb, size_t size)` | `void *ft_calloc(size_t nmemb, size_t size, t_gc **collector)` |
| `char *get_next_line(int fd)` | `char *get_next_line(int fd, t_gc **collector)` |

Pass the same `collector` you initialize at the top of your program, and `gc_clear` will clean up everything at once.

---

## Quick Start

**1. Copy the two files into your project:**

```
garbage_colector.h
garbage_colector.c
```

**2. Include the header:**

```c
#include "garbage_colector.h"
```

**3. Initialize the collector and use it:**

```c
int main(void)
{
    t_gc *gc = NULL;  // must be NULL, not uninitialized

    char *buffer = (char *)gc_malloc(&gc, 1024);
    int  *array  = (int *)gc_malloc(&gc, 100 * sizeof(int));

    // ... your program logic ...

    gc_clear(&gc);  // frees buffer, array, and all list nodes
    return (0);
}
```

**4. Compile:**

```bash
cc -Wall -Wextra -Werror main.c garbage_colector.c -o my_program
```

---

## Full Usage Example

This is the `main.c` at the root of the repository, demonstrating all four functions:

```c
#include "garbage_colector.h"
#include <stdio.h>

int main(void)
{
    t_gc *collector = NULL;  // <-- always initialize to NULL
    int  n;
    int  *vector;

    printf("How many numbers? ");
    scanf("%d", &n);

    // gc_malloc: behaves like malloc, but tracked
    vector = (int *)gc_malloc(&collector, n * sizeof(int));
    if (!vector)
        return (printf("Allocation failed.\n"), 1);

    for (int i = 0; i < n; i++)
    {
        printf("Insert number %d: ", i + 1);
        scanf("%d", &vector[i]);
    }

    printf("Numbers: ");
    for (int i = 0; i < n; i++)
        printf("%d ", vector[i]);
    printf("\n");

    // gc_free: free a single tracked pointer early
    gc_free(&collector, vector);

    // gc_clear: free everything still tracked (list nodes, other allocs)
    gc_clear(&collector);
    return (0);
}
```

---

## Common Patterns

**Error exit — free everything without tracking what failed:**

```c
t_gc *gc = NULL;

char *a = (char *)gc_malloc(&gc, 64);
char *b = (char *)gc_malloc(&gc, 128);
if (!some_condition)
{
    gc_clear(&gc);  // frees a and b regardless of which one failed
    return (NULL);
}
```

**Temporary allocation inside a loop:**

```c
t_gc *gc = NULL;

while (condition)
{
    char *tmp = (char *)gc_malloc(&gc, 256);
    // ... use tmp ...
    gc_free(&gc, tmp);  // release it immediately, keep the collector lean
}
gc_clear(&gc);
```

**Using with the adapted libft:**

```c
t_gc *gc = NULL;

char **tokens = ft_split("hello world foo", ' ', &gc);
char *joined  = ft_strjoin(tokens[0], tokens[1], &gc);
char *result  = ft_itoa(ft_atoi(joined) + 1, &gc);

printf("%s\n", result);
gc_clear(&gc);  // frees tokens, joined, result, and all internal allocs
```
---