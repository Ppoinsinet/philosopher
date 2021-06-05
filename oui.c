#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


int main()
{
	struct timeval current;
	struct timeval begin;
	gettimeofday(&begin, NULL); 
	long begin_time = begin.tv_usec / 1000 + begin.tv_sec * 1000;
	long current_time = begin.tv_usec / 1000 + begin.tv_sec * 1000;
	int test = 0;
	while (test < 10000)
	{
		gettimeofday(&current, NULL);
		current_time = current.tv_usec / 1000 + current.tv_sec * 1000;
		long value = (test + 1) * 20;
		long diff = value - (current_time - begin_time);
		if (diff < 0)
			diff = 0;
		printf("desired %ld et current %ld = %ld\n", value, current_time - begin_time,
				diff);
		usleep(1000 * diff);
		current_time += diff;
		test++;
	}
	gettimeofday(&current, NULL);
	current_time = current.tv_usec / 1000 + current.tv_sec * 1000;
	printf("time : %ld\n", current_time - begin_time);
	return (0);
}
