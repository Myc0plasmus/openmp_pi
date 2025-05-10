#include <stdio.h>
#include <time.h>
#include <omp.h>

long long num_steps = 10000000;
double step;

int main(int argc, char* argv[])
{
	clock_t spstart, spstop,ppstart,ppstop;
	
	double sswtime, sewtime, pswtime, pewtime;
//volatile
	double pi, sum=0.0;
	volatile double tSum[50];
	int threadNo;
	int i;
//SEKWENCYJNIE
	sswtime = omp_get_wtime();
	spstart = clock();

	step = 1./(double)num_steps;

	for (i=0; i<num_steps; i++)
	{
		double x = (i + .5)*step;
		sum = sum + 4.0/(1.+ x*x);
	}
	
	pi = sum*step;

	spstop = clock();
	sewtime = omp_get_wtime();
	printf("%15.12f artosc liczby PI sekwencyjnie \n", pi);
	printf("Czas procesorów przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart)/CLOCKS_PER_SEC));
	printf("Czas trwania obliczen sekwencyjnych - wallclock %f sekund \n",  sewtime-sswtime);

	
	for(i=0;i<50;i++) tSum[i]=0;

	for(int offset=0;offset<49;offset++) {
		tSum[offset] =0;
		tSum[offset+1] =0;



		pswtime = omp_get_wtime();
		ppstart = clock();
		//RÓWNOLEGLE
		#pragma omp parallel shared(tSum) private (threadNo) num_threads(2)
		{
			step = 1. / (double)num_steps;
			threadNo = omp_get_thread_num();
			// #pragma omp for 
			for (i = threadNo; i < num_steps; i+=2)
			{

				double x;
				x = (i + .5) * step;
				
				tSum[threadNo+offset] += 4.0 / (1. + x * x);
			}
		}
		pi = (tSum[offset]+tSum[offset+1]) * step;

		ppstop = clock();
		pewtime = omp_get_wtime();

		printf("Obliczenia równoległe dla komórek: %d i %d\n", offset, offset+1);
		printf("%15.12f Wartosc liczby PI rownolegle \n",pi);
		printf("Czas procesorów przetwarzania równoleglego  %f sekund \n", ((double)(ppstop - ppstart)/CLOCKS_PER_SEC));
		printf("Czas trwania obliczen rownoleglych - wallclock %f sekund \n", pewtime-pswtime);
		printf("Przyspieszenie %5.3f \n", (sewtime - sswtime) / (pewtime - pswtime));
	}
	return 0;
}
