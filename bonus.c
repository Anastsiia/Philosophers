/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctifany <ctifany@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:38:50 by ctifany           #+#    #+#             */
/*   Updated: 2022/02/05 10:57:46 by ctifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	ft_atoi(char *s)
{
	ssize_t	num;

	num = 0;
	while (*s == '+')
		s++;
	while (*s >= '0' && *s <= '9')
	{
		num *= 10;
		num += *s - '0';
		if (num > 2147483647)
			return (-2);
		s++;
	}
	if (*s)
		return (-2);
	if (num == 0)
		return (-2);
	return (num);
}

int	ft_check_args(t_game *game)
{
	if (game->philo_num == -2 || game->die == -2 || \
		game->eat == -2 || game->sleep == -2 || \
		game->time_to_eat == -2)
	{
		printf("All arguments must be positive ints.\n");
		return (1);
	}
	if (game->philo_num > 200)
	{
		printf("Too many philosoohers, try 200 or less.\n");
		return (1);
	}
	return (0);
}

int	ft_get(int argc, char **argv, t_game *game)
{
	if (argc != 5 && argc != 6)
	{
		printf("Bad arguments.\n");
		return (1);
	}
	game->philo_num = ft_atoi(argv[1]);
	game->die = ft_atoi(argv[2]);
	game->eat = ft_atoi(argv[3]);
	game->sleep = ft_atoi(argv[4]);
	if (argv[5])
		game->time_to_eat = ft_atoi(argv[5]);
	else
		game->time_to_eat = -1;
	if (check_args(game))
		return (1);
	game->full_philos = 0;
	return (0);
}

void	ft_close_s(t_game *game)
{
	if (game->write != SEM_FAILED)
	{
		sem_close(game->write);
		sem_unlink("p_write");
	}
	if (game->forks != SEM_FAILED)
	{
		sem_close(game->forks);
		sem_unlink("p_forks");
	}
	if (game->kill != SEM_FAILED)
	{
		sem_close(game->kill);
		sem_unlink("p_kill");
	}
}

void	ft_init_s(t_game *game)
{
	sem_unlink("p_write");
	game->write = sem_open("p_write", O_CREAT | O_EXCL, 0664, 1);
	if (game->write == SEM_FAILED)
	{
		close_semaphores(game);
		exit(1);
	}
	sem_unlink("p_kill");
	game->kill = sem_open("p_kill", O_CREAT | O_EXCL, 0664, 0);
	if (game->kill == SEM_FAILED)
	{
		close_semaphores(game);
		exit(1);
	}
	sem_unlink("p_forks");
	game->forks = sem_open("p_forks", O_CREAT | O_EXCL, 0664, \
		game->philo_num);
	if (game->kill == SEM_FAILED)
	{
		close_semaphores(game);
		exit(1);
	}
}

unsigned long	ft_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int	ft_msg(int msg, t_philo *philo)
{
	if (msg == 3)
	{
		printf("%ld %d is sleeping\n", (unsigned long)(ft_time() - \
			philo->game->time_start), philo->num);
		sem_post(philo->game->write);
		return (0);
	}
	if (msg == 4)
	{
		printf("%ld %d is thinking\n", (unsigned long)(ft_time() - \
			philo->game->time_start), philo->num);
		sem_post(philo->game->write);
		return (0);
	}
	if (msg == 0)
	{
		printf("%ld %d is dead\n", (unsigned long)(ft_time() - \
			philo->game->time_start), philo->num);
		return (0);
	}
	return (0);
}

int	ft_message(int msg, t_philo *philo)
{
	sem_wait(philo->game->write);
	if (philo->death && msg != 0)
		return (1);
	if (msg == 1)
	{
		printf("%ld %d has taken a fork\n", (unsigned long)(ft_time() - philo->game->time_start), philo->num);
		sem_post(philo->game->write);
		return (0);
	}
	if (msg == 2)
	{
		printf("%ld %d is eating\n", (unsigned long)(ft_time() - \
			philo->game->time_start), philo->num);
		sem_post(philo->game->write);
		return (0);
	}
	return (ft_msg(msg, philo));
}

void	*ft_check_live(t_philo *philo)
{
	unsigned long	time;

	while (1)
	{
		time = ft_time();
		if (time - philo->last_meal_time >= (unsigned long)philo->game->die)
		{
			philo->death = 1;
			sem_post(philo->game->kill);
			ft_message(0, philo);
			return (NULL);
		}
		usleep(1000);
	}
	return (NULL);
}

void	ft_usleep(int	wait)
{
	unsigned long	time;

	time = ft_time();
	while (ft_time() < time + (unsigned long)wait)
		usleep(100);
}

void	ft_live_loop(t_philo *philo)
{
	while (philo->meal != philo->game->time_to_eat)
	{
		sem_wait(philo->game->forks);
		if (ft_message(1, philo))
			exit(0);
		sem_wait(philo->game->forks);
		if (ft_message(1, philo))
			exit(0);
		if (ft_message(2, philo))
			exit(0);
		ft_usleep(philo->game->eat);
		philo->last_meal_time = ft_time();
		philo->meal++;
		sem_post(philo->game->forks);
		sem_post(philo->game->forks);
		if (philo->meal == philo->game->time_to_eat)
			exit(0);
		if (ft_message(3, philo))
			exit(0);
		user_usleep(philo->game->sleep);
		if (ft_message(4, philo))
			exit(0);
	}
}

void	*ft_live(t_game *game, int i)
{
	t_philo	philo;

	philo.num = i + 1;
	philo.game = game;
	philo.meal = 0;
	philo.death = 0;
	philo.last_meal_time = ft_time();
	pthread_create(&philo.thread, NULL, (void *)&ft_check_live, &philo);
	pthread_detach(philo.thread);
	ft_live_loop(&philo);
	exit(0);
}

void	ft_fork(t_game *game, int *pid)
{
	int	i;

	i = 0;
	while (i < game->philo_num)
	{
		pid[i] = fork();
		if (pid[i] < 0)
		{
			perror("Error");
			while (i >= 0)
			{
				kill(pid[i], SIGKILL);
				i--;
			}
			exit(1);
		}
		if (pid[i] == 0)
			ft_live(game, i);
		i++;
	}
}

void	ft_kill_pid(t_game *game, int kill_pid, int i, int *pid)
{
	if (kill_pid == 0)
	{
		sem_wait(game->kill);
		while (i < game->philo_num)
		{
			kill(pid[i], SIGKILL);
			i++;
		}
		exit(0);
	}
}

void	ft_create_p(t_game *game)
{
	int	i;
	int	*pid;
	int	kill_pid;

	i = 0;
	pid = malloc(sizeof(int) * game->philo_num);
	if (!pid)
		exit(1);
	ft_fork(game, pid);
	kill_pid = fork();
	ft_kill_pid(game, kill_pid, i, pid);
	while (i < game->philo_num)
	{
		waitpid(pid[i], NULL, 0);
		i++;
	}
	kill(kill_pid, SIGKILL);
	free(pid);
}

void	ft_close_s(t_game *game)
{
	if (game->write != SEM_FAILED)
	{
		sem_close(game->write);
		sem_unlink("p_write");
	}
	if (game->forks != SEM_FAILED)
	{
		sem_close(game->forks);
		sem_unlink("p_forks");
	}
	if (game->kill != SEM_FAILED)
	{
		sem_close(game->kill);
		sem_unlink("p_kill");
	}
}

int	main(int argc, char **argv)
{
	t_game	game;

	if (ft_get(argc, argv, &game))
		return (1);
	ft_init_s(&game);
	game.time_start = ft_time();
	ft_create_p(&game);
	ft_close_s(&game);
	return (0);
}
