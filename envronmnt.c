#include "shell.h"

/**
 * _myenvironment - prints the current environment
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0
 */
int _myenvironment(info_t *info)
{
    show_list_string(info->env);
    return (0);
}

/**
 * _getenv_ironment - gets the value of an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *         constant function prototype.
 * @name: env var name
 *
 * Return: the value
 */
char *_getenv_ironment(info_t *info, const char *name)
{
    list_t *node = info->env;
    char *p;

    while (node)
    {
        p = start_with(node->str, name);
        if (p && *p)
            return (p);
        node = node->next;
    }
    return (NULL);
}

/**
 * _mysetenvironment - Initialize a new environment variable,
 *                     or modify an existing one
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0
 */
int _mysetenvironment(info_t *info)
{
    if (info->argc != 3)
    {
        _error_puts("Incorrect number of arguments\n");
        return (1);
    }
    if (_setenvironment(info, info->argv[1], info->argv[2]))
        return (0);
    return (1);
}

/**
 * _myunsetenvironmet - Remove an environment variable
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0
 */
int _myunsetenvironmet(info_t *info)
{
    int i;

    if (info->argc == 1)
    {
        _error_puts("Too few arguments.\n");
        return (1);
    }
    for (i = 1; i < info->argc; i++) // Fixed loop condition to avoid going out of bounds
        _unsetenvironment(info, info->argv[i]);

    return (0);
}

/**
 * populate_environment_list - populates the env linked list
 * @info: Structure containing potential arguments. Used to maintain
 *        constant function prototype.
 *
 * Return: Always 0
 */
int populate_environment_list(info_t *info)
{
    list_t *node = NULL;
    size_t i;

    for (i = 0; environ[i]; i++)
        insert_node_at_end(&node, environ[i], 0);
    info->env = node;
    return (0);
}

