#include<stdio.h>
#include<omp.h>
#include<time.h>

void answer(int curr_size, float coeff_curr[][3], float *x, int orig_size, float *rhs, float boundary_start, float boundary_end, int *found) {
	if(curr_size == 0) return;
	int new_size = curr_size/2, i, j;
	float coeff_new[new_size][3], rhs_new[new_size];
	#pragma omp parallel for private(i, j)
	for(j = 0; j < new_size; j++) {
		i = 2*j + 1;
		coeff_new[j][0] = (coeff_curr[i-1][0] * coeff_curr[i][0]) / coeff_curr[i-1][1];
		coeff_new[j][1] = ((coeff_curr[i-1][2] * coeff_curr[i][0]) / coeff_curr[i-1][1]) + ((coeff_curr[i][2] * coeff_curr[i+1][0]) / coeff_curr[i+1][1]) - coeff_curr[i][1];
		coeff_new[j][2] = (coeff_curr[i][2] * coeff_curr[i+1][2]) / coeff_curr[i+1][1];
		rhs_new[j] = ((rhs[i-1]*coeff_curr[i][0]) / coeff_curr[i-1][1]) + ((rhs[i+1]*coeff_curr[i][2]) / coeff_curr[i+1][1]) - rhs[i];
	}
	//recursive call
	answer(new_size, coeff_new, x, orig_size, rhs_new, boundary_start, boundary_end, found);
	//--------------------------------------------------------------------------------------------------
	//printf("curr size - %d\n", curr_size);
	//for(i = 0; i < curr_size; i++) {
		//printf("%.2f %.2f %.2f %.2f\n", coeff_curr[i][0], coeff_curr[i][1], coeff_curr[i][2], rhs[i]);
	//}
	//printf("\n");
	
	int gap = orig_size / (curr_size + 1) + 1;
	#pragma omp parallel for private(i, j)
	for(i = gap - 1; i < orig_size; i += gap) {
		if(!found[i]) {
			j = (i + 1)/gap - 1;
			float xprev = (j == 0 ? boundary_start : x[i-gap]);
			float xnext = (j == curr_size - 1 ? boundary_end : x[i+gap]);
			x[i] = (rhs[j] - coeff_curr[j][0]*xprev - coeff_curr[j][2]*xnext) / coeff_curr[j][1];
			found[i] = 1;
		}
	}
}

int main() {
	puts("Enter number of equations");
	int n, i, j; scanf("%d", &n);
	float coeff[n][3], x[n], rhs[n];
	puts("Enter a, b and c coefficients of tridiagonal matrix, followed by d, the rhs of the equation");
	for(i = 0; i < n; i++) {
		scanf("%f %f %f %f", &coeff[i][0], &coeff[i][1], &coeff[i][2], &rhs[i]);
	}
	puts("The equation entered is of the form AX = B, with A being the coefficients, X is unknown, and B is right hand side of equations.");
	if(n<32){
		puts("Entered value of A is -\n");
		for(i = 0; i < n; i++) {
			if(i == 0) {
				printf("%0.1f %0.1f ",  coeff[i][1], coeff[i][2]);
				for(j = 0; j < n-2; j++) printf("0.0 ");
			}
			else if(i == n-1) {
				for(j = 0; j < n-2; j++) printf("0.0 ");
				printf("%0.1f %0.1f ",  coeff[i][0], coeff[i][1]);
			}
			else {
				for(j = 0; j < i-1; j++) printf("0.0 ");
				printf("%0.1f %0.1f %0.1f ",  coeff[i][0], coeff[i][1], coeff[i][2]);
				for(j = 0; j < n - (i + 2); j++) printf("0.0 ");
			}
			printf("\n");
		}
	}
	int found[n];
	for(i = 0; i < n; i++) found[i] = 0;
	puts("Enter boundary values, first starting value then ending value");
	float b1, b2;
	scanf("%f", &b1);
	scanf("%f", &b2);
	long double t = clock();
	answer(n, coeff, x, n, rhs, b1, b2, found);
	t = clock() - t;
	for(i = 0; i < n; i++) printf("%.5f\n", x[i]);
	printf("Time Taken :%0.5Lf", t/CLOCKS_PER_SEC);
}