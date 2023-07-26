#include "shell.h"

/**
 * _setenvironment - Initialize a new environment variable,
 *                   or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 * @var: the string env var property
 * @value: the string env var value
 *
 * Return: Always 0
 */
int _setenvironment(info_t *info, char *var, char *value)
{
    char *buf = NULL;
    list_t *node;
    char *p;

    if (!var || !value)
        return (0);

    buf = malloc(_string_length(var) + _string_length(value) + 2);
    if (!buf)
        return (1);

    _string_cpy(buf, var);
    _string_cat(buf, "=");
    _string_cat(buf, value);

    node = info->env;
    while (node)
    {
        p = start_with(node->str, var);
        if (p && *p == '=')
        {
            free(node->str);
            node->str = buf;
            info->env_changed = 1;
            return (0);
        }
        node = node->next;
    }

    insert_node_at_end(&(info->env), buf, 0);
    free(buf);
    info->env_changed = 1;
    return (0);
}

/**
 * _my_help - A placeholder function for a help command.
 *            Currently, it only prints a message.
 *
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0
 */
int _my_help(info_t *info)
{
    char **arg_array;

    arg_array = info->argv;
    _puts("help call works. Function not yet implemented \n");
    
    /* Temp unused workaround to avoid unused variable warning */
    if (0)
        _puts(*arg_array);

    return (0);
}

