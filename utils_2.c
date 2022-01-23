/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppoinsin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 20:21:03 by ppoinsin          #+#    #+#             */
/*   Updated: 2022/01/21 20:23:06 by ppoinsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

size_t	ft_strlen(const char *s)
{
	unsigned int	count;

	count = 0;
	while (*s++)
		count++;
	return (count);
}

size_t	ft_strlcpy(char *dst, const char *src, size_t size)
{
	unsigned int	count;

	if (!size)
		return (ft_strlen(src));
	count = 0;
	while (count + 1 < size && src[count])
	{
		dst[count] = src[count];
		count++;
	}
	dst[count] = 0;
	return (ft_strlen(src));
}

char	*ft_strjoin(char *s1, const char *s2)
{
	int		taille;
	char	*result;

	taille = ft_strlen(s1) + ft_strlen(s2);
	result = malloc(taille * (sizeof(char) + 1));
	if (!result)
		return (NULL);
	ft_strlcpy(result, (char *)s1, taille + 1);
	ft_strlcpy(&result[ft_strlen(s1)], (char *)s2, taille + 1);
	if (s1)
		free(s1);
	return (result);
}

int	give_next_index(t_elem *app, int id)
{
	if (id == app->num_philo)
		return (0);
	return (id);
}

char	read_finish(t_elem *app)
{
	char	result;

	pthread_mutex_lock(&app->mutex_app_finish);
	result = app->finish;
	pthread_mutex_unlock(&app->mutex_app_finish);
	return (result);
}

void	set_finish(t_elem *app, char value)
{
	pthread_mutex_lock(&app->mutex_app_finish);
	app->finish = value;
	pthread_mutex_unlock(&app->mutex_app_finish);
}

char	read_ready_state(t_elem *app, int index)
{
	char	result;

	pthread_mutex_lock(&app->mutex_ready);
	result = app->ready_tab[index];
	pthread_mutex_unlock(&app->mutex_ready);
	return (result);
}

void	set_ready_state(t_elem *app, int index, char value)
{
	pthread_mutex_lock(&app->mutex_ready);
	app->ready_tab[index] = value;
	pthread_mutex_unlock(&app->mutex_ready);
}

char	read_finish_tab(t_elem *app, int index)
{
	char	result;
	pthread_mutex_lock(&app->mutex_finish_tab[index]);
	result = app->finish_tab[index];
	pthread_mutex_unlock(&app->mutex_finish_tab[index]);
	return (result);
}

void	set_finish_tab(t_elem *app, int index, char value)
{
	pthread_mutex_lock(&app->mutex_finish_tab[index]);
	app->finish_tab[index] = value;
	pthread_mutex_unlock(&app->mutex_finish_tab[index]);
}