#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define READ_BUF_SIZE 1024
#define WRITE_BUF_SIZE 1024
#define BUF_FLUSH -1

/* for command chaining */
#define CMD_NORM    0
#define CMD_OR      1
#define CMD_AND     2
#define CMD_CHAIN   3

/* for conv_number() */
#define CONVERT_LOWERCASE   1
#define CONVERT_UNSIGNED    2

/* 1 if using system getline() */
#define USE_GETLINE 0
#define USE_STRTOK  0

#define HIST_FILE   ".simple_shell_history"
#define HIST_MAX    4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} list_t;

/**
 *struct passinfo - contains pseudo-arguments to pass into a function,
 *                  allowing a uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguments
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    list_t *env;
    list_t *history;
    list_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
    int cmd_buf_type; /* CMD_type ||, &&, ; */
    int readfd;
    int histcount;
} info_t;

#define INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
    char *type;
    int (*func)(info_t *);
} builtin_table;

/* hsh.c */
int hsh(info_t *, char **);
int find_builtin_em(info_t *);
void find_the_cmd(info_t *);
void fork_the_cmd(info_t *);

/* path.c */
int isit_cmd(info_t *, char *);
char *duplicate_chars(char *, int, int);
char *find_the_path(info_t *, char *, char *);

/* loophsh.c */
int loophsh(char **);

/* err_string */
void _error_puts(char *);
int _error_putchar(char);
int _put_file_d(char c, int fd);
int _puts_file_descriptor(char *str, int fd);

/* string_functions.c */
int _string_length(char *);
int _string_compare(char *, char *);
char *start_with(const char *, const char *);
char *_string_cat(char *, char *);
char *_string_cpy(char *, char *);
char *_string_dup(const char *);
void _puts(char *);
int _putchar(char);
char *_string_n_cpy(char *, char *, int);
char *_string_n_cat(char *, char *, int);
char *_string_chr(char *, char);
char **string_tow(char *, char *);
char **string_tow2(char *, char);

/* m_functions */
char *_memory_set(char *, char, unsigned int);
void ffree(char **);
void *_re_allocation(void *, unsigned int, unsigned int);
int buffer_free(void **);
int interactive(info_t *);
int isit_delim(char, char *);
int _isit_alpha(int);
int _conv_atoi(char *);
int _error_atoi(char *);
void print_the_error(info_t *, char *);
int print_d(int, int);
char *conv_number(long int, int, int);
void rem_comments(char *);

/* builtin emulators */
int _my_exit(info_t *);
int _my_cd(info_t *);
int _my_help(info_t *);
int _my_history(info_t *);
int _my_alias(info_t *);

/* getline */
ssize_t get_input(info_t *);
int _getline(info_t *, char **, size_t *);
void sigintHandler(int);

/* information */
void clear_the_info(info_t *);
void set_info_bf(info_t *, char **);
void free_info_bf(info_t *, int);

/* environment */
char *_getenv_ironment(info_t *, const char *);
int _myenvironment(info_t *);
int _mysetenvironment(info_t *);
int _myunsetenvironmet(info_t *);
int populate_environmet_list(info_t *);
char **get_environment(info_t *);
int _unsetenvironment(info_t *, char *);
int _setenvironment(info_t *, char *, char *);

/* file io functions */
char *get_historyfile(info_t *info);
int w_history(info_t *info);
int r_history(info_t *info);
int build_historylist(info_t *info, char *buf, int linecount);
int renum_history(info_t *info);

/* list string */
list_t *insert_node(list_t **, const char *, int);
list_t *insert_node_at_end(list_t **, const char *, int);
size_t show_list_string(const list_t *);
int del_nodeat_index(list_t **, unsigned int);
void freelist_bf(list_t **);
size_t list_length(const list_t *);
char **conv_listto_strings(list_t *);
size_t show_list(const list_t *);
list_t *nodestart_with(list_t *, char *, char);
ssize_t get_index_node(list_t *, list_t *);

/* chaining */
int isit_chain(info_t *, char *, size_t *);
void chain_check(info_t *, char *, size_t *, size_t, size_t);
int rep_alias(info_t *);
int rep_vars(info_t *);
int rep_str(char **, char *);

#endif

