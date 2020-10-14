/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zlayine <zlayine@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/03/07 19:13:51 by zlayine           #+#    #+#             */
/*   Updated: 2020/03/12 13:00:14 by zlayine          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <termios.h>
# include <curses.h>
# include <term.h>
#include <unistd.h>
#include <math.h>

# define ARRW_LEFT 4479771
# define ARRW_RIGHT 4414235
# define ALT_LEFT 25115
# define ALT_RIGHT 26139
# define HOME_BTN 4610843
# define END_BTN 4741915
# define ALT_V 10127586
# define ALT_X 8948194
# define ALT_C 42947
# define ALT_S 40899
# define ALT_UP 71688292227867
# define ALT_DOWN 72787803855643
# define ARRW_UP 4283163
# define ARRW_DOWN 4348699
# define BCK_SP 127
# define ENTER_BTN 10
# define SIG_C 3
# define SIG_D 4
# define SIG_SL 28


typedef struct  s_control
{
    char    *str;
    int     start;
    int     len;
    int     cut;
}               t_control;

typedef struct  s_config
{
    struct termios  term;
    char            *str;
    int             x;
    int             y;
    int             o_x;
    int             o_y;
    int             width;
    int             height;
    char            *cursor;
    long             buff;
    int             c;
    int             len;
    struct s_history *history;
    struct s_control control;
}               t_config;      

typedef struct  s_history
{
    char                *data;
    struct s_history    *prev;
    struct s_history    *next;
}               t_history;

int     ft_putchar(int c)
{
    write(0, &c, 1);
    return (0);
}

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

int		ft_isalpha(int c)
{
	if (c < 65 || (c > 90 && c < 97) || c > 122)
		return (0);
	return (1);
}

int		ft_isdigit(int c)
{
	if (c < 48 || c > 57)
		return (0);
	return (1);
}

int		ft_isprint(int c)
{
	if (c < 32 || c > 126)
		return (0);
	return (1);
}

int		ft_isalnum(int c)
{
	if (ft_isalpha(c) || ft_isdigit(c))
		return (1);
	return (0);
}

int	ft_strlen(const char *str)
{
	int len;

	len = 0;
	while (str[len] != '\0')
	{
		len++;
	}
	return (len);
}

char	*ft_strdup(const char *src)
{
	char	*res;
	int		i;

	i = 0;
	res = malloc(sizeof(*src) * (ft_strlen(src) + 1));
	if (!res)
		return (NULL);
	while (src[i])
	{
		res[i] = src[i];
		i++;
	}
	res[i] = '\0';
	return (res);
}

char	*ft_strcpy(char *dest, char *src)
{
	int		i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

size_t	ft_strlcpy(char *dest, const char *src, size_t size)
{
	size_t	i;
	size_t	full;

	if (!src)
		return (0);
	full = ft_strlen(src);
	i = 0;
	if (size > 0)
	{
		while (*src && i < size - 1)
		{
			*dest = *src;
			dest++;
			src++;
			i++;
		}
		*dest = '\0';
	}
	return (full);
}

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t	i;
	char	*d;
	char	*s;

	s = (char *)src;
	d = (char *)dst;
	if (!s && !d)
		return (0);
	i = 0;
	if (dst == src)
		return (dst);
	while (i < n)
	{
		*(d + i) = *(s + i);
		i++;
	}
	return (dst);
}

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	size_t			i;
	unsigned char	*d;
	unsigned char	*s;

	d = (unsigned char *)dst;
	s = (unsigned char *)src;
	if (!s && !d)
		return (0);
	i = d - s;
	if (i > 0 && i < len)
		while (len--)
			*(d + len) = *(s + len);
	else
		ft_memcpy(d, s, len);
	return (dst);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*r;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s1 || !s2)
		return (NULL);
	r = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!r)
		return (NULL);
	while (s1[i])
		r[j++] = s1[i++];
	i = 0;
	while (s2[i])
		r[j++] = s2[i++];
	r[j] = '\0';
	return (r);
}

void    print_line_up(t_config *config)
{
    int i;
    int c;
    
    config->y = ((config->o_x + config->c) / config->width) + config->o_y;
    if (config->o_y == 0)
    {
        i = 0;
        c = 0;
        dprintf(0, "minishell~>");
        tputs(tgoto(config->cursor, 12, config->y), 0, ft_putchar);
    }
    else
    {
        i = 0;
        c = config->c - config->width + 1;
        tputs(tgoto(config->cursor, 0, config->y), 0, ft_putchar);
    }
    while (i < config->width && config->str[i])
    {
        ft_putchar(config->str[i + c]);
        i++;
    }
}

void    print_line_down(t_config *config)
{
    int i;
    int c;

    config->y = ((config->o_x + config->c) / config->width) + config->o_y;
    i = 0;
    c = config->c;
    tputs(tgoto(config->cursor, 0, config->y), 0, ft_putchar);
    while (i < config->width && config->str[i])
    {
        ft_putchar(config->str[i + c]);
        i++;
    }
}

void    display_cursor(t_config *config)
{
    if (config->x == 0 && config->y == 0)
    {
        config->o_y++;
        tputs(tgetstr("sr", 0), 0, ft_putchar);
        print_line_up(config);
    }
    config->x = (config->o_x + config->c) % config->width;
    config->y = ((config->o_x + config->c) / config->width) + config->o_y;
    tputs(tgoto(config->cursor, config->x, config->y), 0, ft_putchar);
    if (config->o_y + ((config->o_x + config->c) / config->width) >= config->height)
    {
        tputs(tgetstr("sf", 0), 0, ft_putchar);
        config->o_y--;
        print_line_down(config);
    }
}

void    move_cursor(t_config *config, int dir)
{
    if (dir == 1 && config->c + config->x > config->o_x)
        config->c--;
    else if (dir == 3 && config->c < config->len)
        config->c++;
    else if (dir == 2 && config->c)
    {
        while (!ft_isalnum(config->str[config->c - 1]))
            config->c--;
        while (ft_isalnum(config->str[config->c - 1]))
            config->c--;
    }
    else if (dir == 4)
    {
        while (ft_isalnum(config->str[config->c]))
            config->c++;
        while (!ft_isalnum(config->str[config->c]) && config->str[config->c])
            config->c++;
    }
    else if (dir == 5 || dir == 6)
    {
        if (config->c - config->width > 0 && dir == 5)
            config->c -= config->width;
        else if (dir == 6 && config->len >= config->c + config->width)
            config->c += config->width;
    }
    display_cursor(config);
}

void    display_content(t_config *config)
{
    config->str[config->len] = 0;
    ft_putchar((int)config->str[config->len]);
    display_cursor(config);
}

void    delete_char(t_config *config)
{
    int i;

    if (config->c < config->len && config->c)
    {
        i = config->c - 1;
        config->c--;
        display_cursor(config);
        tputs(tgetstr("cd", NULL), 0, ft_putchar);
        while(i < config->len)
        {
            config->str[i] = config->str[i + 1];
            ft_putchar((int)config->str[i]);
            i++;
        }
        config->len--;
        display_content(config);
    }
    else if (config->c)
    {
        config->c--;
        config->len--;
        display_content(config);
        tputs(tgetstr("cd", NULL), 0, ft_putchar);
    }
}

void    print_char(t_config *config)
{
    char    tmp;
    int     i;

    if (config->c < config->len)
    {
        i = config->c;
        tputs(tgetstr("cd", NULL), 0, ft_putchar);
        while (i <= config->len)
        {
            tmp = config->str[i];
            config->str[i] = config->buff;
            config->buff = tmp;
            ft_putchar((int)config->str[i]);
            i++;
        }
        config->c++;
        config->len++;
    }
    else
    {
        config->str[config->c] = config->buff;
        ft_putchar((int)config->buff);
        config->c++;
        config->len = config->c;
    }
    display_content(config);
}


void    get_cursor_pos(t_config *config)
{
    int     i;
    char    buff[20];

    i = 0;
    bzero(buff, sizeof(char) * 20);
    dprintf(2, "\e[6n");
    read(2, buff, sizeof(buff));
    while (buff[i] < '0' || buff[i] > '9')
        i++;
    config->o_y = ft_atoi(buff + i) - 1;
    while (buff[i] >= '0' && buff[i] <= '9')
        i++;
    config->o_x = ft_atoi(buff + i + 1);
}

t_history   *new_history(t_history *prev)
{
    t_history *history;

    history = malloc(sizeof(t_history));
    history->data = "";
    history->prev = prev;
    history->next = NULL;
    return (history);
}

void    free_next_history(t_history **history)
{
    t_history *curr;
    t_history *tmp;

    curr = (*history)->next;
    while (curr)
    {
       tmp = curr->next;
       free(curr);
       curr = tmp;
    }
    (*history)->next = NULL;
}

t_config    *add_history(t_config *config)
{
    config->history->data = ft_strdup(config->str);
    if (config->history->next)
        free_next_history(&config->history);
    config->history->next = new_history(config->history);
    config->history = config->history->next;
    return (config);
}

void    display_history(t_config *config, int dir)
{
    int exist;
    
    exist = 0;
    if (dir && config->history->next)
    {
        config->history = config->history->next;
        exist = 1;
    }
    else if (dir == 0 && config->history->prev)
    {
        config->history = config->history->prev;
        exist = 1;
    }
    if (exist)
    {
        tputs(tgoto(config->cursor, config->o_x, config->o_y), 0, ft_putchar);
        tputs(tgetstr("cd", NULL), 0, ft_putchar);
        ft_strcpy(config->str, config->history->data);
        config->len = ft_strlen(config->str);
        config->c = config->len;
        dprintf(0, "%s", config->str);
        display_cursor(config);
    }
}

void    re_init_shell(t_config *config)
{
    config = add_history(config);
    bzero(config->str, config->len * sizeof(char));
    config->c = 0;
    config->buff = 0;
    config->len = 0;
    dprintf(0, "\nminishell~>");
    get_cursor_pos(config);
    display_cursor(config);
}

void    paste_control(t_config *config)
{
    int     i;
    char    tmp;

    ft_memmove(config->str + config->c + config->control.len, config->str + config->c, config->control.len + 1);
    ft_memcpy(config->str + config->c, config->control.str, config->control.len);
    config->len += config->control.len;
    i = config->c - 1;
    tputs(tgetstr("cd", NULL), 0, ft_putchar);
    while(++i < config->len)
        ft_putchar((int)config->str[i]);
    config->c += config->control.len;
    display_cursor(config);
}

void    cut_control(t_config *config)
{
    int i;
    
    config->control.cut = 1;
    config->control.len = config->c - config->control.start;
    if (config->control.start > config->c)
    {
        config->control.start = config->c;
        config->control.len = -config->control.len;
    }
    config->control.str = malloc(sizeof(char) * (config->control.len + 1));
    ft_strlcpy(config->control.str, config->str + config->control.start, config->control.len + 1);
    while (config->c > config->control.start)
        delete_char(config);
}

void    copy_control(t_config *config)
{
    config->control.cut = 0;
    config->control.len = config->c - config->control.start;
    if (config->control.start > config->c)
    {
        config->control.start = config->c;
        config->control.len = -config->control.len;
    }
    config->control.str = malloc(sizeof(char) * (config->control.len + 1));
    ft_strlcpy(config->control.str, config->str + config->control.start, config->control.len + 1);
}

void    set_control(t_config *config, int a)
{
    if (a == 0)
    {
        config->control.start = config->c;
        config->control.str = "";
        config->control.len = 0;
        config->control.cut = 0;
    }
    else if (a == 1)
        copy_control(config);
    else if (a == 2)
        cut_control(config);
}

void    handle_control(t_config *config)
{
    if (config->buff == ALT_V)
        paste_control(config);
    else if (config->buff == ALT_X)
        set_control(config, 2);
    else if (config->buff == ALT_C)
        set_control(config, 1);
    else if (config->buff == ALT_S)
        set_control(config, 0);
}

void    handle_btns(t_config *config)
{
    if (config->buff == HOME_BTN)
    {
        config->c = config->len;
        move_cursor(config, 0);
    }
    else if (config->buff == END_BTN)
    {
        config->c = 0;
        move_cursor(config, 0);
    }
    else if (config->buff == BCK_SP)
        delete_char(config);
    else if (config->buff == ENTER_BTN)
        re_init_shell(config);
}

void    handle_keys(t_config *config)
{
    if (config->buff == ARRW_LEFT)
        move_cursor(config, 1);
    else if (config->buff == ARRW_RIGHT)
        move_cursor(config, 3);
    else if (config->buff == ALT_LEFT)
        move_cursor(config, 2);
    else if (config->buff == ALT_RIGHT)
        move_cursor(config, 4);
    else if (config->buff == ALT_UP)
        move_cursor(config, 5);
    else if (config->buff == ALT_DOWN)
        move_cursor(config, 6);
    else if (config->buff == ARRW_UP)
        display_history(config, 0);
    else if (config->buff == ARRW_DOWN)
        display_history(config, 1);
    handle_btns(config);
    handle_control(config);
}

void    init_config_data(t_config *config)
{
    config->len = 0;
    config->str = malloc(sizeof(char) * 512);
    bzero(config->str, sizeof(char) * 512);
    config->buff = 0;
    config->history = malloc(sizeof(t_history));
    config->history->prev = NULL;
    config->history->next = NULL;
    config->width = tgetnum("co");
    config->height = tgetnum("li");
    get_cursor_pos(config);
    config->x = config->o_x;
    config->y = config->o_y;
    move_cursor(config, 0);
}

void    init_config(t_config *config)
{
    char        *term;
    int         ret;
    
    term = getenv("TERM");;
    ret = tgetent(NULL, term);
    if (ret < 1)
        printf("this is an error");
    if (tcgetattr(0, &config->term) == -1)
        printf("this is an error");
    config->term.c_lflag &= ~(ICANON | ECHO);
    config->cursor = tgetstr("cm", NULL);
    if (tcsetattr(0, 0, &config->term) == -1)
        printf("this is an error");
}

void    end_terminal(t_config *config)
{
    config->term.c_lflag |= (ICANON | ECHO );
    if (tcsetattr(0, 0, &config->term) == -1)
        printf("this is an error");
}

int main(void)
{
    t_config config;
    
    // term_type = getenv("TERM");
    // ret = tgetent(NULL, term_type);
    // if (ret < 1)
    //     printf("this is an error");
    
    // if (tcgetattr(0, &config.term) == -1)
    //     return (-1);
    
    
    // config.term.c_lflag &= ~(ICANON | ECHO);
    // config.cursor = tgetstr("cm", NULL);
    // config.term.c_lflag &= ~(ISIG); // remove control+c...

    // if (tcsetattr(0, 0, &config.term) == -1)
	// 		return (-1);
    init_config(&config);
    dprintf(0, "minishell~>");
    init_config_data(&config);
    // config.c = 0;
    // config.len = 0;
    // config.str = malloc(sizeof(char) * 512);
    // bzero(config.str, sizeof(char) * 512);
    // config.buff = 0;
    // config.history = malloc(sizeof(t_history));
    // config.history->prev = NULL;
    // config.history->next = NULL;
    // config.width = tgetnum("co");
    // config.height = tgetnum("li");
    
    
    // get_cursor_pos(&config);
    // config.x = config.o_x;
    // config.y = config.o_y;
    // move_cursor(&config, 0);
    int stop = 0;
    while (!stop)
    {
        read(0, &config.buff, sizeof(&config.buff));
        // dprintf(0, "%ld\n", config.buff);
        // check_scroll(&config);
        handle_keys(&config);
        if (ft_isprint(config.buff))
            print_char(&config);
        if (config.buff == 4)
        {
            ft_putchar('\n');
            stop = 1;
        }
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
        // fflush(stdout);
        config.buff = 0;
    }
    end_terminal(&config);
    return (0);
}





