# Utils

Ce dossier contient les fonctions utilitaires partagées par tout le projet : gestion des erreurs et split avec GC.

---

## Gestion des erreurs : `error.c`

Deux niveaux d'erreur sont disponibles, à choisir selon la gravité.

### `ms_error` — erreur récupérable

```c
void ms_error(t_gc *gc, char *context, char *msg, int status)
{
    // écrit sur stderr (fd 2) : "minishell: <context>: <msg>\n"
    write(2, "minishell: ", 11);
    write(2, context, ft_strlen(context));
    write(2, ": ", 2);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);

    gc->exit_status = status; // met à jour le code de retour
    // NE quitte PAS le shell → on continue la boucle principale
}
```

**Utilisation typique** : erreur de syntaxe dans la ligne de commande.

```c
// Exemple dans lexer_words.c :
return (ms_error(gc, "syntax error",
        "unexpected EOF while looking for matching quote", 2), NULL);
// → affiche "minishell: syntax error: unexpected EOF while looking for matching quote"
// → exit_status = 2, retourne NULL pour indiquer l'échec
```

### `ms_fatal` — erreur fatale

```c
void ms_fatal(t_gc *gc, char *context, char *msg, int status)
{
    // même affichage que ms_error
    write(2, "minishell: ", 11);
    write(2, context, ft_strlen(context));
    write(2, ": ", 2);
    write(2, msg, ft_strlen(msg));
    write(2, "\n", 1);

    gc_destroy(gc);      // libère TOUTE la mémoire et ferme les fd
    exit(status % 256);  // quitte le processus (% 256 pour rester dans 0-255)
}
```

**Utilisation typique** : échec de `malloc`, `fork`, ou `execve` — des situations dont on ne peut pas se remettre.

```c
// Exemple dans exec_ast.c :
if (pid == -1)
    ms_fatal(gc, "fork", "fork failed", 1);
```

### Différence clé

| Fonction | Quitte ? | Quand l'utiliser |
|---|---|---|
| `ms_error` | Non | Erreur utilisateur (syntaxe, fichier introuvable…) |
| `ms_fatal` | Oui | Erreur système (malloc, fork, execve…) |

---

## Split avec GC : `gc_split.c`

Version de `ft_split` qui alloue via le GC au lieu de `malloc` direct. Découpe une chaîne sur un caractère séparateur.

```c
char **gc_split(t_gc *gc, char const *str, char charset)
{
    int    words  = count_words(str, charset); // compte le nombre de segments
    char **result = gc_alloc(gc, sizeof(char *) * (words + 1)); // +1 pour NULL final
    return fill_split(gc, str, charset, result);
}
```

### Comptage des mots

```c
static int count_words(char const *s, char c)
{
    int count = 0, in_word = 0;

    while (*s)
    {
        if (*s != c && !in_word)
        {
            in_word = 1; // on entre dans un mot
            count++;
        }
        else if (*s == c)
            in_word = 0; // on sort du mot
        s++;
    }
    return (count);
}
```

**Exemple** : `"/usr/bin:/usr/local/bin::/"` avec `':'`
- entre dans mot → `/usr/bin` → count=1
- délimiteur `:` → hors mot
- entre dans mot → `/usr/local/bin` → count=2
- délimiteur `:` → hors mot
- délimiteur `:` → hors mot (double `:` ignoré)
- entre dans mot → `/` → count=3
- **résultat** : 3

### Extraction d'un mot

```c
static char *extract_word(t_gc *gc, char const *s, char c)
{
    int len = 0;
    while (s[len] && s[len] != c) // mesure jusqu'au prochain séparateur
        len++;
    char *word = gc_alloc(gc, len + 1);
    for (int i = 0; i < len; i++)
        word[i] = s[i];
    word[len] = '\0';
    return (word);
}
```

### Remplissage du tableau

```c
static char **fill_split(t_gc *gc, char const *s, char c, char **result)
{
    int i = 0, words = count_words(s, c);

    while (i < words)
    {
        while (*s == c) s++;          // saute les séparateurs consécutifs
        result[i] = extract_word(gc, s, c); // extrait le mot courant
        while (*s && *s != c) s++;    // avance jusqu'au prochain séparateur
        i++;
    }
    result[i] = NULL; // termine le tableau
    return (result);
}
```

### Utilisation concrète dans le projet

```c
// Dans exec_cmd.c — découpe le PATH en tableau de répertoires
char **paths = gc_split(gc, path_env, ':');
// "/usr/local/bin:/usr/bin:/bin" → ["usr/local/bin", "/usr/bin", "/bin", NULL]

// On peut ensuite itérer pour trouver l'exécutable :
for (int i = 0; paths[i]; i++)
    if (access(build_path(gc, paths[i], cmd), X_OK) == 0)
        return full_path;
```

> Pas besoin de `free` sur le résultat : tout est alloué via `gc_alloc` et sera libéré par `gc_reset()` ou `gc_destroy()`.
