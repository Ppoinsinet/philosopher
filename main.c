#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

typedef struct s_elem
{
    pthread_mutex_t *mutex_tab;
    pthread_t *philo_tab;
    struct timeval *init_tab;
    char            *state_tab;
    char            *forks_tab;
    pthread_t       io_thread;
    pthread_t check_thread;
    int size;
    int num_philo;
    int time_to_die;
    int time_to_eat;
    int time_to_sleep;
    int num_eat;
    char finish;
    struct timeval init;
} t_elem;

typedef struct s_container
{
    t_elem *elem;
    int id;
}   t_container;

void ft_sleep(long int ms)
{
    struct timeval init, tmp;
    long int elapsed;

    gettimeofday(&init, NULL);
    while (1)
    {
        gettimeofday(&tmp, NULL);
        elapsed = (tmp.tv_sec * 1000 + tmp.tv_usec / 1000) - (init.tv_sec * 1000 + init.tv_usec / 1000);
        if (elapsed >= ms)
            return;
        usleep(1);
    }
}

long int getTime(struct timeval init)
{
    struct timeval tmp;

    gettimeofday(&tmp, NULL);
    return ((tmp.tv_sec * 1000 + tmp.tv_usec / 1000) - (init.tv_sec * 1000 + init.tv_usec / 1000));
}

int take_fork(t_elem *app, int id, int index_tab)
{
    pthread_mutex_lock(&app->mutex_tab[index_tab]);
    //app->forks_tab[index_tab] = 1;
    gettimeofday(&app->init_tab[id - 1], NULL);
    app->state_tab[id - 1] = index_tab + 1;
    if (app->finish)
    {
        pthread_mutex_unlock(&app->mutex_tab[index_tab]);
        return (-1);
    }
    //gettimeofday(&app->init_tab[id - 1], NULL);
    ft_sleep(app->time_to_eat);
    //app->forks_tab[index_tab] = 0;
    pthread_mutex_unlock(&app->mutex_tab[index_tab]);
    //printf("%ld ms id %d unlock %d\n", getTime(app->init) ,id, index_tab);
    return (0);
}

void *loop(void *arg)
{
    t_elem *app;
    int id;
    int n_eat;
    int index_tmp;

    app = ((t_container *) arg)->elem;
    id = ((t_container *) arg)->id;
    index_tmp = (id - 1) / 2;
    n_eat = 0;
    while ((app->num_eat < 0 || n_eat < app->num_eat) && !app->finish)
    {
        if (take_fork(app, id, index_tmp) < 0)
            break;
        app->state_tab[id - 1] = -1;
        if (id == app->num_philo && app->num_philo % 2)
        {
            if (!index_tmp)
                index_tmp = (app->num_philo / 2) - 1;
            else
                index_tmp = 0;
        }
        n_eat++;
        ft_sleep(app->time_to_sleep);
    }
    pthread_exit(NULL);
}

int ft_atoi(const char *s)
{
    int index;
    int result;
    int signe;

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

int init_app(t_elem *app, int ac, const char **av)
{
    app->num_philo = ft_atoi(av[1]);
    if (app->num_philo < 1)
        return (-1);
    app->size = app->num_philo;
    app->time_to_die = ft_atoi(av[2]);
    app->time_to_eat = ft_atoi(av[3]);
    app->time_to_sleep = ft_atoi(av[4]);
    app->num_eat = -1;
    if (ac == 6)
        app->num_eat = ft_atoi(av[5]);
    app->finish = 0;
    app->philo_tab = malloc(sizeof(pthread_t) * app->num_philo);
    if (!app->philo_tab)
        return (-1);
    app->mutex_tab = malloc(sizeof(pthread_mutex_t) * app->size);
    app->forks_tab = malloc(sizeof(char) * app->size);
    if (!app->mutex_tab || !app->forks_tab)
    {
        free(app->philo_tab);
        return (-1);
    }
    return (0);
}

void    *check_philo_die(void *arg)
{
    t_elem  *app;
    int count;
    long int elapsed;

    app = (t_elem *) arg;
    count = 0;
    while (count < app->num_philo)
    {
        elapsed = getTime(app->init_tab[count]);
        if (elapsed >= app->time_to_die && app->state_tab[count] <= 0)
            {
                app->finish = 1;
                printf("%ld ms %d died : %d\n", getTime(app->init), count + 1, app->state_tab[count]);
                pthread_exit(NULL);
            }
        if (count == app->num_philo - 1)
            count = 0;
        else
            count++;
    }
    return NULL;
}

void *loop_io(void *arg)
{
    t_elem *app;
    long int elapsed;

    app = (t_elem *) arg;
    int count = 0;
    while (count < app->num_philo && !app->finish)
    {
        elapsed = getTime(app->init);
        if (!app->finish && app->state_tab[count] > 0)
        {
            gettimeofday(&app->init_tab[count], NULL);
            printf("%ld ms %d has taken a fork\n%ld ms %d is eating : %d\n", elapsed, count + 1, elapsed, count + 1, app->state_tab[count] - 1);
        } else if (!app->finish && app->state_tab[count] < 0)
        {
            printf("%ld ms %d is sleeping\n", elapsed, count + 1);
        }
        app->state_tab[count] = 0;
        count++;
        if (count >= app->num_philo)
            count = 0;
    }
    return (NULL);
}

int main(int ac, const char **av)
{
    t_elem app;
    int count;

    if (ac < 5 || ac > 6)
        return (1);
    if (init_app(&app, ac, av) < 0)
        return (1);
    count = 0;
    while (count < app.size)
        pthread_mutex_init(&app.mutex_tab[count++], NULL);

    t_container *test;
    test = malloc(sizeof(t_container) * app.num_philo);
    if (!test)
    {
        return (1);
    }
    app.init_tab = malloc(sizeof(struct timeval) * app.num_philo);
    if (!app.init_tab)
        return (1);
    app.state_tab = malloc(sizeof(char) * app.num_philo);
    if (!app.state_tab)
        return (1);
    gettimeofday(&app.init, NULL);
    count = 0;
    pthread_create(&app.io_thread, NULL, loop_io, &app);
    for (int k = 0; k < app.num_philo; k++) {
        app.forks_tab[k] = 0;
        app.state_tab[k] = 0;
        app.init_tab[k] = app.init;
        test[k].id = k + 1;
        test[k].elem = &app;
    }
    while (count < app.num_philo)
    {
        pthread_create(&app.philo_tab[count], NULL, loop, &test[count]);
        count++;
    }
    pthread_create(&app.check_thread, NULL, check_philo_die, &app);
    count = 0;
    while (count < app.num_philo)
        pthread_join(app.philo_tab[count++], NULL);
    pthread_join(app.check_thread, NULL);
    free(app.mutex_tab);
    free(app.philo_tab);
    free(app.init_tab);
    free(test);
}