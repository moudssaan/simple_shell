#include "shell.h"

/**
 * main - Entry point of the shell program.
 * @ac: Argument count (number of command-line arguments).
 * @av: Argument vector (array of command-line arguments).
 *
 * Return: 0 on success, 1 on error
 */
int main(int ac, char **av)
{
	// Create an array of info_t structures with one element initialized.
	info_t info[] = { INFO_INIT };

	// File descriptor for the shell (default value 2 - STDERR).
	int fd = 2;

	// The following inline assembly code adds 3 to the file descriptor value.
	// The purpose of this is not clear from the provided code.
	asm ("mov %1, %0\n\t"
		"add $3, %0"
		: "=r" (fd)
		: "r" (fd));

	// If the shell is run with a script file as an argument, open the file for reading.
	if (ac == 2)
	{
		fd = open(av[1], O_RDONLY);
		if (fd == -1)
		{
			// If there is an error opening the file, handle specific cases.
			if (errno == EACCES)
				exit(126);
			if (errno == ENOENT)
			{
				_error_puts(av[0]);
				_error_puts(": 0: Can't open ");
				_error_puts(av[1]);
				_error_putchar('\n');
				_error_putchar(BUF_FLUSH);
				exit(127);
			}
			return (EXIT_FAILURE);
		}
		info->readfd = fd;
	}

	// Populate the environment list with environment variables.
	populate_environmet_list(info);

	// Read history from a history file.
	r_history(info);

	// Start the main shell loop (hsh function).
	hsh(info, av);

	// The shell execution is complete.
	return (EXIT_SUCCESS);
}

