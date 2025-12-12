#include <stdlib.h>
#include <stdio.h>
# include <unistd.h>


int	main(int argc, char **argv, char **envp)
{
	pid_t	pid;
	int		status;
	char	*path;
	
	pid = fork();
	if (pid == -1)
		return (-1);
	if (pid == 0)
	{
		printf("I am child process and this is my status: %d\n", status);
		execve("bin/ls", argv, envp);
		printf("I am child process and this is my status after exec failed: %d\n", status);
	}
	waitpid(pid, &status, 0);
	printf("status: [%d]\n", status);
	return(0);
}