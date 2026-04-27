# Garbage Collector (GC)

Le GC est le socle de tout le projet. Il gère la mémoire, les file descriptors ouverts et les processus fils — le tout dans une seule structure `t_gc` passée partout.

---

## Structure centrale : `t_gc`

```c
typedef struct s_gc
{
    t_arena arena;       // mémoire temporaire → libérée après chaque commande
    t_arena persistent;  // mémoire persistante → libérée seulement à la fin
    t_fd_tracker fds;    // liste des fd ouverts → fermés automatiquement
    t_pid_tracker pids;  // liste des PIDs enfants → attendus avec waitpid
    int exit_status;     // code de retour de la dernière commande
} t_gc;
```

Une `t_arena` est une liste chaînée de pointeurs :

```c
typedef struct s_arena_node
{
    void              *ptr;   // pointeur vers la mémoire allouée
    struct s_arena_node *next;
} t_arena_node;

typedef struct s_arena
{
    t_arena_node *head; // tête de la liste
} t_arena;
```

---

## Cycle de vie : `gc_lifecycle.c`

```c
void gc_init(t_gc *gc)
{
    gc->arena.head = NULL;       // arena temporaire vide
    gc->persistent.head = NULL;  // arena persistante vide

    // tableau de fds : capacité initiale de 8, grandit dynamiquement
    gc->fds.count = 0;
    gc->fds.capacity = 8;
    gc->fds.fds = malloc(gc->fds.capacity * sizeof(int));

    // tableau de pids : idem
    gc->pids.count = 0;
    gc->pids.capacity = 8;
    gc->pids.pids = malloc(gc->pids.capacity * sizeof(pid_t));

    gc->exit_status = 0;
}
```

> Appelé **une seule fois** au démarrage du shell.

```c
void gc_reset(t_gc *gc)
{
    free_arena(&gc->arena);  // libère toute la mémoire temporaire
    gc_close_fds(gc);        // ferme tous les fd trackés
    gc->pids.count = 0;      // remet le compteur de pids à 0 (déjà waitpid-és)
}
```

> Appelé **après chaque commande** pour repartir propre sans détruire les données persistantes (env, etc.).

```c
void gc_destroy(t_gc *gc)
{
    free_arena(&gc->arena);      // libère l'arena temporaire
    free_arena(&gc->persistent); // libère l'arena persistante
    gc_close_fds(gc);            // ferme les fd restants
    free(gc->fds.fds);           // libère le tableau de fd lui-même
    free(gc->pids.pids);         // libère le tableau de pids lui-même
}
```

> Appelé **à la fin du shell** ou en cas d'erreur fatale.

---

## Allocation mémoire : `gc_alloc.c`

```c
void *gc_alloc(t_gc *gc, size_t size)
{
    void         *ptr;
    t_arena_node *node;

    ptr = malloc(size);         // alloue la mémoire demandée
    node = malloc(sizeof(t_arena_node)); // alloue le nœud de tracking
    node->ptr = ptr;
    node->next = gc->arena.head; // insère en tête de liste (O(1))
    gc->arena.head = node;
    return (ptr);               // retourne le pointeur à l'appelant
}
```

> Chaque `gc_alloc` enregistre automatiquement le pointeur. Plus besoin de `free` manuel.

```c
void *gc_alloc_persistent(t_gc *gc, size_t size)
{
    // identique mais insère dans gc->persistent.head
    // → ne sera pas libéré par gc_reset(), seulement par gc_destroy()
}
```

```c
void free_arena(t_arena *arena)
{
    t_arena_node *node = arena->head;
    while (node != NULL)
    {
        t_arena_node *tmp = node->next;
        free(node->ptr);  // libère la mémoire trackée
        free(node);       // libère le nœud lui-même
        node = tmp;
    }
    arena->head = NULL;
}
```

---

## Tracking des file descriptors : `gc_fd.c`

```c
void gc_add_fd(t_gc *gc, int fd)
{
    // si le tableau est plein, on double sa capacité (realloc)
    if (gc->fds.count == gc->fds.capacity)
    {
        new_capacity = gc->fds.capacity * 2;
        gc->fds.fds = ft_realloc(...);
    }
    gc->fds.fds[gc->fds.count] = fd; // enregistre le fd
    gc->fds.count++;
}

void gc_close_fds(t_gc *gc)
{
    // ferme tous les fd enregistrés
    while (i < gc->fds.count)
        close(gc->fds.fds[i++]);
    gc->fds.count = 0; // remet le compteur à zéro
}
```

> Tout fd ouvert avec `open()` ou `pipe()` doit être passé à `gc_add_fd` pour éviter les fuites.

---

## Tracking des processus enfants : `gc_pids.c`

```c
void gc_add_pid(t_gc *gc, pid_t pid)
{
    // croissance dynamique identique à gc_add_fd
    gc->pids.pids[gc->pids.count] = pid;
    gc->pids.count++;
}

void gc_wait_pids(t_gc *gc)
{
    int status;
    while (i < gc->pids.count)
    {
        waitpid(gc->pids.pids[i], &status, 0); // attend chaque enfant
        // récupère le code de retour du DERNIER processus
        if (i == gc->pids.count - 1)
            gc->exit_status = WEXITSTATUS(status);
        i++;
    }
    gc->pids.count = 0; // vide la liste
}
```

> Après un `fork()`, on enregistre le PID avec `gc_add_pid`. Après la commande, `gc_wait_pids` attend tous les fils et stocke le dernier exit code dans `gc->exit_status`.

---

## Schéma de flux

```
gc_init()
    │
    ├─ boucle principale du shell
    │       ├─ gc_alloc()     → tokens, AST, argv…
    │       ├─ gc_add_pid()   → après chaque fork
    │       ├─ gc_add_fd()    → après chaque open/pipe
    │       ├─ gc_wait_pids() → attend les fils
    │       └─ gc_reset()     → nettoie pour la prochaine commande
    │
    └─ gc_destroy()           → fin du shell ou erreur fatale
```
