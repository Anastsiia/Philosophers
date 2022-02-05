/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ctifany <ctifany@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/05 10:37:37 by ctifany           #+#    #+#             */
/*   Updated: 2022/02/05 15:44:23 by ctifany          ###   ########.fr       */
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

int				ft_atoi(char *s);
int				ft_check_args(t_game *game);
int				ft_msg(int msg, t_philo *philo);
int				ft_message(int msg, t_philo *philo);
unsigned long	ft_time(void);


void			ft_create_p(t_game *game);
void			ft_kill_pid(t_game *game, int kill_pid, int i, int *pid);
void			ft_fork(t_game *game, int *pid);
void			ft_init_s(t_game *game);
void			ft_close_s(t_game *game);

void			ft_live_loop(t_philo *philo);
void			*ft_live(t_game *game, int i);
void			*ft_check_live(t_philo *philo);

int				ft_get(int argc, char **argv, t_game *game);
void			ft_usleep(int wait);

#endif

//take 1
//eat 2
//sleep 3
//thinking 4
//dead 0
