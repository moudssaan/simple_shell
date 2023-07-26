#include "shell.h"

/**
 * input_buf - Buffers chained commands from user input
 * @info: Pointer to the parameter struct
 * @buf: Address of the buffer
 * @len: Address of the len variable
 *
 * Return: Bytes read from the input
 */
ssize_t input_buf(info_t *info, char **buf, size_t *len)
{
    ssize_t r = 0;
    size_t len_p = 0;

    if (!*len) /* If nothing left in the buffer, fill it */
    {
        /*buffer_free((void **)info->cmd_buf);*/
        free(*buf);
        *buf = NULL;
        signal(SIGINT, sigintHandler);
#if USE_GETLINE
        r = getline(buf, &len_p, stdin);
#else
        r = _getline(info, buf, &len_p);
#endif
        if (r > 0)
        {
            if ((*buf)[r - 1] == '\n')
            {
                (*buf)[r - 1] = '\0'; /* Remove trailing newline */
                r--;
            }
            info->linecount_flag = 1;
            rem_comments(*buf);
            build_historylist(info, *buf, info->histcount++);
            /* TODO: Check if it's a command chain */
            {
                *len = r;
                info->cmd_buf = buf;
            }
        }
    }
    return (r);
}

/**
 * get_input - Gets a line of input from the user
 * @info: Pointer to the parameter struct
 *
 * Return: Bytes read from the input
 */
ssize_t get_input(info_t *info)
{
    static char *buf; /* The ';' command chain buffer */
    static size_t i, j, len;
    ssize_t r = 0;
    char **buf_p = &(info->arg), *p;

    _putchar(BUF_FLUSH);
    r = input_buf(info, &buf, &len);
    if (r == -1) /* EOF */
        return (-1);
    if (len)    /* If there are commands left in the chain buffer */
    {
        j = i; /* Initialize new iterator to current buf position */
        p = buf + i; /* Get pointer for return */

        chain_check(info, buf, &j, i, len);
        while (j < len) /* Iterate to semicolon or end */
        {
            if (isit_chain(info, buf, &j))
                break;
            j++;
        }

        i = j + 1; /* Increment past nulled ';'' */
        if (i >= len) /* Reached the end of buffer? */
        {
            i = len = 0; /* Reset position and length */
            info->cmd_buf_type = CMD_NORM;
        }

        *buf_p = p; /* Pass back pointer to current command position */
        return (_string_length(p)); /* Return length of current command */
    }

    *buf_p = buf; /* Else not a chain, pass back buffer from _getline() */
    return (r); /* Return length of buffer from _getline() */
}

/**
 * read_buf - Reads data into a buffer
 * @info: Pointer to the parameter struct
 * @buf: Buffer to read data into
 * @i: Size of the buffer
 *
 * Return: Number of bytes read
 */
ssize_t read_buf(info_t *info, char *buf, size_t *i)
{
    ssize_t r = 0;

    if (*i)
        return (0);
    r = read(info->readfd, buf, READ_BUF_SIZE);
    if (r >= 0)
        *i = r;
    return (r);
}

/**
 * _getline - Gets the next line of input from STDIN
 * @info: Pointer to the parameter struct
 * @ptr: Address of pointer to buffer, preallocated or NULL
 * @length: Size of preallocated ptr buffer if not NULL
 *
 * Return: Number of bytes read from the input
 */
int _getline(info_t *info, char **ptr, size_t *length)
{
    static char buf[READ_BUF_SIZE];
    static size_t i, len;
    size_t k;
    ssize_t r = 0, s = 0;
    char *p = NULL, *new_p = NULL, *c;

    p = *ptr;
    if (p && length)
        s = *length;
    if (i == len)
        i = len = 0;

    r = read_buf(info, buf, &len);
    if (r == -1 || (r == 0 && len == 0))
        return (-1);

    c = _string_chr(buf + i, '\n');
    k = c ? 1 + (unsigned int)(c - buf) : len;
    new_p = _re_allocation(p, s, s ? s + k : k + 1);
    if (!new_p) /* MALLOC FAILURE! */
        return (p ? free(p), -1 : -1);

    if (s)
        _string_n_cat(new_p, buf + i, k - i);
    else
        _string_n_cpy(new_p, buf + i, k - i + 1);

    s += k - i;
    i = k;
    p = new_p;

    if (length)
        *length = s;
    *ptr = p;
    return (s);
}

/**
 * sigintHandler - Blocks SIGINT (Ctrl-C) signal
 * @sig_num: The signal number
 *
 * Return: void
 */
void sigintHandler(__attribute__((unused))int sig_num)
{
    _puts("\n");
    _puts("$ ");
    _putchar(BUF_FLUSH);
}

