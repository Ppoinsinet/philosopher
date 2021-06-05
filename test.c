#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>


void ft_usleep(int time)
{
	time /= 10;
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
	usleep(1000 * time);
}

int main()
{
	struct timeval current;
	struct timeval begin;
	gettimeofday(&begin, NULL); 
	long begin_time = begin.tv_usec / 1000 + begin.tv_sec * 1000;
	long current_time = begin.tv_usec / 1000 + begin.tv_sec * 1000;
	int test = 0;
	while (test < 100)
	{
		ft_usleep(20);
		//usleep(20 * 1000);
		//for (int i = 0; i < 10; i++)
		//{
		//	usleep(20 / 10);
		//}
		test++;
	}
	gettimeofday(&current, NULL);
	current_time = current.tv_usec / 1000 + current.tv_sec * 1000;
	printf("time : %ld\n", current_time - begin_time);
	return (0);
}
