#include <math.h>
#include "internals.h"
/*
from: gsl_stats_correlation()
  Calculate Pearson correlation = cov(X, Y) / (sigma_X * sigma_Y)
This routine efficiently computes the correlation in one pass of the
data and makes use of the algorithm described in:

B. P. Welford, "Note on a Method for Calculating Corrected Sums of
Squares and Products", Technometrics, Vol 4, No 3, 1962.

This paper derives a numerically stable recurrence to compute a sum
of products

S = sum_{i=1..N} [ (x_i - mu_x) * (y_i - mu_y) ]

with the relation

S_n = S_{n-1} + ((n-1)/n) * (x_n - mu_x_{n-1}) * (y_n - mu_y_{n-1})
*/

void pairwiseCorr(double* data, int n, int m, int* idx, int idxSize, 
									double* dataCorr, int k)
{	
	double sum_xsq;
  double sum_ysq;
  double sum_cross;
  double ratio;
  double delta_x, delta_y;
  double mean_x, mean_y;
	double corrVal;
	
	int i,j,l;
	int idx1, idx2;
	int corrIdx;
	
	for(i=0; i<n; i++)
	{
		for(j=0; j<idxSize; j++)
		{
			
			idx1=i*m;
			idx2=idx[j]*m;
			
			mean_x = 0;
			mean_y = 0;
			sum_xsq = 0.0;
			sum_ysq = 0.0;
			sum_cross = 0.0;
			/*
			 * Compute:
			 * sum_xsq = Sum [ (x_i - mu_x)^2 ],
			 * sum_ysq = Sum [ (y_i - mu_y)^2 ] and
			 * sum_cross = Sum [ (x_i - mu_x) * (y_i - mu_y) ]
			 * using the above relation from Welford's paper
			 */
			
			mean_x = data[idx1];
			mean_y = data[idx2];

			for(l = 1; l < m; l++)
			{
				idx1=i*m+l;
				idx2=idx[j]*m+l;
				ratio = l / (l + 1.0);
				delta_x = data[idx1] - mean_x;
				delta_y = data[idx2] - mean_y;
				sum_xsq += delta_x * delta_x * ratio;
				sum_ysq += delta_y * delta_y * ratio;
				sum_cross += delta_x * delta_y * ratio;
				mean_x += delta_x / (l + 1.0);
				mean_y += delta_y / (l + 1.0);
			}
			
			//Two pass Correlation classical formula
			/*for(l = 0; l < m; l++)
			{
				idx1=sub2ind(i, l, n, m, rowMajor);
				idx2=sub2ind(idx[j], l, n, m, rowMajor);
				
				mean_x += data[idx1];
				mean_y += data[idx2];
			}
			
			mean_x /= m;
			mean_y /= m;
						
			for(l = 0; l < m; l++)
			{
				idx1=sub2ind(i, l, n, m, rowMajor);
				idx2=sub2ind(idx[j], l, n, m, rowMajor);
				
				sum_xsq += (mean_x-data[idx1])*(mean_x-data[idx1]);
				sum_ysq += (mean_y-data[idx2])*(mean_y-data[idx2]);
				sum_cross += (mean_x-data[idx1])*(mean_y-data[idx2]);
			}*/
			
			//dataCorr is colmajor
			corrIdx = i+j*n;
			corrVal = sum_cross / (sqrt(sum_xsq) * sqrt(sum_ysq));
			dataCorr[corrIdx]=corrVal;
		}
	}
	return;
}
