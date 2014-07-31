#ifndef __PAM_INTERALS__
	#define __PAM_INTERALS__
	
	double evaluateSolution(double *dataset, int n, int m, int *medoids, int k,
												int *clustering, double *correlation, double *correlation2);
	
	double evaluateNeighbor(double *dataset, int n, int m, int *medoids, int k,
												int *clustering, double *correlation, double * correlation2, int neigh);
	
	double corr(double *obj1, double *obj2, int n);
#endif
