# include "defines.h"
# include "utils.h"
# include "args.h"
# include "traceroute.h"
# include <sys/types.h>
# include <unistd.h>

t_traceroute	g_data;

int	main(int argc, char **argv)
{
	if (getuid() !=  0)
		exit_failure("root privileges are required.");

	g_setdefs();

	arg_parser(argc, argv);

	traceroute_lifecycle();

	return 0;
}
