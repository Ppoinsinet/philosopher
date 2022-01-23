/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppoinsin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:58:55 by ppoinsin          #+#    #+#             */
/*   Updated: 2022/01/21 20:36:29 by ppoinsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

void	thread_creation(t_elem *app, t_container *tmp)
{
	int	count;

	count = 0;
	while (count < app->num_philo)
	{
		if (!(app->num_philo % 2 && (count + 1) == app->num_philo)
			|| app->num_philo == 1)
			app->ready_tab[count] = (count + 1) % 2;
		else
			app->ready_tab[count] = 0;
		app->init_tab[count] = app->init;
		tmp[count].id = count + 1;
		tmp[count].elem = app;
		set_finish_tab(app, count, 0);
		count++;
	}
	count = -1;
	pthread_create(&app->check_thread, NULL, check_philo_die, app);
	while (++count < app->num_philo)
		pthread_create(&app->philo_tab[count], NULL, loop, &tmp[count]);
}

int	give_next_thread_alive(t_elem *app, int id)
{
	int	count;

	count = id - 1;
	if (count <= 0)
		count = app->num_philo;
	while (1)
	{
		if (count == id)
		{
			if (read_finish_tab(app, count - 1))
				return (count);
			return (-1);
		}
		if (!read_finish_tab(app, count - 1))
			return (count);
		if (count <= 1)
			count = app->num_philo;
		else
			count--;
	}
	return (-1);
}

void	free_all(t_elem *app)
{
	int	count;

	count = 0;
	while (count < app->num_philo)
	{
		pthread_mutex_destroy(&app->mutex_tab[count++]);
	}
	pthread_mutex_destroy(&app->mutex_app_finish);
	free(app->mutex_tab);
	free(app->philo_tab);
	free(app->ready_tab);
	free(app->init_tab);
	free(app->finish_tab);
}

void	on_destroy(t_elem *app, t_container *tmp)
{
	int	count;

	count = 0;
	while (count < app->num_philo)
	{
		pthread_join(app->philo_tab[count++], NULL);
	}
	pthread_join(app->check_thread, NULL);
	free_all(app);
	free(tmp);
}
