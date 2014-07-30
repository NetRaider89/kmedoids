#include <mex.h>
#include <matrix.h>
#include <stdlib.h>
#include "pam.h"

//gateway function
//matlab is column major
//function invocation
//[medoids, clustering, correlation, objective] = optiPAM(dataset', initialMedoids);
//dataset is a n-by-m matrix of doubles
//initialMedoids is a k-by-1 column vector of int32
//medoids is a k-by-1 column uint32 vector
//clustering is a n-by-1 column uint32 vector
//correlation is a n-by-1 column double vector
//objective is a double scalar value
void mexFunction(int nlhs, mxArray* plhs[], int nrhs, const mxArray* prhs[])
{
	int n, m, k;
	double* data;
	int* medoids;
	int* inMedoids;
	int* clustering;
	double* correlation;
	double objective;
	
	int i, j, iters;

	mexPrintf("Initializing...");

	n=mxGetM(prhs[0]);
	m=mxGetN(prhs[0]);
	k=mxGetM(prhs[1]);
    
    mexPrintf("Dims=%dx%d, medoids=%d\n", n,m,k);

	//input arrays
	/*dataset = (gsl_matrix*)malloc(sizeof(gsl_matrix));
	dataset -> size1 = nObjects;
	dataset -> size2 = nFeatures;
	dataset -> tda = nObjects;
	dataset -> data = mxGetPr(prhs[0]);
	dataset -> block = NULL;
	dataset -> owner = 0;*/
	data = mxGetPr(prhs[0]);

	inMedoids = (int*)mxGetData(prhs[1]);
	
	//output arrays
	//medoids
	plhs[0] = mxDuplicateArray(prhs[1]);
	medoids = (int*)mxGetData(plhs[0]);

	//clustering
	plhs[1] = mxCreateNumericMatrix(n, 1, mxINT32_CLASS, mxREAL);
	clustering = (int*)mxGetData(plhs[1]);
	
	//correlation
	plhs[2] = mxCreateDoubleMatrix(n, 1, mxREAL);
	correlation = mxGetPr(plhs[2]);
	
	mexPrintf("Done\n");
	mexEvalString("drawnow;");
	//computational routine
	iters = pam(data, n, m, medoids, k, clustering, correlation, &objective);
	
	//objective
	plhs[3] = mxCreateDoubleScalar(objective);
	
	mexPrintf("Total number of Iterations: %d\nReturning\n", iters);

	return;
}
