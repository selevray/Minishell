#ifndef LEXER_H
# define LEXER_H

#endif // LEXER_H

typedef enum e_token_type
{
    TOK_WORD,
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
    TOK_BACKGROUND,
    TOK_EOF
} t_token_type;

typedef struct s_token
{
    t_token_type     type;
    char             *value;
    struct s_token   *next;
} t_token;