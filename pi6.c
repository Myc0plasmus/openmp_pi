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
	double tSum[1025];
	int i;
	int threadNo;
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
   for(i=0;i<1025;i++) tSum[i]=0;

//RÓWNOLEGLE
	#pragma omp parallel shared(tSum) private (threadNo) num_threads(2)
	{
		pswtime = omp_get_wtime();
		ppstart = clock();
		sum = 0.0;
		step = 1. / (double)num_steps;
		threadNo = omp_get_thread_num();
		#pragma omp for reduction(+ : sum)
		for (i = 0; i < num_steps; i++)
		{

			double x;
			x = (i + .5) * step;
			
			tSum[threadNo] += 4.0 / (1. + x * x);
		}
	}
	for(i=0;i<128;i++)
	{
	  sum+=tSum[i];
	}
	pi = sum * step;

	ppstop = clock();
	pewtime = omp_get_wtime();

	printf("%15.12f Wartosc liczby PI rownolegle \n",pi);
	printf("Czas procesorów przetwarzania sekwencyjnego  %f sekund \n", ((double)(spstop - spstart)/CLOCKS_PER_SEC));
	printf("Czas procesorów przetwarzania równoleglego  %f sekund \n", ((double)(ppstop - ppstart)/CLOCKS_PER_SEC));
	printf("Czas trwania obliczen sekwencyjnych - wallclock %f sekund \n",  sewtime-sswtime);
	printf("Czas trwania obliczen rownoleglych - wallclock %f sekund \n", pewtime-pswtime);
	printf("Przyspieszenie %5.3f \n", (sewtime - sswtime) / (pewtime - pswtime));
	return 0;
}
