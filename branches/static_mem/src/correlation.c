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

double corr(double *obj1, double *obj2, int n)
{
	double sum_xsq;
  double sum_ysq;
  double sum_cross;
  double ratio;
  double delta_x, delta_y;
  double mean_x, mean_y;
	double corrVal;
	
	int i;
	
	mean_x = obj1[0];
	mean_y = obj2[0];
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

	for(i = 1; i < n; i++)
	{
		ratio = i / (i + 1.0);
		delta_x = obj1[i] - mean_x;
		delta_y = obj2[i] - mean_y;
		sum_xsq += delta_x * delta_x * ratio;
		sum_ysq += delta_y * delta_y * ratio;
		sum_cross += delta_x * delta_y * ratio;
		mean_x += delta_x / (i + 1.0);
		mean_y += delta_y / (i + 1.0);
	}

	return sum_cross / (sqrt(sum_xsq) * sqrt(sum_ysq));
}
