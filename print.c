/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppoinsin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 20:23:20 by ppoinsin          #+#    #+#             */
/*   Updated: 2022/01/21 20:35:10 by ppoinsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosopher.h"

static long	ft_pow(int n, int power)
{
	long	result;

	if (!power)
		return (1);
	else if (power < 0)
		return (0);
	result = 1;
	while (power--)
	{
		result *= n;
	}
	return (result);
}

char	*ft_itoa_2(int n, int taille)
{
	char	*result;

	result = malloc(taille * sizeof(char));
	if (!result)
		return (NULL);
	result[0] = '-';
	if (n == 0)
		result[0] = '0';
	return (result);
}

char	*ft_itoa(int n)
{
	int		taille;
	int		count;
	char	*result;
	long	num;

	count = 0;
	taille = 1;
	num = (long)n;
	if (n < 0)
		num = -((long)n);
	while (num / ft_pow(10, count++) >= 1)
		taille++;
	if (n <= 0)
		taille++;
	result = ft_itoa_2(n, taille);
	if (!result)
		return (NULL);
	result[0] = '-';
	count = -1;
	if (n < 0)
		count = 0;
	while (++count < taille - 1)
		result[count] = (num / (ft_pow(10, taille - 2 - count)) % 10) + 48;
	result[taille - 1] = 0;
	return (result);
}

void	print_eat(t_elem *app, int id)
{
	char	*time;
	char	*result;
	char	*tmp;

	time = ft_itoa(get_time(app->init));
	result = ft_strjoin(ft_itoa(get_time(app->init)), " ms ");
	tmp = ft_itoa(id);
	result = ft_strjoin(result, tmp);
	free(tmp);
	result = ft_strjoin(result, " has taken a fork\n");
	tmp = ft_itoa(get_time(app->init));
	result = ft_strjoin(result, tmp);
	free(tmp);
	result = ft_strjoin(result, " ms ");
	tmp = ft_itoa(id);
	result = ft_strjoin(result, tmp);
	free(tmp);
	result = ft_strjoin(result, " is eating\n");
	write(1, result, ft_strlen(result));
	free(result);
	free(time);
}

void	print_sleep(t_elem *app, int id)
{
	char	*time;
	char	*result;
	char	*tmp;

	time = ft_itoa(get_time(app->init));
	tmp = ft_itoa(id);
	result = ft_strjoin(ft_itoa(get_time(app->init)), " ms ");
	result = ft_strjoin(result, tmp);
	free(tmp);
	result = ft_strjoin(result, " is sleeping\n");
	write(1, result, ft_strlen(result));
	free(result);
	free(time);
}
