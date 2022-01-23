/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ppoinsin <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/21 14:59:49 by ppoinsin          #+#    #+#             */
/*   Updated: 2022/01/21 18:16:32 by ppoinsin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHER_H
# define PHILOSOPHER_H

# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/time.h>
# include <unistd.h>

typedef struct s_elem
{
	pthread_t		*philo_tab;
	struct timeval	*init_tab;
	char			*ready_tab;
	pthread_mutex_t	mutex_ready;
	char			*forks_tab;
	pthread_mutex_t	*mutex_tab;
	char			*finish_tab;
	pthread_mutex_t	*mutex_finish_tab;
	pthread_mutex_t	mutex_output;
	char			output;
	pthread_t		check_thread;
	int				num_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				num_eat;
	char			finish;
	pthread_mutex_t	mutex_app_finish;
	struct timeval	init;
}			t_elem;

typedef struct s_container
{
	t_elem	*elem;
	int		id;
}		t_container;

typedef struct timeval	t_timeval;

int			ft_atoi(const char *s);
long int	get_time(struct timeval init);
void		ft_sleep(long int ms);
void		print_eat(t_elem *app, int id);
void		print_sleep(t_elem *app, int id);

int			take_fork(t_elem *app, int id, int next);
int			give_next_thread_alive(t_elem *app, int id);

int			init_app(t_elem *app, int ac, const char **av);

void		*check_philo_die(void *arg);
void		*loop_io_eat(void *arg);
void		*loop_io_sleep(void *arg);
void		*loop(void *arg);

void		thread_creation(t_elem *app, t_container *tmp);
void		on_destroy(t_elem *app, t_container *tmp);
void		free_all(t_elem *app);
int			give_next_index(t_elem *app, int id);

size_t	ft_strlen(const char *s);
size_t				ft_strlcpy(char *dst, const char *src, size_t size);
char		*ft_strjoin(char *s1, const char *s2);

char		read_finish(t_elem *app);
void		set_finish(t_elem *app, char value);

void	set_ready_state(t_elem *app, int index, char value);
char	read_ready_state(t_elem *app, int index);

char	read_finish_tab(t_elem *app, int index);
void	set_finish_tab(t_elem *app, int index, char value);

#endif
