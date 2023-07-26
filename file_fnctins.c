#include "shell.h"

/**
 * get_historyfile - gets the history file
 * @info: parameter struct
 *
 * Return: allocated string containing the history file
 */
char *get_historyfile(info_t *info)
{
    char *buf, *dir;

    dir = _getenv_ironment(info, "HOME=");
    if (!dir)
        return (NULL);

    buf = malloc(sizeof(char) * (_string_length(dir) + _string_length(HIST_FILE) + 2));
    if (!buf)
        return (NULL);

    buf[0] = 0;
    _string_cpy(buf, dir);
    _string_cat(buf, "/");
    _string_cat(buf, HIST_FILE);
    return (buf);
}

/**
 * w_history - writes the history to a file
 * @info: the parameter struct
 *
 * Return: 1 on success, else -1
 */
int w_history(info_t *info)
{
    ssize_t fd;
    char *filename = get_historyfile(info);
    list_t *node = NULL;

    if (!filename)
        return (-1);

    fd = open(filename, O_CREAT | O_TRUNC | O_RDWR, 0644);
    free(filename);
    if (fd == -1)
        return (-1);

    for (node = info->history; node; node = node->next)
    {
        _puts_file_descriptor(node->str, fd);
        _put_file_descriptor('\n', fd);
    }
    _put_file_descriptor(BUF_FLUSH, fd);
    close(fd);
    return (1);
}

/**
 * r_history - reads history from a file
 * @info: the parameter struct
 *
 * Return: histcount on success, 0 otherwise
 */
int r_history(info_t *info)
{
    int i, last = 0, linecount = 0;
    ssize_t fd, rdlen, fsize = 0;
    struct stat st;
    char *buf = NULL, *filename = get_historyfile(info);

    if (!filename)
        return (0);

    fd = open(filename, O_RDONLY);
    free(filename);
    if (fd == -1)
        return (0);

    if (!fstat(fd, &st))
        fsize = st.st_size;

    if (fsize < 2)
        return (0);

    buf = malloc(sizeof(char) * (fsize + 1));
    if (!buf)
        return (0);

    rdlen = read(fd, buf, fsize);
    buf[fsize] = 0;

    if (rdlen <= 0)
        return (free(buf), 0);

    close(fd);

    for (i = 0; i < fsize; i++)
    {
        if (buf[i] == '\n')
        {
            buf[i] = 0;
            build_historylist(info, buf + last, linecount++);
            last = i + 1;
        }
    }

    if (last != i)
        build_historylist(info, buf + last, linecount++);

    free(buf);
    info->histcount = linecount;

    while (info->histcount-- >= HIST_MAX)
        del_nodeat_index(&(info->history), 0);

    renum_history(info);
    return (info->histcount);
}

/**
 * build_historylist - adds an entry to the history linked list
 * @info: Structure containing potential arguments. Used to maintain
 * @buf: buffer
 * @linecount: the history line count, histcount
 *
 * Return: Always 0
 */
int build_historylist(info_t *info, char *buf, int linecount)
{
    list_t *node = NULL;

    if (info->history)
        node = info->history;

    insert_node_at_end(&node, buf, linecount);

    if (!info->history)
        info->history = node;

    return (0);
}

/**
 * renum_history - renumbers the history linked list after changes
 * @info: Structure containing potential arguments. Used to maintain
 *
 * Return: the new histcount
 */
int renum_history(info_t *info)
{
    list_t *node = info->history;
    int i = 0;

    while (node)
    {
        node->num = i++;
        node = node->next;
    }

    return (info->histcount = i);
}

