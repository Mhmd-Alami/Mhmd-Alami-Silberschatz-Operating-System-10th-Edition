#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_LINE 80
#define READ_END 0
#define WRITE_END 1
#define BUFFER_SIZE 10000
int main(void)
{
    char *args[MAX_LINE / 2 + 1];
    int should_run = 1;
    char mark[] = "No recent command!";
    while (should_run)
    {

        printf("osh>");
        fflush(stdout);
        char command[MAX_LINE] = "quit";
        char *cc = fgets(command, MAX_LINE, stdin);
        
        for (int i = 0; i < strlen(command); i++)
        {
            if (command[i] == '\n')
            {
                command[i] = '\0';
                break;
            }
        }

        int end = 0;
        for (int i = 0; i < strlen(command); i++)
        {
            if (command[i] == '&')
            {
                command[i] = '\0';
                end = i;
                break;
            }
        }

        float equal = strcmp(command, "quit");

        if ((strcmp(command, "!!") == 0) && (strcmp(mark, "No recent command!") != 0))
        {
            strcpy(command, mark);
        }
        if (equal == 0)
        {
            should_run = 0;
            break;
        }
        else
        {
            strcpy(mark, command);
            char *arr[MAX_LINE] = {NULL};
            char **ptr = arr;
            int i = 0;
            ptr[0] = strtok(command, " ");
            while (ptr[i] != NULL)
            {
                i++;
                ptr[i] = strtok(NULL, " ");
            }
            int search_char[] = {90, 90, 90};

            int m = i - 1;
            while (m >= 0)
            {
                if (strcmp(ptr[m], "|") == 0)
                {
                    search_char[0] = m;
                }
                m--;
            }
            m = i - 1;
            while (m >= 0)
            {
                if (strcmp(ptr[m], ">") == 0)
                {
                    search_char[1] = m;
                }
                m--;
            }
            m = i - 1;
            while (m >= 0)
            {
                if (strcmp(ptr[m], "<") == 0)
                {
                    search_char[2] = m;
                }
                m--;
            }

            int min = 2;
            if (search_char[1] < search_char[2])
            {
                min = 1;
            }

            pid_t pid;
            pid = fork();
            int out = 0;
            if ((min == 1) && (search_char[1] != 90))
            {
                out = 1;
            }

            if (pid < 0)
            {
                fprintf(stderr, "Fork Failed\n");
                return 1;
            }
            else if (pid == 0)
            {
                if (search_char[0] != 90)
                {
                    char *ar[MAX_LINE] = {NULL};
                    char **p = ar;
                    int k;
                    for (int y = search_char[0] + 1; y <= i - 1; y++)
                    {
                        if (ptr[y] != NULL)
                        {
                            p[k] = ptr[y];
                            k++;
                        }
                    }
                    p[k] = NULL;

                    for (int z = search_char[0]; z <= i - 1; z++)
                    {
                        ptr[z] = NULL;
                    }
                    char read_msg[BUFFER_SIZE];
                    int fdl[2];

                    pid_t pid1;
                    if (pipe(fdl) == -1)
                    {
                        fprintf(stderr, "Pipe failed\n");
                        return 1;
                    }

                    pid1 = fork();

                    if (pid1 < 0)
                    {
                        fprintf(stderr, "Fork Failed\n");
                        return 1;
                    }
                    else if (pid1 == 0)
                    {
                        close(fdl[READ_END]);
                        dup2(fdl[WRITE_END], STDOUT_FILENO);
                        execvp(ptr[0], ptr);
                        close(fdl[WRITE_END]);
                    }
                    else
                    {
                        close(fdl[WRITE_END]);
                        wait(NULL);
                        read(fdl[READ_END], read_msg, BUFFER_SIZE - 1);
                        char read_msg2[MAX_LINE] = "";

                        for (int rml = 0; rml <= strlen(read_msg); rml++)
                        {
                            read_msg2[rml] = read_msg[rml];
                        }

                        char *args2[] = {NULL};

                        for (int a = 0; a < k; a++)
                        {
                            args2[a] = p[a];
                        }

                        args2[k] = NULL;
                        int fdl2[2];

                        if (pipe(fdl2) == -1)
                        {
                            fprintf(stderr, "Pipe failed");
                            return 1;
                        }

                        int pipefd[2];
                        pipe(pipefd);
                        write(pipefd[1], read_msg2, strlen(read_msg2));
                        close(pipefd[1]);
                        dup2(pipefd[0], STDIN_FILENO);
                        execvp(args2[0], args2);
                        perror("execvp failed");
                        exit(1);
                    }
                }
                else if (out == 1)
                {
                    char file_name[MAX_LINE];
                    strcpy(file_name, ptr[search_char[1] + 1]);

                    for (int j = search_char[1]; j <= i - 1; j++)
                    {
                        ptr[j] = '\0';
                    }
                    int fd;
                    fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);

                    if (fd < 0)
                    {
                        perror("Error opening file");
                        exit(1);
                    }

                    dup2(fd, STDOUT_FILENO);
                    close(fd);
                }
                else if (search_char[2] != 90)
                {
                    char filein[MAX_LINE];
                    strcpy(filein, ptr[search_char[2] + 1]);

                    for (int k = search_char[2]; k <= i - 1; k++)
                    {
                        ptr[k] = NULL;
                    }

                    int fdi = open(filein, O_RDONLY);

                    if (fdi < 0)
                    {
                        perror("Error opening file");
                        exit(1);
                    }

                    dup2(fdi, STDIN_FILENO);
                    close(fdi);
                    execvp(ptr[0], ptr);
                    perror("execvp failed");
                    exit(1);
                }
                execvp(ptr[0], ptr);
                perror("Error in line");
                exit(1);
            }
            else
            {
                if (end == 0)
                {
                    wait(NULL);
                }
                if (out == 1)
                {
                    FILE *fileout;
                    char buffer[1024];
                    fileout = fopen(ptr[search_char[1] + 1], "r");

                    if (fileout == NULL)
                    {
                        perror("Error opening file");
                    }
                    else
                    {
                        while (fgets(buffer, sizeof(buffer), fileout) != NULL)
                        {
                            printf("%s", buffer);
                        }
                    }

                    if (fileout)
                    {
                        fclose(fileout);
                    }
                }
            }
        }
        printf("\nRecent command : %s (!! to use)\n", mark);
    }
    return 0;
}