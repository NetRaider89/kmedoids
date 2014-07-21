#include "internals.h"
#include <stdlib.h>

Neighbor* neighbor_alloc(int n)
{
	Neighbor* neigh;
	neigh = (Neighbor*) malloc(sizeof(Neighbor));
	neigh -> newCorr = (double*)calloc(n, sizeof(double));
	neigh -> medoidIdx = -1;
	neigh -> newMedoid = -1;
	neigh -> updated = 0;
	neigh -> objective = -1 * n;
	return neigh;
}

void neighbor_free(Neighbor* n)
{
	free(n -> newCorr);
	free(n);
	return;
}

void assign(double *dataCorr, int n, int k, int* clustering, 
						double* correlation, double* objective)
{
	int i,j,bestIdx;
	double bestVal;
	
	*objective=0.0;
	for(i=0; i<n; i++)
	{
		bestIdx=0;
		bestVal=dataCorr[i+bestIdx*n];
		for(j=1; j<k; j++)
		{
			if(dataCorr[i+j*n] > bestVal)
			{
				bestIdx=j;
				bestVal=dataCorr[i+j*n];
			}
		}
		clustering[i]=bestIdx;
    correlation[i]=bestVal;
    *objective += bestVal;
	}
	return;
}
