# include "defines.h"
# include "utils.h"
# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

void	exit_usage(void);
void	host_val(char *host);

void	argparser(int argc, char **argv)
{
	if (argc < 2)
		exit_usage();

	for (int i = 0; i < argc; ++i)
	{
		if (ft_strcmp(argv[i], "--help") == 0)
			exit_usage();
	}

	host_val(argv[2]);
}

void	exit_usage(void)
{
	print_usage();
	exit(EXIT_SUCCESS);
}

void	host_val(char *host)
{
	(void) host;
	return;
}
