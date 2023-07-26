#include "shell.h"

/**
 * _error_putchar - writes the character c to stderr
 * @c: The character to print
 *
 * Return: On success 1.
 * On error, -1 is returned, and errno is set appropriately.
 */
int _error_putchar(char c)
{
	static int i;
	static char buf[WRITE_BUF_SIZE];

	if (c == BUF_FLUSH || i >= WRITE_BUF_SIZE)
	{
		write(2, buf, i); // Write the buffered characters to stderr
		i = 0; // Reset the buffer index
	}

	if (c != BUF_FLUSH)
		buf[i++] = c; // Add the character to the buffer

	return (1);
}

/**
 * isit_cmd - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int isit_cmd(info_t *info, char *path)
{
	struct stat st;

	(void)info;
	if (!path || stat(path, &st)) // Check if the file at 'path' exists
		return (0);

	if (st.st_mode & S_IFREG) // Check if it is a regular file
	{
		return (1); // Return 1 if it is an executable command
	}

	return (0); // Return 0 if it is not an executable command
}

/**
 * duplicate_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *duplicate_chars(char *pathstr, int start, int stop)
{
	static char buf[1024]; // Static buffer to hold the duplicated characters
	int i = 0, k = 0;

	for (k = 0, i = start; i < stop; i++)
		if (pathstr[i] != ':')
			buf[k++] = pathstr[i]; // Duplicate characters except ':'

	buf[k] = 0; // Null-terminate the duplicated characters
	return (buf); // Return the duplicated characters (note: this is risky due to static buffer)
}

/**
 * find_the_path - finds this cmd in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_the_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);

	if ((_string_length(cmd) > 2) && start_with(cmd, "./"))
	{
		if (isit_cmd(info, cmd))
			return (cmd);
	}

	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = duplicate_chars(pathstr, curr_pos, i); // Get a duplicated substring from 'pathstr'
			if (!*path)
				_string_cat(path, cmd); // If 'path' is empty, concatenate 'cmd' to it
			else
			{
				_string_cat(path, "/"); // Otherwise, add a '/' before 'cmd'
				_string_cat(path, cmd);
			}

			if (isit_cmd(info, path)) // Check if the resulting path points to an executable command
				return (path); // Return the full path if the command is found

			if (!pathstr[i])
				break;

			curr_pos = i + 1; // Move to the next position in 'pathstr' after ':'
		}
		i++;
	}

	return (NULL); // Return NULL if the command is not found in 'pathstr'
}

/**
 * _error_puts - prints an input string
 * @str: the string to be printed
 *
 * Return: Nothing
 */
void _error_puts(char *str)
{
	int i = 0;

	if (!str)
		return;

	while (str[i] != '\0')
	{
		_error_putchar(str[i]); // Print each character of the string using '_error_putchar'
		i++;
	}
}

