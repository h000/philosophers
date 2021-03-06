/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hpark <hpark@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/12 13:47:25 by hpark             #+#    #+#             */
/*   Updated: 2020/09/26 17:06:57 by hpark            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	clean_shm(void)
{
	sem_unlink("/fork");
	sem_unlink("/pickup");
	sem_unlink("/print");
	sem_unlink("/someone_died");
}

t_vars	*get_vars(void)
{
	static t_vars vars;

	return (&vars);
}

int		free_vars(t_vars *vars)
{
	(void)vars;
	sem_close(vars->fork);
	sem_close(vars->pickup);
	sem_close(vars->print);
	sem_close(vars->someone_died);
	clean_shm();
	return (0);
}

int		init_vars(t_vars *vars, int argc, char **argv)
{
	clean_shm();
	vars->n_philo = ft_atoi(argv[1]);
	vars->t_die = ft_atoi(argv[2]);
	vars->t_eat = ft_atoi(argv[3]);
	vars->t_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		vars->n_must_eat = ft_atoi(argv[5]);
	else
		vars->n_must_eat = -1;
	vars->fork = sem_open("/fork", O_CREAT, 0660, vars->n_philo);
	if (vars->fork == SEM_FAILED)
		return (1);
	if ((vars->print = sem_open("/print", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (1);
	if ((vars->pickup = sem_open("/pickup", O_CREAT, 0660, 1)) == SEM_FAILED)
		return (1);
	vars->someone_died = sem_open("/someone_died", O_CREAT, 0660, 1);
	if (vars->someone_died == SEM_FAILED)
		return (1);
	vars->n_done = 0;
	vars->died = 0;
	return (0);
}

int		main(int argc, char **argv)
{
	t_vars	*vars;

	if (argc != 5 && argc != 6)
		return (ft_error("Error: Wrong Number of Arguments\n"));
	vars = get_vars();
	if (init_vars(vars, argc, argv))
		return (ft_error("Error: sem_open\n"));
	if (create_philo(vars))
		return (free_vars(vars));
	clean_shm();
	return (0);
}
