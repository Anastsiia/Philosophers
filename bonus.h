/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctifany <ctifany@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:37:37 by ctifany           #+#    #+#             */
/*   Updated: 2022/02/05 11:06:03 by ctifany          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <pthread.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <sys/time.h>
# include <semaphore.h>
# include <signal.h>

typedef struct s_philo
{
	pthread_t			thread; //dead;
	int					death;
	int					num; //philo_num
	int					meal; //eat_times
	unsigned long		last_meal_time; //last_eat_time
	struct s_game	    *game;
}						t_philo;

typedef struct s_game
{
	unsigned long	time_start;	//start_time
	int				philo_num;	//num_of_philos;
	int				die; 		//time_to_die
	int				eat;		//time_to_eat
	int				sleep;		//time_to_sleep
	int				time_to_eat;//num_of_times_to_eat
	int				full_philos; //fed_philos;
	struct s_philo	*philo;
	sem_t			*forks;
	sem_t			*write;
	sem_t			*kill;
}					t_game; //t_philo_info;

#endif

//take 1
//eat 2
//sleep 3
//thinking 4
//dead 0
