#include <stdio.h>
#include <stdlib.h>

#define BUFF_SIZE	4096
#define ACC_SIZE	128

void loadVector(FILE* f, char sep, double* vector, int n)
{
	char buff[BUFF_SIZE];
	char accumulator[ACC_SIZE];
	int k, accIdx, count;
	int idx;

	idx = 0;
	accIdx=0;
	while(count=fread(buff, sizeof(char), BUFF_SIZE, f))
	{
		for(k=0; k<count;k++)
		{
			if((buff[k]==sep) || (buff[k]=='\n'))
			{
				accumulator[accIdx]='\0';
				vector[idx]=strtod(accumulator,NULL);
				accIdx=0;
				idx++;
			}
			else
			{
				accumulator[accIdx]=buff[k];
				accIdx++;
			}
		}
	}
	return;
}

void storeVector(FILE* f, char sep, double* vector, int n)
{
	int i;
	
	for(i=0; i < (n-1); i++)
	{
		fprintf(f, "%.17g%c", vector[i], sep);
	}
	fprintf(f, "%.17g\n", vector[i]);
	return;
}

void loadVectorINT(FILE* f, char sep, int* vector, int n, int zeroBased)
{
	char buff[BUFF_SIZE];
	char accumulator[ACC_SIZE];
	int k, accIdx, count;
	int idx;

	idx = 0;
	accIdx=0;
	while(count=fread(buff, sizeof(char), BUFF_SIZE, f))
	{
		for(k=0; k<count;k++)
		{
			if((buff[k]==sep) || (buff[k]=='\n'))
			{
				accumulator[accIdx]='\0';
				vector[idx]= (int) strtol(accumulator,NULL, 10);
				if(!zeroBased)
				{
					vector[idx]--;
				}
				accIdx=0;
				idx++;
			}
			else
			{
				accumulator[accIdx]=buff[k];
				accIdx++;
			}
		}
	}
	return;
}

void storeVectorINT(FILE* f, char sep, int* vector, int n, int zeroBased)
{
	int i;
	
	for(i=0; i < (n-1); i++)
	{
		fprintf(f, "%d%c", zeroBased ? vector[i] : vector[i] + 1 , sep);
	}
	fprintf(f, "%d\n", zeroBased ? vector[i] : vector[i] + 1);
	return;
}

void loadMatrix(FILE* f, char sep, double* matrix, int n, int m)
{
	char buff[BUFF_SIZE];
	char accumulator[ACC_SIZE];
	int i, j, k, accIdx, count;
	int idx;

	i = j = 0;
	accIdx=0;
	while(count=fread(buff, sizeof(char), BUFF_SIZE, f))
	{
		for(k=0; k<count;k++)
		{
			if((buff[k]==sep) || (buff[k]=='\n'))
			{
				accumulator[accIdx]='\0';
				idx = i*m+j;
				matrix[idx]=strtod(accumulator, NULL);
				accIdx=0;
				j++;
				if(j == m) {j=0; i++;}
			}
			else
			{
				accumulator[accIdx]=buff[k];
				accIdx++;
			}
		}
	}
	return;
}

void storeMatrix(FILE* f, char sep, double* matrix, int n, int m)
{
	int i, j;
	int idx;
	
	for(i=0; i < n; i++)
	{
		for(j=0; j<(m-1); j++)
		{
			idx=i*m+j;
			fprintf(f, "%.17g%c", matrix[idx], sep);
		}
		idx=i*m+j;
		fprintf(f, "%.17g\n", matrix[idx]);
	}
	return;
}
