#include "shell.h"

/**
 * is_chain_delimiter - tests if the current char in the buffer is a chain delimiter
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in the buffer
 *
 * Return: 1 if it's a chain delimiter, 0 otherwise
 */
int is_chain_delimiter(info_t *info, char *buffer, size_t *position)
{
    size_t j = *position;

    if (buffer[j] == '|' && buffer[j + 1] == '|')
    {
        buffer[j] = 0;
        j++;
        info->command_buffer_type = CMD_OR;
    }
    else if (buffer[j] == '&' && buffer[j + 1] == '&')
    {
        buffer[j] = 0;
        j++;
        info->command_buffer_type = CMD_AND;
    }
    else if (buffer[j] == ';') /* found the end of this command */
    {
        buffer[j] = 0; /* replace semicolon with null */
        info->command_buffer_type = CMD_CHAIN;
    }
    else
        return (0);

    *position = j;
    return (1);
}

/**
 * chain_check - checks whether we should continue chaining based on the last status
 * @info: the parameter struct
 * @buffer: the char buffer
 * @position: address of the current position in the buffer
 * @start_position: starting position in the buffer
 * @length: length of the buffer
 *
 * Return: Void
 */
void chain_check(info_t *info, char *buffer, size_t *position, size_t start_position, size_t length)
{
    size_t j = *position;

    if (info->command_buffer_type == CMD_AND)
    {
        if (info->status)
        {
            buffer[start_position] = 0;
            j = length;
        }
    }

    if (info->command_buffer_type == CMD_OR)
    {
        if (!info->status)
        {
            buffer[start_position] = 0;
            j = length;
        }
    }

    *position = j;
}

/**
 * replace_alias - replaces an alias in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_alias(info_t *info)
{
    int i;
    list_t *node;
    char *p;

    for (i = 0; i < 10; i++)
    {
        node = find_node_start_with(info->alias, info->argv[0], '=');
        if (!node)
            return (0);
        free(info->argv[0]);
        p = find_char(node->str, '=');
        if (!p)
            return (0);
        p = duplicate_string(p + 1);
        if (!p)
            return (0);
        info->argv[0] = p;
    }
    return (1);
}

/**
 * replace_vars - replaces variables in the tokenized string
 * @info: the parameter struct
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_vars(info_t *info)
{
    int i = 0;
    list_t *node;

    for (i = 0; info->argv[i]; i++)
    {
        if (info->argv[i][0] != '$' || !info->argv[i][1])
            continue;

        if (!string_compare(info->argv[i], "$?"))
        {
            replace_str(&(info->argv[i]), duplicate_string(convert_number(info->status, 10, 0)));
            continue;
        }
        if (!string_compare(info->argv[i], "$$"))
        {
            replace_str(&(info->argv[i]), duplicate_string(convert_number(getpid(), 10, 0)));
            continue;
        }
        node = find_node_start_with(info->env, &info->argv[i][1], '=');
        if (node)
        {
            replace_str(&(info->argv[i]), duplicate_string(find_char(node->str, '=') + 1));
            continue;
        }
        replace_str(&info->argv[i], duplicate_string(""));
    }
    return (0);
}

/**
 * replace_str - replaces string
 * @old: address of the old string
 * @new: new string
 *
 * Return: 1 if replaced, 0 otherwise
 */
int replace_str(char **old, char *new)
{
    free(*old);
    *old = new;
    return (1);
}

