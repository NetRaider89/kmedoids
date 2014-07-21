#ifndef __PAM_INTERALS__
	#define __PAM_INTERALS__
	
	struct _neighbor
	{
		int medoidIdx;
		int newMedoid;
		double objective;
		int updated;
		double* newCorr;
	};

	typedef struct _neighbor Neighbor;

	Neighbor* neighbor_alloc(int n);
	void neighbor_free(Neighbor*);
		
	void pairwiseCorr(double* data, int n, int m, int* idx, int idxSize, 
										double* dataCorr, int k);

	void assign(double *dataCorr, int n, int k, int* clustering, 
							double* correlation, double* objective);
#endif
