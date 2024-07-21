#include<stdio.h>		

int exists(char address[])
{
	FILE *file;
	if ((file = fopen(address, "rb")))
	{
		fclose(file);
		return 1;
	}
	return 0;
}

int main()
{
	char file1[1000] , file2[1000];
	printf("Please enter first file name/path :\n");
	scanf("%s", file1);
	printf("Please enter second file name/path :\n");
	scanf("%s", file2);
	int e1 , e2 ;
	e1 = exists(file1);
	if (e1 == 0)
	{
		printf("First file doesn't exist !\n");
		return 0;

	FILE *input , *output;
	input = fopen(file1, "rb");
	output = fopen(file2, "wb");
	char ch = fgetc(input);
	while (ch != EOF)
		fputc (ch, output);
		ch = fgetc(input);
	}
	printf("Copy is done !\n");
	return 1;
}
