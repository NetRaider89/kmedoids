#include "internals.h"
#include <stdlib.h>

double evaluateSolution(double *dataset, int n, int m, int *medoids, int k, 
												int *clustering, double *correlation, double *correlation2)
{
	int i, j;
	
	double c, objective;
	double *obj1, *obj2;
	
	objective = 0.0;
	for(i = 0; i < n; i++)
	{
		correlation[i] = correlation2[i] = -1;
		for(j = 0; j < k; j++)
		{
			obj1 = dataset + (i * m);
			obj2 = dataset + (medoids[j] * m);
			c = corr(obj1, obj2, m);
			
			if(c > correlation2[i])
			{
				if(c > correlation[i])
				{
					correlation2[i] = correlation[i];
					correlation[i] = c;
					clustering[i] = j;
				}
				else
				{
					correlation2[i] = c;
				}
			}
		}
		objective += correlation[i];
	}
	return objective;
}

double evaluateNeighbor(double *dataset, int n, int m, int *medoids, int k, 
												int *clustering, double *correlation, double *correlation2, 
												int neigh)
{
	int i;
	double *obj1, *obj2;
	double c, value;
	
	value = 0.0;
	for(i = 0; i < n; i++)
	{
		obj1 = dataset + (i * m);
		obj2 = dataset + (neigh * m);
		c = corr(obj1, obj2, m);
		
		if(clustering[i] == clustering[neigh])
		{
			if(c >= correlation2[i])
			{
				value += c - correlation[i];
			}
			else
			{
				value += correlation2[i] - correlation[i];
			}
		}
		else
		{
			if(c > correlation[i])
			{
				value += c - correlation[i];
			}
		}
	}
	
	return value;
}
