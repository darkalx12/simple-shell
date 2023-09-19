#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>

#define BUFSIZE 1024
#define TOK_BUFSIZE 128
#define TOK_DELIM " \t\r\n\a"

/* Points to an array of pointers to strings called the "environment" */
extern char **environ;


/**
 * struct data - struct that contains all relevant data on runtime
 * @av: argument vector
 * @input: command line written by the user
 * @args: tokens of the command line
 * @status: last status of the shell
 * @counter: lines counter
 * @_environ: environment variable
 * @pid: process ID of the shell
 */
typedef struct data
{
	char **av;
	char *input;
	char **args;
	int status;
	int counter;
	char **_environ;
	char *pid;
} data_shell;

/**
 * struct sep_list_s - single linked list
 * @separator: ; | &
 * @next: next node
 * Description: single linked list to store separators
 */
typedef struct sep_list_s
{
	char separator;
	struct sep_list_s *next;
} sep_list;

/**
 * struct line_list_s - single linked list
 * @line: command line
 * @next: next node
 * Description: single linked list to store command lines
 */
typedef struct line_list_s
{
	char *line;
	struct line_list_s *next;
} line_list;

/**
 * struct r_var_list - single linked list
 * @len_var: length of the variable
 * @val: value of the variable
 * @len_val: length of the value
 * @next: next node
 * Description: single linked list to store variables
 */
typedef struct r_var_list
{
	int len_var;
	char *val;
	int len_val;
	struct r_var_list *next;
} r_var;

/**
 * struct builtin_s - Builtin struct for command args.
 * @name: The name of the command builtin i.e cd, exit, env
 * @f: data type pointer function.
 */
typedef struct builtin_s
{
	char *name;
	int (*f)(data_shell *datash);
} builtin_t;

/* aux_lists.c */
sep_list *add_sep_node_end(sep_list **head, char sep);
void free_list(sep_list **head);
line_list *add_line_node_end(line_list **head, char *line);
void free_line(line_list **head);

/* aux_lists2.c */
r_var *add_end(r_var **head, int lvar, char *var, int lval);
void free_rival(r_var **head);

/* aux_str functions */
char *_strcat(char *pest, const char *src);
char *_strcpy(char *pest, char *src);
int _strcmp(char *str, char *str2);
char *_strchr(char *s, char c);
int _strspn(char *s, char *accepted);

void _memorycpy(void *newptr, const void *ptr, unsigned int size);
void *_real(void *ptr, unsigned int old_size, unsigned int new_size);
char **_realcdp(char **ptr, unsigned int old_size, unsigned int new_size);

/* aux_str2.c */
char *_strdup(const char *s);
int _strlen(const char *s);
int cmp_chars(char str[], const char *sep);
char *_strtok(char str[], const char *sep);
int _isdigit(const char *s);

/* aux_str3.c */
void reverse_string(char *s);

/* check_syntax_error.c */
int repeat_char(char *input, int i);
int error_op(char *input, int i, char last);
int first_char(char *input, int *i);
void print_error(data_shell *datash, char *input, int i, int bool);
int check_error(data_shell *datash, char *input);

/* shell_loop.c */
char *without_comment(char *in);
void shell_loop(data_shell *datash);

/* read_line.c */
char *read_line(int *i_eof);

/* split.c */
char *swap_char(char *input, int bool);
void add_nodes(sep_list **head_s, line_list **head_l, char *input);
void go_next(sep_list **list_s, line_list **list_l, data_shell *datash);
int split_commands(data_shell *datash, char *input);
char **split_line(char *input);

/* rep_var.c */
void check_env(r_var **h, char *in, data_shell *data);
int check_vars(r_var **h, char *in, char *st, data_shell *data);
char *replaced_input(r_var **head, char *input, char *new_input, int nlen);
char *rep_var(char *input, data_shell *datash);

/* get_line.c */
void take_line(char **lineptr, size_t *n, char *buffer, size_t j);
ssize_t get_line(char **lineptr, size_t *n, FILE *stream);

/* exec_line */
int execute_line(data_shell *datash);

/* cmd_exec.c */
int is_cdir(char *path, int *i);
char *_rich(char *cmd, char **_environ);
int is_executable(data_shell *datash);
int check_error_comand(char *dir, data_shell *datash);
int comand_exec(data_shell *datash);

/* env1.c */
char *_getenv(const char *name, char **_environ);
int _env(data_shell *datash);

/* env2.c */
char *copy_in(char *name, char *value);
void set_env(char *name, char *value, data_shell *datash);
int _setenv(data_shell *datash);
int _unsetenv(data_shell *datash);

/* cd.c */
void change_back(data_shell *datash);
void change_to(data_shell *datash);
void change_previous(data_shell *datash);
void change_home(data_shell *datash);

/* cd_shell.c */
int change_shell(data_shell *datash);

/* get_builtin */
int (*get_builtin(char *cmd))(data_shell *datash);

/* _exit.c */
int cancel_shell(data_shell *datash);

/* aux_stdlib.c */
int get_len(int n);
char *aux_itoa(int n);
int _atoi(char *s);

/* aux_error1.c */
char *strget_cd(data_shell *, char *, char *, char *);
char *error_cd(data_shell *datash);
char *error_null_found(data_shell *datash);
char *error_ext_shell(data_shell *datash);

/* aux_error2.c */
char *error_get_alias(char **args);
char *error_bro(data_shell *datash);
char *error_syntax(char **args);
char *error_permission(char **args);
char *error_way_755(data_shell *datash);


/* get_error.c */
int catch_error(data_shell *datash, int eval);

/* get_sigint.c */
void sigint(int sig);

/* aux_help.c */
void aux_helping_folder(void);
void aux_helping_setfolder(void);
void aux_helping_unsetfolder(void);
void aux_helping_general(void);
void aux_helping_exit(void);

/* aux_help2.c */
void auth_helping(void);
void auth_helping_aliass(void);
void auth_helping_container(void);

/* get_help.c */
int get_help(data_shell *datash);

#endif /*END SHELL_H*/
