#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX_LENGTH 1000

void *runner(void *param); /* threads call this function */

void deleteLastChar(char *str)
{ /* deleting enter */
    int len = strlen(str);
    str[len - 1] = '\0';
}

int isInArray(int arr[])
{
    for (int i = 0; i < 27; i++)
    {
        if (arr[i] == 0)
        {
            return 1; // Return 1 if 0 is found in the array
        }
    }
    return 0;
}
int main()
{
    char *lines[9];
    char *sudoku[9][9];
    int *thread_result;
    printf("Please enter your sudoku line by line :\n");
    for (int i = 0; i < 9; i++)
    {
        char input[MAX_LENGTH];
        fgets(input, sizeof(input), stdin);
        deleteLastChar(input);
        lines[i] = strdup(input);
        int count = 0;
        char *token = strtok(lines[i], " ");
        while (token)
        {
            sudoku[i][count] = strdup(token);
            count++;
            token = strtok(NULL, " ");
        }
    }
    int results[27];

    // rows :
    char *listi[9];
    for (int k = 0; k < 9; k++)
    {
        for (int m = 0; m < 9; m++)
        {
            listi[m] = sudoku[k][m];
        }
        pthread_t tid; /* the thread identifier */
        /* create the thread */
        pthread_create(&tid, NULL, runner, (void *)listi);
        /* wait for the thread to exit */
        pthread_join(tid, (void **)&thread_result);
        results[k + 9] = *thread_result;
    }

    // columns :
    for (int k = 0; k < 9; k++)
    {
        for (int m = 0; m < 9; m++)
        {
            listi[m] = sudoku[m][k];
        }
        pthread_t tid; /* the thread identifier */
        /* create the thread */
        pthread_create(&tid, NULL, runner, (void *)listi);
        /* wait for the thread to exit */
        pthread_join(tid, (void **)&thread_result);
        results[k + 18] = *thread_result;
    }

    // squares :
    for (int a = 0; a < 3; a++)
    {
        for (int n = 0; n < 3; n++)
        {
            for (int k = 3 * a; k < 3 * a + 3; k++)
            {
                for (int m = 3 * n; m < 3 * n + 3; m++)
                {
                    listi[((k - 3 * a) * 3 + m - 3 * n)] = sudoku[k][m];
                }
            }
            pthread_t tid; /* the thread identifier */
            /* create the thread */
            pthread_create(&tid, NULL, runner, (void *)listi);
            /* wait for the thread to exit */
            pthread_join(tid, (void **)&thread_result);
            results[(3 * a + n)] = *thread_result;
        }
    }

    if (isInArray(results) == 0)
    {
        printf("The sudoku is True!");
    }
    else
    {
        printf("The sudoku is False!");
    }
    return 0;
}

void *runner(void *param)
{
    char **listi = (char **)param;
    int *thread_result = malloc(sizeof(int));
    *thread_result = 0;
    char *listj[9] = {"1", "2", "3", "4", "5", "6", "7", "8", "9"};
    int sizei = 9;
    int sizej = 9;
    int match = 0;
    for (int i = 0; i < sizei; i++)
    {
        for (int j = 0; j < sizej; j++)
        {
            if (strcmp(listi[j], listj[i]) == 0)
            {
                listj[i] = "0";
                match++;
            }
        }
    }
    if (match == 9)
    {
        *thread_result = 1;
    }
    pthread_exit(thread_result);
}