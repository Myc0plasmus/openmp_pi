#include <stdio.h>
#include <omp.h>

int main() {
	int i;
	omp_set_num_threads(4);
	#pragma omp parallel
	{
		printf("my id= %d \n", omp_get_thread_num());
		printf("Hello World\n");
		#pragma omp for schedule(dynamic)
		for (i = 0; i < 12; i++)
			printf("Iter:%d id: %d\n", i, omp_get_thread_num());

		printf("GoodBye World\n");
	}
}

