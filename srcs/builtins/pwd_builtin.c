/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:13:48 by zlayine           #+#    #+#             */
/*   Updated: 2020/10/30 14:41:47 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/shell.h"

int		pwd_builtin(void)
{
	char	*buff;

	if ((buff = getcwd(NULL, 0)) == NULL)
		print_error("pwd", errno, 0);
	else
	{
		ft_putstr_fd(buff, 1);
		ft_putstr_fd("\n", 1);
		free(buff);
	}
	return (0);
}
