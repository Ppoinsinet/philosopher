/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppoinsin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:23:17 by ppoinsin          #+#    #+#             */
/*   Updated: 2022/01/21 20:19:25 by ppoinsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	take_fork(t_elem *app, int id, int next)
{
	int	tmp;

	//printf("salut1 %d %d\n", id, read_ready_state(app, id - 1));
	while (!read_ready_state(app, id - 1))
		usleep(1);
	//printf("salut2 %d %d\n", id, read_ready_state(app, id - 1));
	if (id == app->num_philo)
		next = 0;
	//printf("id %d lock %d et %d\n", id, id-1, next);
	if (id - 1 < next)
	{
		pthread_mutex_lock(&app->mutex_tab[id - 1]);
		pthread_mutex_lock(&app->mutex_tab[next]);
	}
	else 
	{
		pthread_mutex_lock(&app->mutex_tab[next]);
		pthread_mutex_lock(&app->mutex_tab[id - 1]);
	}


	gettimeofday(&app->init_tab[id - 1], NULL);
	pthread_mutex_lock(&app->mutex_output);
	if (app->output)
		print_eat(app, id);
	pthread_mutex_unlock(&app->mutex_output);
	ft_sleep(app->time_to_eat);

	tmp = give_next_thread_alive(app, id);
	if (tmp < 0)
		set_finish(app, 1);
	else
	{
		set_ready_state(app, tmp - 1, 1);
		//app->forks_tab[tmp - 1] = 1;
	}


	pthread_mutex_unlock(&app->mutex_tab[next]);
	pthread_mutex_unlock(&app->mutex_tab[id - 1]);
	return (0);
}

void	*loop(void *arg)
{
	t_elem		*app;
	int			id;
	int			n_eat;

	app = ((t_container *) arg)->elem;
	id = ((t_container *) arg)->id;
	n_eat = 0;
	if (app->num_philo == 1)
		set_finish(app, 1);
	if (app->num_philo == 1)
		printf("%ld ms %d died\n", get_time(app->init), id);
	while (!read_finish(app) && !take_fork(app, id, id))
	{
		if (app->num_eat > 0 && ++n_eat >= app->num_eat)
		{
			set_finish_tab(app, id - 1, 1);
			pthread_exit(NULL);
		}
		pthread_mutex_lock(&app->mutex_output);
		if (app->output)
			print_sleep(app, id);
		pthread_mutex_unlock(&app->mutex_output);
		ft_sleep(app->time_to_sleep);
	}
	pthread_exit(NULL);
}

int	check_elapsed_time(t_elem *app, int count, int finish_value)
{
	if (get_time(app->init_tab[count]) >= app->time_to_die
		&& !finish_value)
	{
		set_finish(app, (char)printf("%ld ms %d died\n", get_time(app->init), count + 1));
		pthread_mutex_lock(&app->mutex_output);
		app->output = 0;
		pthread_mutex_unlock(&app->mutex_output);


		for (int k = 0; k < app->num_philo; k++)
		{
			printf("salut %d \n", k);
			pthread_detach(app->philo_tab[k]);
		}
		write(1, "ciao\n", 5);
		pthread_exit(NULL);
	}
	return (0);
}

void	*check_philo_die(void *arg)
{
	t_elem	*app;
	int		count;
	int		finish_count;

	app = (t_elem *) arg;
	count = -1;
	finish_count = 0;
	while (++count < app->num_philo)
	{
		int tmp = read_finish_tab(app, count);
		finish_count += tmp;
		check_elapsed_time(app, count, tmp);
		if (count >= app->num_philo - 1)
		{
			if (finish_count >= app->num_philo - 1)
			{
				set_finish(app, 1);
				pthread_exit(NULL);
			}
			count = -1;
			finish_count = 0;
		}
	}
	pthread_exit(NULL);
}

int	main(int ac, const char **av)
{
	t_container	*tmp;
	t_elem		app;
	int			count;

	if (ac < 5 || ac > 6 || init_app(&app, ac, av) < 0)
		return (1);
	count = 0;
	while (count < app.num_philo)
		pthread_mutex_init(&app.mutex_tab[count++], NULL);
	pthread_mutex_init(&app.mutex_app_finish, NULL);
	pthread_mutex_init(&app.mutex_ready, NULL);
	pthread_mutex_init(&app.mutex_output, NULL);
	app.output = 1;
	count = 0;
	while (count < app.num_philo)
		pthread_mutex_init(&app.mutex_finish_tab[count++], NULL);
	tmp = malloc(sizeof(t_container) * app.num_philo);
	if (!tmp)
	{
		free_all(&app);
		return (1);
	}
	gettimeofday(&app.init, NULL);
	thread_creation(&app, tmp);
	on_destroy(&app, tmp);
	return (0);
}
