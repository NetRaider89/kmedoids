/*
data:			input n-by-p matrix row-major (see gsl matrix storage)
medoids:		input/output vector of k elements. Each element is the 
				index of an element (row) of the data matrix. When the function
				is called it contains the initial value, when the function 
				returns it contains the local best found
clustering:	output vector of n elements that contains for each element the 
				index of the best representing medoid
correlation:	output vector of n elements that contains for each element the
				level of correlation with the best representing medoid
objective:	output value of the objective function when the function 
				terminates
*/

void loadVectorINT(FILE* f, char sep, int* vector, int n, int zeroBased);
void storeVectorINT(FILE* f, char sep, int* vector, int n, int zeroBased);
void loadVector(FILE* f, char sep, double* vector, int n);
void storeVector(FILE* f, char sep, double* vector, int n);
void storeMatrix(FILE* f, char sep, double* matrix, int n, int m);
void loadMatrix(FILE* f, char sep, double* matrix, int n, int m);

int pam(double* data, int n, int m, int* medoids, int k, 
				int* clustering, double* correlation, double* objective, 
				double eps, double sampleSize, FILE *log, char *intermediate, int progress);
