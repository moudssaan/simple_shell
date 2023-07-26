#include "shell.h"

/**
 * _unsetenvironment - Remove an environment variable.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: The string representing the environment variable property.
 *
 * Return: 1 on delete, 0 otherwise.
 */
int _unsetenvironment(info_t *info, char *var)
{
    list_t *node = info->env;
    size_t i = 0;
    char *p;

    if (!node || !var)
        return (0);

    while (node)
    {
        p = start_with(node->str, var);
        if (p && *p == '=')
        {
            info->env_changed = del_nodeat_index(&(info->env), i);
            i = 0;
            node = info->env;
            continue;
        }
        node = node->next;
        i++;
    }
    return (info->env_changed);
}

/**
 * clear_the_info - Initializes the info_t struct.
 * @info: Pointer to the struct address.
 */
void clear_the_info(info_t *info)
{
    info->arg = NULL;
    info->argv = NULL;
    info->path = NULL;
    info->argc = 0;
}

/**
 * set_info_bf - Initializes the info_t struct with arguments.
 * @info: Pointer to the struct address.
 * @av: Argument vector.
 */
void set_info_bf(info_t *info, char **av)
{
    int i = 0;

    info->fname = av[0];
    if (info->arg)
    {
        info->argv = string_tow(info->arg, " \t");
        if (!info->argv)
        {
            info->argv = malloc(sizeof(char *) * 2);
            if (info->argv)
            {
                info->argv[0] = _string_dup(info->arg);
                info->argv[1] = NULL;
            }
        }
        for (i = 0; info->argv && info->argv[i]; i++)
            ;
        info->argc = i;

        rep_alias(info);
        rep_vars(info);
    }
}

/**
 * free_info_bf - Frees info_t struct fields.
 * @info: Pointer to the struct address.
 * @all: True if freeing all fields.
 */
void free_info_bf(info_t *info, int all)
{
    ffree(info->argv);
    info->argv = NULL;
    info->path = NULL;
    if (all)
    {
        if (!info->cmd_buf)
            free(info->arg);
        if (info->env)
            freelist_bf(&(info->env));
        if (info->history)
            freelist_bf(&(info->history));
        if (info->alias)
            freelist_bf(&(info->alias));
        ffree(info->environ);
        info->environ = NULL;
        buffer_free((void **)info->cmd_buf);
        if (info->readfd > 2)
            close(info->readfd);
        _putchar(BUF_FLUSH);
    }
}

/**
 * get_environment - Returns the string array copy of our environ.
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0.
 */
char **get_environment(info_t *info)
{
    if (!info->environ || info->env_changed)
    {
        info->environ = conv_listto_strings(info->env);
        info->env_changed = 0;
    }

    return (info->environ);
}

