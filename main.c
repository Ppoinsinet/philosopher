#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct		s_philo
{
	int				index;
	pthread_t		thread;
	pthread_t		checker;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				n_eat;
	long				life;
	pthread_mutex_t	myFork;
	struct s_philo	*otherFork;
	void			*scene;
}					t_philo;

typedef struct	s_scene
{
	int			num_philo;
	int			time_to_die;
	int			time_to_eat;
	int			time_to_sleep;
	int			n_eat;
	long		begin;
	t_philo		*tab;
	char		finished;
}				t_scene;

long getTime(void)
{
	struct timeval current;

	gettimeofday(&current, NULL);
	long diff = current.tv_sec * 1000 + current.tv_usec / 1000;
	return (current.tv_usec / 1000 + current.tv_sec * 1000);
}

void ft_usleep(int amount)
{
	long tmp;

	tmp = getTime();
	while (getTime() < tmp + amount)
	{
		usleep(1);
	}
}

void	*checkLife(void *arg)
{
	long tmp;

	t_philo *philo = (t_philo *)arg;
	ft_usleep(philo->time_to_die);
	if ((tmp = getTime()) - philo->life >= philo->time_to_die)
	{
		((t_scene *)philo->scene)->finished = 1;
		printf("%ld : %d is dead (rip)\n", tmp - ((t_scene *)philo->scene)->begin, philo->index);
	}
	pthread_exit(NULL);
}

void *boucle(void *arg)
{
	t_philo *philo = ((t_philo *)arg);
	long				begin;

	begin = ((t_scene *)philo->scene)->begin;
	while (!((t_scene *)philo->scene)->finished)
	{
		pthread_mutex_lock(&philo->myFork);
		pthread_mutex_lock(&philo->otherFork->myFork);
		pthread_create(&philo->checker, NULL, checkLife, philo);
		if (((t_scene *)philo->scene)->finished)
		{
			pthread_mutex_unlock(&philo->myFork);
			pthread_mutex_unlock(&philo->otherFork->myFork);
			break ;
		}
		printf("%ld : %d is eating\n", getTime() - begin, philo->index);
		philo->life = getTime();
		ft_usleep(philo->time_to_eat);
		pthread_mutex_unlock(&philo->otherFork->myFork);
		pthread_mutex_unlock(&philo->myFork);
		if (((t_scene *)philo->scene)->finished)
			break ;
		printf("%ld : %d is sleeping\n", getTime() - begin, philo->index);
		ft_usleep(philo->time_to_sleep);
	}
	pthread_exit(NULL);
}

int main()
{
	t_scene scene;
	scene.tab = NULL;
	int num_philo = 3;
	scene.num_philo = num_philo;
	scene.time_to_sleep = 100;
	scene.time_to_eat = 200;
	scene.time_to_die = 610;
	scene.n_eat = -1;
	scene.finished = 0;


	if (!(scene.tab = malloc(sizeof(t_philo) * scene.num_philo)))
		return (1);
	int i = 0;
	while (i < num_philo)
	{
		(scene.tab[i]).index = i + 1;
		scene.tab[i].time_to_die = scene.time_to_die;
		scene.tab[i].time_to_eat = scene.time_to_eat;
		scene.tab[i].time_to_sleep = scene.time_to_sleep;
		scene.tab[i].n_eat = scene.n_eat;
		pthread_mutex_init(&scene.tab[i].myFork, NULL);
		scene.tab[i].scene = &scene;
		i++;
	}

	for (int k = 0; k < scene.num_philo - 1; k++)
	{
		scene.tab[k].otherFork = &scene.tab[k + 1];
	}
	scene.tab[scene.num_philo - 1].otherFork = &scene.tab[0];

	scene.begin = getTime();
	for (int j = 0; j < scene.num_philo; j++)
	{
		scene.tab[j].life = scene.begin;
		pthread_create(&scene.tab[j].thread, NULL, boucle, &scene.tab[j]);
	}
	for (int k = 0; k < scene.num_philo; k++)
	{
		pthread_join(scene.tab[k].thread, NULL);
		return (0);
	}
}
