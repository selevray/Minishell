/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: selevray <selevray@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/05 14:00:00 by selevray          #+#    #+#             */
/*   Updated: 2026/04/27 09:44:20 by selevray         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFT_H
# define LIBFT_H

# include <stdarg.h>
# include <stddef.h>
# include <stdlib.h>
# include <unistd.h>

int		ft_atoi(const char *str);
void	ft_bzero(void *s, size_t n);
void	*ft_calloc(size_t nmemb, size_t size);
int		ft_isalnum(int c);
int		ft_isalpha(int c);
int		ft_isascii(int c);
int		ft_isdigit(int c);
int		ft_isprint(int c);
char	*ft_itoa(int n);
void	*ft_memchr(const void *memoryBlock, int searchedChar, size_t size);
int		ft_memcmp(const void *pointer1, const void *pointer2, size_t size);
void	*ft_memcpy(void *destination, const void *source, size_t size);
void	*ft_memmove(void *destination, const void *source, size_t size);
void	*ft_memset(void *s, int c, size_t n);
void	*memdup(const void *mem, size_t size);
int		ft_print_hex(unsigned long n, int format);
int		ft_print_ptr(unsigned long n);
int		ft_print_unsigned(unsigned int n);
int		ft_printf(const char *format, ...);
void	ft_putchar(char c);
void	ft_putchar_fd(char c, int fd);
void	ft_putendl_fd(char *s, int fd);
void	ft_putnbr_fd(int n, int fd);
int		ft_print_char(char c);
int		ft_putstr(char *string);
void	*ft_realloc(void *ptr, size_t old_size, size_t new_size);
void	ft_putstr_fd(char *s, int fd);
ssize_t	ft_safe_write(int fd, const void *buf, size_t count);
char	**ft_split(char const *str, char charset);
char	*ft_strchr(const char *string, int searchedChar);
void	ft_strcpy(char *dest, char *src);
char	*ft_strdup(const char *s);
void	ft_striteri(char *s, void (*f)(unsigned int, char *));
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_strcat(char *dest, char const *src);
size_t	ft_strlcat(char *dst, const char *src, size_t size);
size_t	ft_strlcpy(char *dest, const char *src, size_t size);
size_t	ft_strlen(const char *str);
char	*ft_strmapi(char const *s, char (*f)(unsigned int, char));
int		ft_strncmp(const char *s1, const char *s2, size_t n);
int		ft_strcmp(char *s1, char *s2);
char	*ft_strnstr(const char *big, const char *little, size_t len);
char	*ft_strrchr(const char *string, int searchedChar);
char	*ft_strtrim(char const *s1, char const *set);
char	*ft_substr(char const *s, unsigned int start, size_t len);
int		ft_tolower(int character);
int		ft_toupper(int character);
char	*ft_strndup(const char *s, int n);

#endif
