# Executor (EN COURS)

L'executor parcourt l'AST produit par le parser et exécute chaque nœud. Il gère les commandes simples, les pipes, les opérateurs logiques (`&&`, `||`) et les redirections.

> **Statut** : heredoc non implémenté, sous-shell non implémenté.

---

## Vue d'ensemble

```
AST
 │
 ▼
exec_ast()          ← dispatch selon le type de nœud
 ├─ NODE_CMD    → exec_simple_cmd()   fork + execve
 ├─ NODE_PIPE   → exec_pipe()         deux forks + pipe(2)
 ├─ NODE_AND    → exec_and()          exec gauche, puis droite si exit==0
 ├─ NODE_OR     → exec_or()           exec gauche, puis droite si exit!=0
 └─ NODE_SUBSHELL → (TODO)
```

---

## Dispatch principal : `exec_ast.c`

```c
void exec_ast(t_gc *gc, t_ast *ast, char **envp)
{
    if (ast == NULL)
        return;

    if (ast->type == NODE_AND)
        exec_and(gc, ast, envp);
    else if (ast->type == NODE_OR)
        exec_or(gc, ast, envp);
    else if (ast->type == NODE_CMD)
        exec_simple_cmd(gc, ast, envp);
    else if (ast->type == NODE_PIPE)
        exec_pipe(gc, ast, envp);
    else if (ast->type == NODE_SUBSHELL)
        return; // TODO
}
```

### Commande simple

```c
static void exec_simple_cmd(t_gc *gc, t_ast *ast, char **envp)
{
    pid_t pid = fork();

    if (pid == 0)
    {
        // === ENFANT ===
        char *path = get_path_cmd(gc, ast->cmd->argv[0], envp); // résout le chemin
        exec_redir(gc, ast->cmd->redirections);                  // applique les redirections
        execve(path, ast->cmd->argv, envp);                      // remplace le processus
        ms_fatal(gc, "execve", "failed execve", 1);              // si execve échoue
    }
    else if (pid > 0)
    {
        // === PARENT ===
        gc_add_pid(gc, pid);   // enregistre le PID
        gc_wait_pids(gc);      // attend la fin du fils
    }
}
```

### Opérateurs logiques

```c
// && : exécute droite SEULEMENT si gauche a réussi (exit_status == 0)
static void exec_and(t_gc *gc, t_ast *ast, char **envp)
{
    exec_ast(gc, ast->left, envp);
    if (gc->exit_status == 0)
        exec_ast(gc, ast->right, envp);
}

// || : exécute droite SEULEMENT si gauche a échoué (exit_status != 0)
static void exec_or(t_gc *gc, t_ast *ast, char **envp)
{
    exec_ast(gc, ast->left, envp);
    if (gc->exit_status != 0)
        exec_ast(gc, ast->right, envp);
}
```

---

## Pipes : `exec_pipe.c`

Un pipe nécessite **deux processus fils** et un descripteur de fichier partagé.

```
  [fd[1] = write end]          [fd[0] = read end]
  FILS GAUCHE (commande gauche)  →  FILS DROIT (commande droite)
      stdout → fd[1]                  stdin ← fd[0]
```

```c
void exec_pipe(t_gc *gc, t_ast *ast, char **envp)
{
    int   fd[2];
    pid_t pid;

    pipe(fd); // crée la paire de fd : fd[0]=lecture, fd[1]=écriture

    pid = fork();
    if (pid == 0)
    {
        // === FILS GAUCHE ===
        dup2(fd[1], STDOUT_FILENO); // redirige stdout vers le pipe
        close(fd[1]);
        close(fd[0]);               // le fils gauche n'a pas besoin de lire
        exec_ast(gc, ast->left, envp);
        exit(gc->exit_status);
    }
    else if (pid > 0)
    {
        // === PARENT ===
        exec_pipe_right(gc, ast, envp, fd); // lance le fils droit
        gc_add_pid(gc, pid);                // enregistre le fils gauche
        gc_wait_pids(gc);                   // attend les deux fils
    }
}

void exec_pipe_right(t_gc *gc, t_ast *ast, char **envp, int *fd)
{
    pid_t pid2 = fork();

    if (pid2 == 0)
    {
        // === FILS DROIT ===
        dup2(fd[0], STDIN_FILENO); // redirige stdin depuis le pipe
        close(fd[1]);
        close(fd[0]);
        exec_ast(gc, ast->right, envp);
        exit(gc->exit_status);
    }
    else if (pid2 > 0)
    {
        gc_add_pid(gc, pid2); // enregistre le fils droit
        close(fd[0]);          // le parent ferme les deux extrémités
        close(fd[1]);
    }
}
```

---

## Recherche de commande : `exec_cmd.c`

```c
char *get_path_cmd(t_gc *gc, char *cmd, char **envp)
{
    // si la commande contient '/' → chemin absolu ou relatif, on vérifie directement
    if (ft_strchr(cmd, '/'))
    {
        if (access(cmd, X_OK) == 0)
            return cmd;
        return (NULL);
    }

    // sinon on cherche dans chaque répertoire du PATH
    char *path_env = find_path_env(envp); // extrait la valeur de PATH
    if (!path_env)
        path_env = "/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin"; // fallback

    char **paths = gc_split(gc, path_env, ':'); // découpe PATH en tableau
    return find_valid_path(gc, paths, cmd);      // cherche le premier exécutable
}
```

```c
// Extrait "PATH=..." de envp → retourne le pointeur après "PATH="
static char *find_path_env(char **envp)
{
    for (int i = 0; envp[i]; i++)
        if (ft_strncmp(envp[i], "PATH=", 5) == 0)
            return (envp[i] + 5);
    return (NULL);
}

// Construit "/usr/bin/ls" à partir de "/usr/bin" et "ls"
static char *build_path(t_gc *gc, char *dir, char *cmd)
{
    char *full_path = gc_alloc(gc, ft_strlen(dir) + ft_strlen(cmd) + 2);
    ft_strcpy(dir, full_path);
    ft_strcat(full_path, "/");
    ft_strcat(full_path, cmd);
    return (full_path);
}

// Parcourt les répertoires et retourne le premier chemin exécutable
static char *find_valid_path(t_gc *gc, char **paths, char *cmd)
{
    for (int i = 0; paths[i]; i++)
    {
        char *full_path = build_path(gc, paths[i], cmd);
        if (access(full_path, X_OK) == 0) // vérifie si le fichier est exécutable
            return (full_path);
    }
    return (NULL);
}
```

---

## Redirections : `exec_redir.c`

Appliquées **dans le processus fils**, avant `execve`.

```c
int exec_redir(t_gc *gc, t_redir *redir)
{
    while (redir != NULL) // parcourt la liste chaînée de redirections
    {
        if (redir->type == REDIR_APPEND)   apply_redir_append(gc, redir);
        else if (redir->type == REDIR_IN)  apply_redir_in(gc, redir);
        else if (redir->type == REDIR_OUT) apply_redir_out(gc, redir);
        else if (redir->type == REDIR_HEREDOC) apply_redir_heredoc(gc, redir); // TODO
        redir = redir->next;
    }
    return (0);
}
```

```c
// >  : ouvre en écriture, crée si absent, ÉCRASE si existant
static int apply_redir_out(t_gc *gc, t_redir *redir)
{
    int fd = open(redir->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    dup2(fd, STDOUT_FILENO); // remplace stdout par le fichier
    close(fd);               // fd original plus nécessaire
    return (0);
}

// >> : ouvre en écriture, crée si absent, AJOUTE à la fin
static int apply_redir_append(t_gc *gc, t_redir *redir)
{
    int fd = open(redir->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
    dup2(fd, STDOUT_FILENO);
    close(fd);
    return (0);
}

// <  : ouvre en lecture, remplace stdin
static int apply_redir_in(t_gc *gc, t_redir *redir)
{
    int fd = open(redir->target, O_RDONLY, 0644);
    dup2(fd, STDIN_FILENO); // remplace stdin par le fichier
    close(fd);
    return (0);
}

// << : heredoc — TODO
static int apply_redir_heredoc(t_gc *gc, t_redir *redir)
{
    (void)gc;
    (void)redir;
    return (-1); // non implémenté
}
```

---

## Ce qui reste à faire

| Fonctionnalité | Fichier | Statut |
|---|---|---|
| Heredoc (`<<`) | `exec_redir.c` | TODO |
| Sous-shell `(...)` | `exec_ast.c` | TODO |
| Builtins (`cd`, `echo`, `export`…) | à créer | TODO |
| Expansion des variables `$VAR` | à créer | TODO |
| Wildcard `*` | à créer | TODO |
