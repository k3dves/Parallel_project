#include<stdio.h>
#include<time.h>
void answer(int curr_size, long double coeff_curr[][3], long double *x, int orig_size, long double *rhs, long double boundary_start, long double boundary_end, int *found) {
	if(curr_size == 0) return;
	int new_size = curr_size/2, i, j;
	long double coeff_new[new_size][3], rhs_new[new_size];
	for(j = 0; j < new_size; j++) {
		i = 2*j + 1;
		coeff_new[j][0] = (coeff_curr[i-1][0] * coeff_curr[i][0]) / coeff_curr[i-1][1];
		coeff_new[j][1] = ((coeff_curr[i-1][2] * coeff_curr[i][0]) / coeff_curr[i-1][1]) + ((coeff_curr[i][2] * coeff_curr[i+1][0]) / coeff_curr[i+1][1]) - coeff_curr[i][1];
		coeff_new[j][2] = (coeff_curr[i][2] * coeff_curr[i+1][2]) / coeff_curr[i+1][1];
		if(coeff_new[j][2] == 0) printf("%Lf %Lf %Lf\n", coeff_curr[i][2], coeff_curr[i+1][2], coeff_curr[i+1][1]);
		rhs_new[j] = ((rhs[i-1]*coeff_curr[i][0]) / coeff_curr[i-1][1]) + ((rhs[i+1]*coeff_curr[i][2]) / coeff_curr[i+1][1]) - rhs[i];
	}
	//-----------------------------------------------------------------------------------------------------------------------
	answer(new_size, coeff_new, x, orig_size, rhs_new, boundary_start, boundary_end, found);
	//printf("curr size - %d\n", curr_size);
	//for(i = 0; i < curr_size; i++) {
		//printf("%.2f %.2f %.2f %.2f\n", coeff_curr[i][0], coeff_curr[i][1], coeff_curr[i][2], rhs[i]);
	//}
	//printf("\n");
	
	int gap = orig_size / (curr_size + 1) + 1;
	for(i = gap - 1, j = 0; i < orig_size; i += gap, j++) {
		if(!found[i]) {
			long double xprev = (j == 0 ? boundary_start : x[i-gap]);
			long double xnext = (j == curr_size - 1 ? boundary_end : x[i+gap]);
			x[i] = (rhs[j] - coeff_curr[j][0]*xprev - coeff_curr[j][2]*xnext) / coeff_curr[j][1];
			found[i] = 1;
		}
	}
}

int main() {
	int n, i; scanf("%d", &n);
	long double coeff[n][3], x[n], rhs[n];
	for(i = 0; i < n; i++) {
		scanf("%Lf %Lf %Lf %Lf", &coeff[i][0], &coeff[i][1], &coeff[i][2], &rhs[i]);
	}
	int found[n]; for(i = 0; i < n; i++) found[i] = 0;
	long double t = clock();
	answer(n, coeff, x, n, rhs, 1.0, 1.0, found);
	t = clock() - t;
	for(i = 0; i < n; i++) printf("%.5Lf  ", x[i]);
	printf("\n%.5Lf", t/CLOCKS_PER_SEC);
}
