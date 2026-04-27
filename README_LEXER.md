# Lexer

Le lexer transforme une chaîne brute (la ligne tapée par l'utilisateur) en une liste chaînée de tokens. Chaque token représente une unité syntaxique : un mot, un opérateur, une redirection, etc.

---

## Structures de données

```c
// Tous les types de tokens possibles
typedef enum e_token_type
{
    TOK_WORD,         // un mot : "ls", "-la", "foo", "'bar'", etc.
    TOK_PIPE,         // |
    TOK_AND,          // &&  (bonus)
    TOK_OR,           // ||  (bonus)
    TOK_LPAREN,       // (   (bonus)
    TOK_RPAREN,       // )   (bonus)
    TOK_REDIR_IN,     // <
    TOK_REDIR_OUT,    // >
    TOK_REDIR_APPEND, // >>
    TOK_HEREDOC,      // <<
    TOK_WILDCARD,     // *
    TOK_BACKGROUND,   // &
    TOK_EOF           // fin de liste (sentinelle)
} t_token_type;

// Un nœud de la liste chaînée de tokens
typedef struct s_token
{
    t_token_type  type;   // le type du token
    char         *value;  // la valeur textuelle (NULL pour les opérateurs)
    int           quoted; // 1 si le mot contient des quotes
    struct s_token *next; // pointeur vers le token suivant
} t_token;
```

---

## Point d'entrée : `lexer.c`

```c
t_token *tokenize_input(t_gc *gc, char *input)
{
    t_token *token_list = NULL;
    t_token *new_tok    = NULL;

    while (*input)
    {
        skip_spaces(&input);         // ignore les espaces et tabulations
        if (*input == '\0')
            break;

        // 1. essaie de reconnaître un opérateur (|, &&, >>, etc.)
        new_tok = get_operator_token(gc, &input);

        // 2. si ce n'est pas un opérateur, c'est un mot
        if (!new_tok)
            new_tok = get_word_token(gc, &input);

        if (new_tok)
            add_token_back(&token_list, new_tok); // ajout en queue de liste
    }

    // termine toujours la liste par un token EOF (sentinelle)
    add_token_back(&token_list, create_token(gc, TOK_EOF, NULL, 0));
    return (token_list);
}
```

**Exemple** : `echo "hello" | cat -n`
```
TOK_WORD("echo") → TOK_WORD("\"hello\"",quoted=1) → TOK_PIPE → TOK_WORD("cat") → TOK_WORD("-n") → TOK_EOF
```

---

## Reconnaissance des opérateurs : `lexer_operator.c`

La reconnaissance est en deux passes pour toujours préférer l'opérateur **double** au simple :

```c
t_token *get_operator_token(t_gc *gc, char **input)
{
    t_token *tok = get_double_op(gc, input); // essaie d'abord ||, &&, <<, >>
    if (tok == NULL)
        tok = get_single_op(gc, input);      // puis |, &, <, >, (, ), *
    return (tok);
}
```

`get_double_op` :
```c
// Vérifie les deux premiers caractères ensemble
if ((*input)[0] == '|' && (*input)[1] == '|') { *input += 2; return TOK_OR; }
if ((*input)[0] == '&' && (*input)[1] == '&') { *input += 2; return TOK_AND; }
if ((*input)[0] == '<' && (*input)[1] == '<') { *input += 2; return TOK_HEREDOC; }
if ((*input)[0] == '>' && (*input)[1] == '>') { *input += 2; return TOK_REDIR_APPEND; }
```

`get_single_op` → `get_single_op_b` pour les cas restants :
```c
'|'  → TOK_PIPE
'&'  → TOK_BACKGROUND
'<'  → TOK_REDIR_IN
'>'  → TOK_REDIR_OUT
'('  → TOK_LPAREN
')'  → TOK_RPAREN
'*'  → TOK_WILDCARD
```

> Le pointeur `*input` est avancé de 1 ou 2 selon l'opérateur reconnu.

---

## Reconnaissance des mots : `lexer_words.c`

### Délimiteurs

```c
int is_delimiter(char c)
{
    // espace, tabulation, newline → séparent les tokens
    if (c == ' ' || c == '\t' || c == '\n') return (1);
    // opérateurs → terminent aussi un mot
    if (c == '|' || c == '<' || c == '>' || c == '&' || c == '(' || c == ')')
        return (1);
    return (0);
}
```

### Calcul de la longueur du mot

```c
int get_word_len(char *str)
{
    int i = 0, in_squote = 0, in_dquote = 0;

    while (str[i])
    {
        if (str[i] == '\'' && !in_dquote)
            in_squote = !in_squote;   // bascule le mode quote simple
        else if (str[i] == '"' && !in_squote)
            in_dquote = !in_dquote;   // bascule le mode quote double
        else if (!in_squote && !in_dquote && is_delimiter(str[i]))
            break;                    // stop au premier délimiteur hors quotes
        i++;
    }
    if (in_squote == 1 || in_dquote == 1)
        return (-1); // quote non fermée → erreur syntaxe
    return (i);
}
```

> Les quotes permettent d'inclure des espaces et opérateurs dans un mot : `"hello world"` → un seul token.

### Construction du token mot

```c
t_token *get_word_token(t_gc *gc, char **input)
{
    int len = get_word_len(*input);

    if (len == 0)  return (NULL);             // rien à lire
    if (len == -1) return (ms_error(...), NULL); // quote non fermée

    // détecte si le mot contient des quotes (pour l'expansion ultérieure)
    int quoted = 0;
    for (int i = 0; i < len; i++)
        if ((*input)[i] == '\'' || (*input)[i] == '"')
            quoted = 1;

    char *value = gc_alloc(gc, len + 1);
    ft_strlcpy(value, *input, len + 1); // copie exactement len caractères
    *input += len;                       // avance le curseur
    return create_token(gc, TOK_WORD, value, quoted);
}
```

---

## Utilitaires : `lexer_utils.c`

```c
// Alloue et initialise un token via le GC
t_token *create_token(t_gc *gc, t_token_type type, char *value, int quoted)
{
    t_token *node = gc_alloc(gc, sizeof(t_token));
    node->type   = type;
    node->value  = value;
    node->quoted = quoted;
    node->next   = NULL;
    return (node);
}

// Parcourt la liste jusqu'à la fin et chaîne le nouveau token
void add_token_back(t_token **head, t_token *new_node)
{
    if (*head == NULL) { *head = new_node; return; }
    t_token *temp = *head;
    while (temp->next) temp = temp->next;
    temp->next = new_node;
}

// Saute les espaces et tabulations en avançant le pointeur
void skip_spaces(char **input)
{
    while (**input == ' ' || **input == '\t')
        (*input)++;
}
```

---

## Schéma de flux

```
"echo hello | cat"
        │
        ▼
tokenize_input()
        │
        ├─ skip_spaces()
        ├─ get_operator_token()  → NULL (c'est un mot)
        ├─ get_word_token()      → TOK_WORD("echo")
        │
        ├─ skip_spaces()
        ├─ get_operator_token()  → NULL
        ├─ get_word_token()      → TOK_WORD("hello")
        │
        ├─ skip_spaces()
        ├─ get_operator_token()  → TOK_PIPE
        │
        ├─ skip_spaces()
        ├─ get_operator_token()  → NULL
        ├─ get_word_token()      → TOK_WORD("cat")
        │
        └─ TOK_EOF (sentinelle)

Résultat : [WORD("echo")] → [WORD("hello")] → [PIPE] → [WORD("cat")] → [EOF]
```
