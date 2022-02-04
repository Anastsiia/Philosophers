/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctifany <ctifany@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/22 14:42:44 by ctifany           #+#    #+#             */
/*   Updated: 2022/01/24 16:12:03 by ctifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_atoi(char *str)
{
	int		i;
	long	result;

	i = 0;
	result = 0;
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + str[i] - 48;
		i++;
	}
	if (str[i] != '\0' || result > 2147483647 || result == 0)
		result = -1;
	return ((int)result);
}

unsigned long	ft_now_time(void)
{
	struct timeval	timeval;

	gettimeofday(&timeval, NULL);
	return (timeval.tv_sec * 1000 + timeval.tv_usec / 1000);
}

void	ft_init_philo(t_game *game, int count)
{
	game->philo[count].num = count + 1;
	game->philo[count].l_fork = count;
	if (count == game->philo_num - 1)
		game->philo[count].r_fork = 0;
	else
		game->philo[count].r_fork = count + 1;
	game->philo[count].game = game;
	game->philo[count].meal = 0;
	game->philo[count].last_meal_time = ft_now_time();
}

int ft_init (t_game *game)
{
	int count;

	count = 0;
	game->philo = malloc(sizeof(t_philo) * game->philo_num);
	if (game->philo == NULL)
		return (1);
	game->fork = malloc(sizeof(pthread_mutex_t) * game->philo_num);
	if (game->fork == NULL)
	{
		free(game->philo);
		return (1);
	}
	while (count < game->philo_num)
	{
		pthread_mutex_init(&game->fork[count], NULL);
		ft_init_philo(game, count);
		count++;
	}
	return (0);
}

int	ft_check_args (int argc, char **argv, t_game *game)
{
	if (argc != 5 && argc != 6)
		return (1);
	game->philo_num = ft_atoi(argv[1]);
	game->die = ft_atoi(argv[2]);
	game->eat = ft_atoi(argv[3]);
	game->sleep = ft_atoi(argv[4]);
	if (argv[5])
		game->time_to_eat = ft_atoi(argv[5]);
	else
		game->time_to_eat = -2;
	if (game->philo_num == -1 ||  game->die == -1 || game->eat == -1 || game->sleep == -1 || game->time_to_eat == -1)
		return (1);
	return (0);		
}

int	ft_print2(t_philo *philo, int msg)
{
	if (msg == 3)
	{
		printf("%ld ms %d p is sleeping\n", (unsigned long)(ft_now_time() - \
			philo->game->time_start), philo->num);
		return (0);
	}
	if (msg == 4)
	{
		printf("%ld ms %d p is thinking\n", (unsigned long)(ft_now_time() - \
			philo->game->time_start), philo->num);
		return (0);
	}
	if (msg == 0)
	{
		printf("%ld ms %d p is dead\n", (unsigned long)(ft_now_time() - \
			philo->game->time_start), philo->num);
		return (0);
	}
	return (0);
}

int ft_print (t_philo *philo, int msg)
{
	if (philo->game->dead && msg != 0)
		return (1);
	if (msg == 1)
	{
		printf("%ld ms %d p take a fork\n", (unsigned long)(ft_now_time() - \
			philo->game->time_start), philo->num);
		return (0);
	}
	if (msg == 2)
	{
		printf("%ld ms %d p is eating\n", (unsigned long)(ft_now_time() - \
			philo->game->time_start), philo->num);
		return (0);
	}
	return (ft_print2(philo, msg));
}

void	ft_wait(int	till_time)
{
	unsigned long	time;

	time = ft_now_time();
	while (ft_now_time() < (unsigned long)till_time + time)
		usleep(100);
}

int	ft_live(t_philo *philo)
{
	if (philo->game->philo_num == 1)
		return (1);
	pthread_mutex_lock(&philo->game->fork[philo->r_fork]);
	if (ft_print(philo, 1))
	{
		pthread_mutex_unlock(&philo->game->fork[philo->l_fork]);
		pthread_mutex_unlock(&philo->game->fork[philo->r_fork]);
		return (1);
	}
	if (ft_print(philo, 2))
	{
		pthread_mutex_unlock(&philo->game->fork[philo->l_fork]);
		pthread_mutex_unlock(&philo->game->fork[philo->r_fork]);
		return (1);
	}
	ft_wait(philo->game->eat);
	philo->last_meal_time = ft_now_time();
	philo->meal++;
	pthread_mutex_unlock(&philo->game->fork[philo->l_fork]);
	pthread_mutex_unlock(&philo->game->fork[philo->r_fork]);
	return (0);
}

void	*ft_philo_live(t_philo *philo)
{
	while (philo->meal != philo->game->time_to_eat && philo->game->dead == 0)
	{
		pthread_mutex_lock(&philo->game->fork[philo->l_fork]);
		if (ft_print(philo, 1))
		{
			pthread_mutex_unlock(&philo->game->fork[philo->l_fork]);
			return (NULL);
		}
		if (ft_live(philo))
			return (NULL);
		if (philo->meal == philo->game->time_to_eat)
			break ;
		if (ft_print(philo, 3))
			return (NULL);
		ft_wait(philo->game->sleep);
		if (ft_print(philo, 4))
			return (NULL);
	}
	philo->game->full_philos++;
	return (NULL);
}

void ft_pthread_create (t_game *game)
{
	int	count;

	count = 0;
	while (count < game->philo_num)
	{
		pthread_create(&game->philo[count].thread, NULL, (void *)&ft_philo_live, \
			&game->philo[count]);
		count += 2;
	}
	usleep(1000);
	count = 1;
	while (count < game->philo_num)
	{
		pthread_create(&game->philo[count].thread, NULL, (void *)&ft_philo_live, \
			&game->philo[count]);
		count += 2;
	}
}

int	ft_check_status(t_game *game)
{
	int				i;
	unsigned long	time;

	while (1)
	{
		i = 0;
		while (i < game->philo_num)
		{
			if (game->full_philos == game->philo_num)
				return (3);
			time = ft_now_time();
			if (time - game->philo[i].last_meal_time >= (unsigned long)game->die)
			{
				game->dead = 1;
				ft_print(&game->philo[i], 0);
				return (2);
			}
			i++;
		}
		usleep(1000);
	}
	return (1);
}

void	ft_join(t_game *game)
{
	int	i;

	i = 0;
	while (i < game->philo_num)
	{
		pthread_join(game->philo[i].thread, NULL);
		i++;
	}
	i = 0;
	while (i < game->philo_num)
	{
		pthread_mutex_destroy(&game->fork[i]);
		i++;
	}
}

int	main (int argc, char **argv)
{
	t_game	game;
	int		status;

	if (ft_check_args(argc, argv, &game) == 1)
	{
		printf("Bad arguments.\n");
		return(1);
	}
	game.full_philos = 0;
	game.dead = 0;
	if (ft_init(&game) == 1)
		return (1);
	game.time_start = ft_now_time();
	ft_pthread_create(&game);
	status = ft_check_status(&game);
	ft_join(&game);
	free(game.fork);
	free(game.philo);
	return (status);
}
