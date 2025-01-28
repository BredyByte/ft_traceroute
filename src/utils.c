# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>

void	exit_failure(char *str)
{
	if (!str)
		fprintf(stderr, "Fatal error\n");
	else
		fprintf(stderr, "%s\n", str);

	exit(EXIT_FAILURE);
}

void	print_usage(void)
{
	printf("Usage: \n");
	printf("\tft_tracerote:\thost (IPv4 10.3.1.14 or FQDM example.com)\n");
	printf("Arguments: \n");
	printf("+\thost\t\tThe host to traceroute to.\n");
	printf("\t\t\tIf --help is not used, it is assumed\n");
	printf("\t\t\tthat host is always the first argument passed.\n");
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i = 0;

	while (s1[i] == s2[i])
	{
		if (s1[i] == '\0' || s2[i] == '\0')
			break;

		++i;
	}

	if (s1[i] == '\0' && s2[i] == '\0')
		return 0;

	return (s1[i] - s2[i]);
}
