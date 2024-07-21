#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 1000

int *firstGlobalVariable;
int *secondGlobalVariable;
int *sortedList;
int sizeprime;

void deleteLastChar(char *str)
{ /* deleting enter */
    int len = strlen(str);
    str[len - 1] = '\0';
}

void *sort(void *arg)
{
    char *str = (char *)arg;

    if (strcmp(str, "first") == 0)
    {
        for (int i = 0; i < sizeprime / 2 + sizeprime % 2 - 1; i++)
        {
            for (int j = 0; j < sizeprime / 2 + sizeprime % 2 - i - 1; j++)
            {
                if (firstGlobalVariable[j] > firstGlobalVariable[j + 1])
                {
                    int temp = firstGlobalVariable[j];
                    firstGlobalVariable[j] = firstGlobalVariable[j + 1];
                    firstGlobalVariable[j + 1] = temp;
                }
            }
        }
    }
    else if (strcmp(str, "second") == 0)
    {
        for (int i = 0; i < sizeprime / 2 - 1; i++)
        {
            for (int j = 0; j < sizeprime / 2 - i - 1; j++)
            {
                if (secondGlobalVariable[j] > secondGlobalVariable[j + 1])
                {
                    int temp = secondGlobalVariable[j];
                    secondGlobalVariable[j] = secondGlobalVariable[j + 1];
                    secondGlobalVariable[j + 1] = temp;
                }
            }
        }
    }
}

void *mergeLists()
{
    int i = 0, j = 0, k = 0, size1 = sizeprime / 2 + sizeprime % 2, size2 = sizeprime / 2;

    while (i < size1 && j < size2)
    {
        if (firstGlobalVariable[i] <= secondGlobalVariable[j])
        {
            sortedList[k] = firstGlobalVariable[i];
            i++;
        }
        else
        {
            sortedList[k] = secondGlobalVariable[j];
            j++;
        }
        k++;
    }

    while (i < size1)
    {
        sortedList[k] = firstGlobalVariable[i];
        i++;
        k++;
    }

    while (j < size2)
    {
        sortedList[k] = secondGlobalVariable[j];
        j++;
        k++;
    }
}

int main()
{
    int theList[MAX_LENGTH];
    char input[MAX_LENGTH];
    printf("Please enter the list :\n");
    fgets(input, sizeof(input), stdin);
    deleteLastChar(input);

    int count = 0;
    char *token = strtok(input, " ");

    while (token)
    {
        theList[count] = atoi(token);
        count++;
        token = strtok(NULL, " ");
    }
    sizeprime = count;
    firstGlobalVariable = (int *)malloc(sizeprime * sizeof(int));
    secondGlobalVariable = (int *)malloc(sizeprime * sizeof(int));
    sortedList = (int *)malloc(sizeprime * 2 * sizeof(int));

    for (int i = 0; i < sizeprime; i++)
    {
        if (i < sizeprime / 2 + sizeprime % 2)
        {
            firstGlobalVariable[i] = theList[i];
        }
        else
        {
            secondGlobalVariable[i - (sizeprime / 2 + sizeprime % 2)] = theList[i];
        }
    }

    pthread_t tid1;                             /* the thread identifier */
    pthread_create(&tid1, NULL, sort, "first"); /* create the thread */
    pthread_join(tid1, NULL);                   /* wait for the thread to exit */

    printf("\nSorted list from first thread : \n");
    for (int i = 0; i < sizeprime / 2 + sizeprime % 2; i++)
    {
        printf("%d ", firstGlobalVariable[i]);
    }

    pthread_t tid2;                              /* the thread identifier */
    pthread_create(&tid2, NULL, sort, "second"); /* create the thread */
    pthread_join(tid2, NULL);                    /* wait for the thread to exit */

    printf("\n\nSorted list from second thread : \n");
    for (int i = 0; i < sizeprime / 2; i++)
    {
        printf("%d ", secondGlobalVariable[i]);
    }

    pthread_t tid;                                /* the thread identifier */
    pthread_create(&tid, NULL, mergeLists, NULL); /* create the thread */
    pthread_join(tid, NULL);                      /* wait for the thread to exit */

    printf("\n\nMerged list thread : \n");
    for (int i = 0; i < sizeprime; i++)
    {
        printf("%d ", sortedList[i]);
    }
}