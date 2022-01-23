/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppoinsin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:10:19 by ppoinsin          #+#    #+#             */
/*   Updated: 2022/01/21 18:15:34 by ppoinsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

int	ft_atoi(const char *s)
{
	int	index;
	int	result;
	int	signe;

	if (!s)
		return (0);
	index = 0;
	result = 0;
	signe = 1;
	if (s[index] == '-')
	{
		signe *= -1;
		index++;
	}
	while (s[index])
	{
		if (s[index] >= '0' && s[index] <= '9')
			result = result * 10 + s[index] - '0';
		else
			return (signe * result);
		index++;
	}
	return (signe * result);
}

void	ft_sleep(long int ms)
{
	t_timeval	init;

	gettimeofday(&init, NULL);
	while (1)
	{
		if (get_time(init) >= ms)
			return ;
		usleep(1);
	}
}

long	get_time(struct timeval init)
{
	t_timeval	tmp;

	gettimeofday(&tmp, NULL);
	return ((tmp.tv_sec * 1000 + tmp.tv_usec / 1000)
		- (init.tv_sec * 1000 + init.tv_usec / 1000));
}

int	check_error(t_elem *app)
{
	if (!app->philo_tab || !app->mutex_tab || !app->init_tab || !app->finish_tab
		|| !app->ready_tab || !app->mutex_finish_tab)
	{
		if (app->philo_tab)
			free(app->philo_tab);
		if (app->mutex_tab)
			free(app->mutex_tab);
		if (app->init_tab)
			free(app->init_tab);
		if (app->ready_tab)
			free(app->ready_tab);
		if (app->finish_tab)
			free(app->finish_tab);
		if (app->mutex_finish_tab)
			free(app->mutex_finish_tab);
		return (-1);
	}
	return (0);
}

int	init_app(t_elem *app, int ac, const char **av)
{
	app->num_philo = ft_atoi(av[1]);
	if (app->num_philo < 1)
		return (-1);
	app->time_to_die = ft_atoi(av[2]);
	app->time_to_eat = ft_atoi(av[3]);
	app->time_to_sleep = ft_atoi(av[4]);
	app->num_eat = -1;
	if (ac == 6)
		app->num_eat = ft_atoi(av[5]);
	if (app->time_to_die < 0 || app->time_to_eat < 0 || app->time_to_sleep < 0
		|| (ac == 6 && app->num_eat < 0))
		return (-1);
	app->finish = 0;
	app->philo_tab = malloc(sizeof(pthread_t) * app->num_philo);
	app->mutex_tab = malloc(sizeof(pthread_mutex_t) * app->num_philo);
	app->mutex_finish_tab = malloc(sizeof(pthread_mutex_t) * app->num_philo);
	app->init_tab = malloc(sizeof(struct timeval) * app->num_philo);
	app->ready_tab = malloc(sizeof(char) * app->num_philo);
	app->finish_tab = malloc(sizeof(char) * app->num_philo);
	if (!app->mutex_tab || !app->philo_tab)
		return (-1);
	if (check_error(app))
		return (-1);
	return (0);
}
