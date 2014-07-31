#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "internals.h"

int pam(double* dataset, int n, int m, int* medoids, int k, 
				int* clustering, double* correlation, double* objective, 
				double eps, double sampleSize, FILE *log)
{
	int iterations;
	time_t now;
	char timeString[17];
	struct tm *tm_info;
	
	int localOptimum;	
	int i;
	
	int best, bestIdx;
	double value, bestValue;
	double *correlation2 = (double*)calloc(sizeof(double), n);
	
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
		fprintf(log, "%s - Initial objective value %lf\n", 
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
		}
		iterations++;
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
				fprintf(log, "%s - Updating solution, new objective value %lf\n", timeString,  *objective);
			}
		}
		else
		{
			//4.Return if no improvement
			localOptimum=1;
		}
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
