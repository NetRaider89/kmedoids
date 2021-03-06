#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pam.h>
#include "internals.h"

#define PROGRESS_WIDTH 50
#define FILENAME_LENGTH 256

int pam(double* dataset, int n, int m, int* medoids, int k, 
				int* clustering, double* correlation, double* objective, 
				double eps, double sampleSize, FILE *log, char *intermediate, int progress)
{
	int iterations;
	time_t now;
	char timeString[17];
	struct tm *tm_info;
	
	FILE *intsol;
	char intermediate_filename[FILENAME_LENGTH];
	
	int localOptimum;	
	int i;
	
	int best, bestIdx;
	double value, bestValue;
	double *correlation2 = (double*)calloc(sizeof(double), n);
	
	float ratio;
	int perc, count;

	double r;
	int* doNotSwap = (int*)calloc(sizeof(int), n);
	for(i=0;i<k;i++)
	{
		doNotSwap[medoids[i]]=1;
	}
	
	srand(time(NULL));
	*objective = 	evaluateSolution(dataset, n, m, medoids, k, clustering,
								correlation, correlation2);
	if(log)
	{
		time(&now);
		tm_info = localtime(&now);
		strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
		fprintf(log, "%s - Initial objective value %.17g\n", 
						timeString, *objective);
	}
	
	//Iteratively until no improvement of the objective function
	//search in the neghborhood of the current solution a better feasible 
	//solution and update current state.
	//A neighbour solution is given swapping each element with it's 
	//prototype medoid.
	//This leads to a neighborhood of size n (actually n-k, but 
	//k << n), where n is
	//the number of objects.

	localOptimum=0;
	iterations=0;

	//3.Search in the neighborhood to improve the objective function
	while(!localOptimum)
	{
		bestValue = -150;
		best = -1;
		bestIdx = -1;
		for(i=0; i < n; i++)
		{
			if(doNotSwap[i]) continue;
			r = rand() / (RAND_MAX + 1.0);
			if(r > sampleSize) continue;
			value = evaluateNeighbor(dataset, n, m, medoids, k, clustering,
							correlation, correlation2, i);
			if(value > bestValue)
			{
				bestValue = value;
				best = i;
				bestIdx = clustering[i];
			}
			if(progress)
			{
				//shows a text progress bar
				ratio = ((float)i)/((float)(n-1));
				perc = (int)(ratio * PROGRESS_WIDTH);

				if(!((int)(ratio*100) % 10))
				{
					printf("\r\tIteration %d - %3d%%[", iterations, (int)(ratio*100));
					for(count=0; count < perc; count++)
					{
						printf("=");
					}
					
					for(count=perc; count < PROGRESS_WIDTH; count++)
					{
						printf(" ");
					}
					printf("]");
					fflush(stdout);
				}
			}
		}
		
		if(progress)
		{
			printf("\n");
		}

		if(bestValue > eps)
		{
			doNotSwap[medoids[bestIdx]]=0;
			doNotSwap[best]=1;
			medoids[bestIdx]=best;
			//smarter update
			*objective = 	evaluateSolution(dataset, n, m, medoids, k, clustering,
								correlation, correlation2);
			
			if(log)
			{
				time(&now);
				tm_info = localtime(&now);
				strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
				fprintf(log, "%s - Updating solution, new objective value %.17g\n", 
								timeString,  *objective);
			}
			if(intermediate)
			{
				if(log)
				{
					time(&now);
					tm_info = localtime(&now);
					strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
					fprintf(log, "%s - Saving intermediate results\n", timeString);
				}
				snprintf(intermediate_filename, FILENAME_LENGTH, "%s/iteration_%d", intermediate, iterations);
				intsol = fopen(intermediate_filename, "w");
				fprintf(intsol, "Iteration %d:\nMedoids\n", iterations);
				storeVectorINT(intsol, ',', medoids, k, 0);
				fprintf(intsol, "Clustering\n");
				storeVectorINT(intsol, ',', clustering, n, 0);
				fprintf(intsol, "\n");
				fclose(intsol);
			}
		}
		else
		{
			//4.Return if no improvement
			localOptimum=1;
		}

		iterations++;
	}
	free(doNotSwap);
	free(correlation2);
	
	if(log)
	{
		time(&now);
		tm_info = localtime(&now);
		strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
		fprintf(log, "%s - Returning\n", timeString);
	}
	
	return iterations;
}
