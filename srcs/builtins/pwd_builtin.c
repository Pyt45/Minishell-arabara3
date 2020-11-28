/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_builtin.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aaqlzim <aaqlzim@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/29 19:13:48 by zlayine           #+#    #+#             */
/*   Updated: 2020/11/28 18:52:10 by aaqlzim          ###   ########.fr       */
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
		ft_del(buff);
	}
	return (0);
}
