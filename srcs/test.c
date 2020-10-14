



/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 19:13:51 by zlayine           #+#    #+#             */
/*   Updated: 2020/03/09 16:03:12 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
# include <curses.h>
# include <term.h>
#include <unistd.h>

static int	ft_check(long n, int sign)
{
	if (n < 0 && sign > 0)
		return (-1);
	if (n > 0 && sign < 0)
		return (0);
	return ((int)n);
}

int			ft_atoi(const char *str)
{
	int		i;
	long	n;
	int		sign;

	i = 0;
	n = 0;
	sign = 1;
	while (str[i] == '\t' || str[i] == ' ' || str[i] == '\n' ||
		str[i] == '\v' || str[i] == '\r' || str[i] == '\f')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] != '\0' && str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - 48);
		i++;
	}
	n = n * sign;
	return (ft_check(n, sign));
}

void    get_cursor_pos()
{
    // printf("xrere\e[6n\n");
    // read(STDIN_FILENO, read_buffer, sizeof(read_buffer));
    // int ii = 0;
    // while (read_buffer[ii] < '0' || read_buffer[ii] > '9')
    //     ii++;
    // y = ft_atoi(read_buffer + ii);
    // printf("Y=%d\n", y);
    // while (read_buffer[ii] >= '0' && read_buffer[ii] <= '9')
    //     ii++;
    // x = ft_atoi(read_buffer + ii + 1);
    // printf("X=%d\n", x);
}

int		ft_isprint(int c)
{
	if (c < 32 || c > 126)
		return (0);
	return (1);
}

int main(void)
{
    int ret;
    int x;
    int y;
    struct termios term, save;
    char *term_type = getenv("TERM");
    struct timeval timeout;

    // printf("TERM %s\n", term_type);

    ret = tgetent(NULL, term_type);

    char *cursor = tgetstr("cm", NULL);
    char *ku = tgetstr("ku", NULL);
    char *reset = tgetstr("me", NULL);

    timeout.tv_sec = 0;
	timeout.tv_usec = 5000;

    if (tcgetattr(0, &term) == -1)
        return (-1);

    if (tcgetattr(0, &save) == -1)
        return (-1);

    term.c_lflag &= ~(ICANON);
    term.c_lflag &= ~(ECHO);
    // term.c_lflag &= ~(ISIG); // remove control+c...

    if (tcsetattr(0, 0, &term) == -1)
			return (-1);
    
    // tputs(tgetstr("cl", NULL), 1, putchar); // clear terminal
    // tputs(tgetstr("vi", NULL), 1, putchar); // hide cursors

    int read_buffer;
    // fd_set read_fd_set;
    // FD_ZERO(&read_fd_set);
	// FD_SET(STDIN_FILENO, &read_fd_set);
    // int i = select(1, &read_fd_set, NULL, NULL, &timeout);
    int stop = 0;
    x = 37;
    y = 0;
    
    // printf("\e[6n\n");
    // read(STDIN_FILENO, read_buffer, sizeof(read_buffer));
    // read_buffer[0] = '\0';
    // int ii = 0;
    // while (read_buffer[ii] < '0' || read_buffer[ii] > '9')
    //     ii++;
    // y = ft_atoi(read_buffer + ii) - 1;
    // while (read_buffer[ii] >= '0' && read_buffer[ii] <= '9')
    //     ii++;
    // x = ft_atoi(read_buffer + ii + 1) - 1;
    // tputs(tgoto(tgetstr("cm", NULL), x, y), 1, putchar);
    // fflush(stdout);

    // tputs(tgetstr("cl", NULL), 1, putchar);
    while (!stop)
    {
        int rs = read(0, &read_buffer, sizeof(&read_buffer));
        // read_buffer[rs] = '\0';
        printf("%d\n", read_buffer);
        
        // if (strncmp(read_buffer, ku, strlen(ku)) == 0)
        // {
        //     printf("keyup\n");
        //     stop = 1;
        // }
        // else if (x == 5)
        // {
        //     x--;
        //     // putchar('\n');
        //     tputs(tgoto(cursor, x, y), 1, putchar);
        // }
        // // else if (x == 5)
        // // {
        // //     // dprintf(2, "left\n");
        // //     putchar('\n');
        // //     x = 0;
        // //     y++;
        // //     tputs(tgoto(tgetstr("cm", NULL), x, y), 1, putchar);
        // // } 
        // else if (ft_isprint(read_buffer[0]))
        // {            
        //     x++;
        //     putchar(read_buffer[0]);
        //     // tputs(read_buffer, 1, putchar);
            // tputs(tgoto(cursor, x, y), 1, putchar);
        // }
        fflush(stdout);
        read_buffer = 0;
    }
    
    // sleep(10);
    // tputs(tgetstr("ve", NULL), 1, putchar); // display cursor

    if (tcsetattr(0, 0, &save) == -1)
        return (-1);
    
    return ret;
}