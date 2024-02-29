/* pipe dup fork and exec Example */
/* runs as: ./pdfexec date wc */
/* Runs the shell equivalent of date | wc */
#include <unistd.h>
#include<stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
int main(int argc, char *argv[])
{
    int pipefd [2], status;
    pid_t cpid;
    pipe (pipefd);
    cpid = fork();
    if (cpid == 0) { /* left child (for left cmd of pipe) */
        //close(pipefd[0]); /* Close unused read end */
        dup2 (pipefd [1], STDOUT_FILENO); /* Make output go to pipe */
        execlp (argv[1], argv[1], (char *) NULL);
    }
    cpid = fork();
    if (cpid == 0) { /* right child (for right cmd of pipe */
        close (pipefd [1]); /* Close unused write end */
        dup2 (pipefd [0],STDIN_FILENO); /* Get input from pipe */
        execlp (argv[2], argv[2], (char*) NULL);
    }
    close (pipefd[0]); /* close pipes so EOF can work */
    close (pipefd[1]); /* This is a subtle but important step. The second child
    will not receive a EOF to trigger it to terminate while
    at least one other process (the parent) has the write
    end open */
    /* Parent reaps children exits */
    waitpid (-1, &status, 0);
    waitpid (-1, &status, 0);
}

int	main(int argc, char *argv[], char *envp[])
{
    int		i;
    int		pid;
    char	**argvcmd;
    char	*path;
    int		(*fd)[2];
    int		pip;

    if (argc < 4)
        return (ft_printf("arr in nb of args"));
    if (access(argv[1], F_OK) == -1 || access(argv[argc - 1], F_OK) == -1)
    {
        perror("error in io_files (io_file not exist)");
        return (1);
    }
    i = 2;
    fd = malloc((argc - 2) * sizeof(int[2]));
    while (i < argc - 1)
    {
        argvcmd = ft_split(argv[i], ' ');
        path = check_cmd_access(argvcmd[0], get_env_path(envp));
        if (path == NULL)
            return (1);
        pip = pipe(fd[i - 2]);
        if (pip == -1)
            perror("pipe error");
        pid = fork();
        if (pid == 0) {
            // for the first cmd
            if (i == 2 && argc >= 5)
            {
                dup2(fd[i - 2][1], STDOUT_FILENO);
                close(fd[i - 2][0]);
            }
            // for the last cmd
            else if (i == argc - 2 && argc >= 5){
                close(fd[i - 3][1]);
                dup2(fd[i - 3][0], STDIN_FILENO);
            }
            // for the middel cmds
            else if(i > 2)
            {
                close(fd[i - 3][1]);
                dup2(fd[i - 3][0], STDIN_FILENO);
                dup2(fd[i - 2][1], STDOUT_FILENO);
            }
            if (execve(path, argvcmd, NULL) == -1)
                perror("err in execve");
        }
        else {
            // Close unused pipe ends in the parent process
            if (i != 2)
                close(fd[i - 3][0]);
            if (i != argc - 2)
                close(fd[i - 2][1]);
        }
        
        i++;
        free(path);
        path = NULL;
        free_2d_arr(argvcmd);
        free(argvcmd);
        argvcmd = NULL;
    }
}