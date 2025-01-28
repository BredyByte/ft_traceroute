# include "defines.h"
# include "utils.h"
# include "args.h"
# include <sys/types.h>
# include <unistd.h>

int	main(int argc, char **argv)
{
	if (getuid() !=  0)
		exit_failure("root privileges are required.");

	argparser(argc, argv);

	return 0;
}
