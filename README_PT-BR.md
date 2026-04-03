# 🗑️ Garbage Collector do Lucão

Um garbage collector leve, baseado em lista encadeada, para programas em C. Criado para eliminar chamadas manuais de `free()`, evitar vazamentos de memória e tornar o gerenciamento de memória transparente.  
*Especialmente útil para estudantes da 42.*

---

## Sumário

- [Que Problema Ele Resolve?](#que-problema-ele-resolve)
- [Como Funciona](#como-funciona)
- [Estrutura do Projeto](#estrutura-do-projeto)
- [A API — Quatro Funções](#a-api--quatro-funções)
  - [gc\_malloc](#gc_malloc)
  - [gc\_add](#gc_add)
  - [gc\_free](#gc_free)
  - [gc\_clear](#gc_clear)
- [Como as Funções São Construídas](#como-as-funções-são-construídas)
- [Integração com a libft](#integração-com-a-libft)
- [Início Rápido](#início-rápido)
- [Exemplo Completo de Uso](#exemplo-completo-de-uso)
- [Padrões Comuns](#padrões-comuns)
- [Bugs Conhecidos e Limitações](#bugs-conhecidos-e-limitações)

---

## Que Problema Ele Resolve?

Em C, cada `malloc()` precisa ter um `free()` correspondente. Em programas complexos, especialmente aqueles com muitos retornos antecipados, caminhos de erro ou funções recursivas, é fácil vazar memória ou dar um double-free em um ponteiro. Este garbage collector resolve isso rastreando cada alocação em uma lista encadeada.  
Quando terminar, basta uma chamada a `gc_clear()` para liberar tudo de uma vez.

```c
// Sem o GC — você precisa rastrear e liberar cada ponteiro manualmente
char *a = malloc(10);
char *b = malloc(20);
char *c = malloc(30);
// ... se um erro ocorrer aqui, a e b vazam memória
free(a);
free(b);
free(c);

// Com o GC — uma única chamada resolve tudo
char *a = gc_malloc(&gc, 10);
char *b = gc_malloc(&gc, 20);
char *c = gc_malloc(&gc, 30);
// ... em caso de erro ou ao sair:
gc_clear(&gc); // libera a, b e c — e os próprios nós da lista
```

---

## Como Funciona

O coletor é uma **lista encadeada simples** onde cada nó armazena um ponteiro retornado por `malloc`. Toda vez que você aloca memória via `gc_malloc`, um novo nó é inserido no início da lista. A lista é armazenada como `t_gc **` (ponteiro para ponteiro), então qualquer modificação na cabeça é refletida em todo lugar.

```
collector ──► [ nó3 | ptr=C | next ] ──► [ nó2 | ptr=B | next ] ──► [ nó1 | ptr=A | next=NULL ]
```

Quando `gc_clear` é chamado, ele percorre toda a lista, libera cada ponteiro armazenado, libera o nó da lista em si e define a cabeça como `NULL`.

---

## Estrutura do Projeto

```
.
├── garbage_colector.h       # Header público — único arquivo que você precisa incluir
├── garbage_colector.c       # Implementação das quatro funções do GC
├── main.c                   # Demo standalone (sem dependência da libft)
└── libft/
    ├── libft.h              # Header da libft (inclui garbage_colector.h)
    ├── garbage_colector.h   # Cópia do header do GC dentro da libft
    ├── garbage_colector.c   # Cópia da implementação do GC dentro da libft
    ├── get_next_line.c      # GNL adaptado para usar o GC
    ├── get_next_line_bonus.c
    ├── ft_strdup.c          # Funções da libft adaptadas para usar o GC
    ├── ft_substr.c
    ├── ft_strjoin.c
    ├── ft_strtrim.c
    ├── ft_split.c
    ├── ft_calloc.c
    ├── ft_itoa.c
    ├── ft_strmapi.c
    └── ...                  # Todos os outros arquivos padrão da libft
```

---

## A API — Quatro Funções

Inclua o header e declare um `t_gc *` inicializado com `NULL` no início de qualquer escopo onde você queira alocação rastreada.

```c
#include "garbage_colector.h"

t_gc *gc = NULL; // Sempre inicialize com NULL
```

---

### `gc_malloc`

```c
void *gc_malloc(t_gc **gc, size_t size);
```

Um substituto direto do `malloc`. Aloca `size` bytes, registra o ponteiro no coletor e o retorna. Retorna `NULL` em caso de falha.

```c
int *numeros = (int *)gc_malloc(&gc, 10 * sizeof(int));
char *str    = (char *)gc_malloc(&gc, 64);
```

---

### `gc_add`

```c
void gc_add(t_gc **gc, void *ptr);
```

Registra um ponteiro **já alocado** no coletor. Use isso se você alocou memória com um `malloc` simples (por exemplo, de uma biblioteca de terceiros) e ainda quer que o GC gerencie seu tempo de vida.

```c
void *externo = malloc(256); // malloc simples
gc_add(&gc, externo);        // agora o GC é dono dele
```

> **Nota:** se o próprio `gc_add` falhar ao alocar seu nó de lista, ele imediatamente libera `ptr` para evitar um vazamento silencioso.

---

### `gc_free`

```c
void gc_free(t_gc **gc, void *ptr);
```

Libera um único ponteiro específico e o remove da lista do coletor. Equivalente a um `free()` rastreado. Seguro de chamar mesmo que `ptr` não esteja na lista (simplesmente não faz nada).

```c
char *tmp = (char *)gc_malloc(&gc, 128);
// ... usa tmp ...
gc_free(&gc, tmp); // libera tmp agora, antes do gc_clear
```

---

### `gc_clear`

```c
void gc_clear(t_gc **gc);
```

Libera **todos** os ponteiros rastreados e todos os nós da lista, depois define a cabeça do coletor como `NULL`. Chame isso ao sair do programa, em caso de erro, ou ao final de qualquer escopo onde você usou o GC.

```c
gc_clear(&gc); // libera todas as alocações restantes
```

---

## Como as Funções São Construídas

### A struct `t_gc`

```c
typedef struct s_gc
{
    void        *ptr;   // O ponteiro alocado sendo rastreado
    struct s_gc *next;  // Próximo nó na lista
}   t_gc;
```

Cada nó possui exatamente um ponteiro. A lista cresce por **inserção no início** (novos nós vão para a frente), então a inserção é O(1).

---

### `gc_malloc` — funcionamento interno

```c
void *gc_malloc(t_gc **gc, size_t size)
{
    void *ptr = malloc(size);
    if (!ptr)
        return (NULL);
    if (gc_add(gc, ptr))   // registra o novo ponteiro
      return (NULL);
    return (ptr);
}
```

A alocação e o registro são dois passos separados. Isso significa que você também pode registrar ponteiros externos diretamente com `gc_add`.

---

### `gc_add` — funcionamento interno

```c
int gc_add(t_gc **gc, void *ptr)
{
    t_gc *new_node;

    if (!ptr)
        return ;
    new_node = malloc(sizeof(t_gc));
    if (!new_node)
    {
        free(ptr);   // se não conseguimos rastrear, não vazamos
        return (1);
    }
    new_node->ptr  = ptr;
    new_node->next = *gc;  // insere no início da lista
    *gc = new_node;
    return (0);
}
```

Observe o **padrão de inserção no início**: `new_node->next = *gc; *gc = new_node;`. Isso garante inserção O(1) e faz com que o ponteiro alocado mais recentemente esteja sempre na cabeça da lista.

---

### `gc_free` — funcionamento interno

```c
void gc_free(t_gc **gc, void *ptr)
{
    t_gc *current = *gc;
    t_gc *prev    = NULL;

    while (current)
    {
        if (current->ptr == ptr)       // encontrou o nó
        {
            if (prev)
                prev->next = current->next;  // desvincula
            else
                *gc = current->next;         // era a cabeça
            free(current->ptr);
            free(current);
            return ;
        }
        prev    = current;
        current = current->next;
    }
}
```

Esta é uma **remoção padrão em lista encadeada** por valor. O ponteiro `prev` rastreia o nó anterior para que possamos desvincular sem precisar de uma lista duplamente encadeada.

---

### `gc_clear` — funcionamento interno

```c
void gc_clear(t_gc **gc)
{
    t_gc *current;
    t_gc *next;

    if (!gc || !*gc)
        return (1);
    current = *gc;
    while (current)
    {
        next = current->next;
        if (current->ptr)
            free(current->ptr);  // libera a alocação rastreada
        free(current);           // libera o próprio nó da lista
        current = next;
    }
    *gc = NULL;  // reinicia o coletor
}
```

Duas coisas são liberadas por iteração: o **ponteiro de carga** e o **nó da lista** que o rastreava. Após o loop, `*gc = NULL` deixa o coletor pronto para reutilização.

---

## Integração com a libft

A libft dentro deste repositório foi adaptada para que todas as funções que normalmente alocam memória agora aceitem um parâmetro `t_gc **collector` em vez de usar `malloc` diretamente. Isso significa que toda alocação feita pelas funções da libft é rastreada automaticamente.

| Assinatura original | Assinatura adaptada |
|---|---|
| `char *ft_strdup(const char *s)` | `char *ft_strdup(const char *s, t_gc **collector)` |
| `char *ft_substr(char *s, unsigned int start, size_t len)` | `char *ft_substr(char *s, unsigned int start, size_t len, t_gc **collector)` |
| `char *ft_strjoin(char const *s1, char const *s2)` | `char *ft_strjoin(char const *s1, char const *s2, t_gc **collector)` |
| `char *ft_strtrim(const char *s1, const char *set)` | `char *ft_strtrim(const char *s1, const char *set, t_gc **collector)` |
| `char **ft_split(char const *s, char c)` | `char **ft_split(char const *s, char c, t_gc **collector)` |
| `char *ft_itoa(int n)` | `char *ft_itoa(int n, t_gc **collector)` |
| `void *ft_calloc(size_t nmemb, size_t size)` | `void *ft_calloc(size_t nmemb, size_t size, t_gc **collector)` |
| `char *get_next_line(int fd)` | `char *get_next_line(int fd, t_gc **collector)` |

Passe o mesmo `collector` que você inicializa no topo do seu programa e `gc_clear` vai limpar tudo de uma vez.

---

## Início Rápido

**1. Copie os dois arquivos para dentro do seu projeto:**

```
garbage_colector.h
garbage_colector.c
```

**2. Inclua o header:**

```c
#include "garbage_colector.h"
```

**3. Inicialize o coletor e use:**

```c
int main(void)
{
    t_gc *gc = NULL;  // deve ser NULL, não não-inicializado

    char *buffer = (char *)gc_malloc(&gc, 1024);
    int  *array  = (int *)gc_malloc(&gc, 100 * sizeof(int));

    // ... lógica do seu programa ...

    gc_clear(&gc);  // libera buffer, array e todos os nós da lista
    return (0);
}
```

**4. Compile:**

```bash
cc -Wall -Wextra -Werror main.c garbage_colector.c -o meu_programa
```

---

## Exemplo Completo de Uso

Este é o `main.c` na raiz do repositório, demonstrando as quatro funções:

```c
#include "garbage_colector.h"
#include <stdio.h>

int main(void)
{
    t_gc *collector = NULL;  // <-- sempre inicialize com NULL
    int  n;
    int  *vector;

    printf("Quantos números? ");
    scanf("%d", &n);

    // gc_malloc: funciona como malloc, mas rastreado
    vector = (int *)gc_malloc(&collector, n * sizeof(int));
    if (!vector)
        return (printf("Falha na alocação.\n"), 1);

    for (int i = 0; i < n; i++)
    {
        printf("Insira o número %d: ", i + 1);
        scanf("%d", &vector[i]);
    }

    printf("Números: ");
    for (int i = 0; i < n; i++)
        printf("%d ", vector[i]);
    printf("\n");

    // gc_free: libera um único ponteiro rastreado antecipadamente
    gc_free(&collector, vector);

    // gc_clear: libera tudo que ainda está rastreado (nós, outras alocações)
    gc_clear(&collector);
    return (0);
}
```

---

## Padrões Comuns

**Saída por erro — libera tudo sem precisar rastrear o que falhou:**

```c
t_gc *gc = NULL;

char *a = (char *)gc_malloc(&gc, 64);
char *b = (char *)gc_malloc(&gc, 128);
if (!alguma_condicao)
{
    gc_clear(&gc);  // libera a e b independente de qual falhou
    return (NULL);
}
```

**Alocação temporária dentro de um loop:**

```c
t_gc *gc = NULL;

while (condicao)
{
    char *tmp = (char *)gc_malloc(&gc, 256);
    // ... usa tmp ...
    gc_free(&gc, tmp);  // libera imediatamente, mantém o coletor enxuto
}
gc_clear(&gc);
```

**Usando com a libft adaptada:**

```c
t_gc *gc = NULL;

char **tokens = ft_split("hello world foo", ' ', &gc);
char *joined  = ft_strjoin(tokens[0], tokens[1], &gc);
char *result  = ft_itoa(ft_atoi(joined) + 1, &gc);

printf("%s\n", result);
gc_clear(&gc);  // libera tokens, joined, result e todas as alocações internas
```
---