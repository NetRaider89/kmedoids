#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pam.h>

//separator is assumed as ',' rowMajor is assumed true

//argv[1] dataset filename
//argv[2] dataset rows
//argv[3] dataset columns
//argv[4] initialMedoids filename
//argv[5] number of medoids
//argv[6] Percentage of neighbours to check (CLARANS)
//argv[7] minimum improvement accepted (eps)
//argv[8] path of the report file

int main(int argc, char** argv)
{
	time_t now;
	char timeString[17];
	struct tm *tm_info;
	
	int iters;
	int n, m, k;
	
	double *dataset;
	int *medoids;
	int *clustering;
	double *correlation;
	double objective;
	
	double sampleSize;
	double eps;
	
	FILE *dataIn, *medoidsIn, *report;

	n=(int) strtol(argv[2],NULL, 10);
	m=(int) strtol(argv[3],NULL, 10);
	k=(int) strtol(argv[5],NULL, 10);

	time(&now);
	tm_info = localtime(&now);
	strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
	fprintf(stdout, "%s - Loading dataset...\n", timeString);

	dataIn = fopen(argv[1], "r");
	dataset=(double *)calloc(n*m, sizeof(double));
	loadMatrix(dataIn, ',', dataset, n, m);
	storeMatrix(dataIn, ',', dataset, n, m);
	
	time(&now);
	tm_info = localtime(&now);
	strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
	fprintf(stdout, "%s - Done.\n", timeString);
	fprintf(stdout, "%s - Loading input medoids...\n", timeString);

	medoidsIn = fopen(argv[4], "r");
	medoids=(int *)calloc(k, sizeof(int));
	loadVectorINT(medoidsIn, ',', medoids, k);
	
	time(&now);
	tm_info = localtime(&now);
	strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
	fprintf(stdout, "%s - Done.\n", timeString);
	
	clustering=(int *)calloc(n, sizeof(int));
	correlation=(double *)calloc(n, sizeof(double));
	
	sampleSize = strtod(argv[6], NULL);
	eps = strtod(argv[7], NULL);
	
	fprintf(stdout, "%s - Clustering...\n", timeString);
	iters = pam(dataset, n, m, medoids, k, clustering, correlation, &objective, eps, sampleSize, stdout);
	time(&now);
	tm_info = localtime(&now);
	strftime(timeString, 17, "%d/%m/%Y %H:%M", tm_info);
	fprintf(stdout, "%s - Done, %d iterations.\n", timeString, iters);
	
	report=fopen(argv[8], "w");
	
	fprintf(report, "Objective function:\n");
	fprintf(report, "%lf\n\nOutput medoids:\n", objective);
	storeVectorINT(report, ',', medoids, k);
	fprintf(report, "\n\nClustering:\n");
	storeVectorINT(report, ',', clustering, n);
	fprintf(report, "\n\nCorrelation:\n");
	storeVector(report, ',', correlation, n);
	
	free(dataset);
	free(medoids);
	free(clustering);
	free(correlation);
	
	fclose(dataIn);
	fclose(medoidsIn);
	fclose(report);
	
	return 0;
}
