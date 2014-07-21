#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include "internals.h"

// #ifdef MEX
// #include <mex.h>
// #endif

//assume that data is stored rowmajor
//assume that dataCorr is stored colmajor

int pam(double* data, int n, int m, int* medoids, int k, 
				int* clustering, double* correlation, double* objective, 
				double eps, double sampleSize, FILE *log)
{
	int iterations;
	time_t now;
	char timeString[17];
	struct tm *tm_info;
	
	//correlations matrix (colum major for computational ease)
	double* dataCorr = (double*)calloc(n*k, sizeof(double));
	
	//neighbor solution
	int* neigh_medoids = (int*)calloc(k, sizeof(int));
	int* neigh_clustering = (int*)calloc(n, sizeof(int));
	double* neigh_correlation = (double*)calloc(n, sizeof(double));
	double neigh_objective;
	
	int oldMedoidIdx, oldMedoidVal;
	double* oldMedoidCorr=(double*)calloc(n, sizeof(double));
	double* corrVec;
	
	Neighbor* best_neigh = neighbor_alloc(n);
	int localOptimum;	
	int i;
	
	double r;
	int* doNotSwap = (int*)calloc(sizeof(int), n);
	for(i=0;i<k;i++)
	{
		doNotSwap[medoids[i]]=1;
	}
	
	srand(time(NULL));
	
	//From the data matrix and the initial medoids, compute 
	//the initial values for clustering and correlation vectors
	//and the initial objective function value
	
	if(log)
	{
		time(&now);
		tm_info = localtime(&now);
		strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
		fprintf(log, "%s - Computing correlations...\n", timeString);
	}
	
	
	//1.Calculate dataCorr from data and medoids
	/*if(outStream)*/ 
	// #ifdef MEX
		// mexPrintf("Computing correlations...\n");
	// #endif
	
	// #ifdef MEX
		// mexPrintf("Initial medoids\n");
		// int index;
		// for(index=0;index<k;index++)
		// {
			// mexPrintf("%d\t", medoids[index]);
		// }
		// mexPrintf("\n");
	// #endif
	
	//corr(data, medoids, dataCorr);
	pairwiseCorr(data, n, m, medoids, k, dataCorr, k);
	/*if(outStream)*/ 
	// #ifdef MEX
		// mexPrintf("Initial dataCorr\n");
		// for(index=0;index<n;index++)
		// {
			// mexPrintf("%.4lf\t%.4lf\t%.4lf\n", dataCorr[sub2ind(index,0,n,k,0)], dataCorr[sub2ind(index,1,n,k,0)], dataCorr[sub2ind(index,2,n,k,0)]);
		// }
	// #endif
	
	// #ifdef MEX
		// mexPrintf("\bDone\n");
	// #endif
	
	if(log)
	{
		time(&now);
		tm_info = localtime(&now);
		strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
		fprintf(log, "%s - Done.\n", timeString);
	}
					//printf("initial correlation\n");
					//gsl_matrix_fprintf(stdout, dataCorr, "%f");
					//getchar();
	//2.Given dataCorr assign each object to the most correlated medoid
	assign(dataCorr, n, k, clustering, correlation, objective);
	// #ifdef MEX
		// mexPrintf("Initial objective value %lf\n", *objective);
		// mexEvalString("input('');");
	// #endif
	
	if(log)
	{
		time(&now);
		tm_info = localtime(&now);
		strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
		fprintf(log, "%s - Initial objective value %lf\n", timeString,*objective);
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
	memcpy(neigh_medoids, medoids, k * sizeof(int));
	//3.Search in the neighborhood to improve the objective function
	while(!localOptimum)
	{
		best_neigh -> objective = *objective;
		best_neigh -> updated = 0;
		for(i=0; i < n; i++)
		{
			if(doNotSwap[i]) continue;
			r = rand() / (RAND_MAX + 1.0);
			if(r > sampleSize) continue;
			
			oldMedoidIdx = clustering[i];
			oldMedoidVal = medoids[oldMedoidIdx];
			//swap medoid m with object i
			neigh_medoids[oldMedoidIdx]=i;
			corrVec = &dataCorr[oldMedoidIdx*n];
			
			//store correlations of current medoid
			memcpy(oldMedoidCorr, corrVec, n * sizeof(double));
							//printf("neighbor solution\n");
							//gsl_vector_int_fprintf(stdout, neigh_medoids, "%d");
							//getchar();
			
			//calculate correlation of potential new medoid
			pairwiseCorr(data, n, m, &i, 1, corrVec, 1);
									//printf("current correlation\n");
									//gsl_matrix_fprintf(stdout, dataCorr, "%f");
									//getchar();
			assign(	dataCorr, n, k, neigh_clustering, neigh_correlation, 
							&neigh_objective);
									//printf("neigh assignment\n");
									//gsl_vector_int_fprintf(stdout, neigh_clustering, "%d");
									//getchar();
								
				// #ifdef MEX
					// mexPrintf("Exchanging medoid %d with data object %d with an obj of %lf\n", oldMedoidIdx, i, neigh_objective);
					// mexEvalString("input('');");
				// #endif
			if((neigh_objective - (best_neigh -> objective)) > eps)
			{
			// #ifdef MEX
				// mexPrintf("Updating best neighbor");
				// mexEvalString("input('');");
			// #endif
				best_neigh -> medoidIdx = oldMedoidIdx;
				best_neigh -> newMedoid = i;
				best_neigh -> objective = neigh_objective;
				memcpy(best_neigh -> newCorr, corrVec, n * sizeof(double));
				best_neigh -> updated = 1;
			}
			else
			{
				neigh_medoids[oldMedoidIdx]=oldMedoidVal;
				memcpy(corrVec, oldMedoidCorr, n * sizeof(double));
			}
		}
		iterations++;
		if(best_neigh -> updated)
		{
			// #ifdef MEX
				// mexPrintf("Updating solution substituting medoid %d\n", best_neigh->medoidIdx);
				// mexEvalString("input('');");
			// #endif
			//solution update
			doNotSwap[medoids[best_neigh->medoidIdx]]=0;
			doNotSwap[best_neigh->newMedoid]=1;
			medoids[best_neigh->medoidIdx]=best_neigh->newMedoid;
			corrVec = &dataCorr[best_neigh->medoidIdx * n];
			memcpy(corrVec, best_neigh->newCorr, n * sizeof(double));
			assign(dataCorr, n, k, clustering, correlation, objective);
			// #ifdef MEX
				// mexPrintf("Updating solution, new objective value %lf\n", *objective);
				// mexEvalString("input('');");
			// #endif
			
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
	
	free(dataCorr);
	free(neigh_medoids);
	free(neigh_clustering);
	free(neigh_correlation);	
	free(oldMedoidCorr);
	free(doNotSwap);
	neighbor_free(best_neigh);
	// #ifdef MEX
		// mexPrintf("Local minimum reached\n");
	// #endif
	
	if(log)
	{
		time(&now);
		tm_info = localtime(&now);
		strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
		fprintf(log, "%s - Returning\n", timeString);
	}
	
	return iterations;
}
