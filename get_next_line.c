/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ybouladh <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/15 21:33:52 by ybouladh          #+#    #+#             */
/*   Updated: 2018/10/21 17:33:59 by ybouladh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list		*check_fd_list(t_list **lst, int fd)
{
	t_list		*temporary;
	t_list		*new;

	temporary = *lst;
	while (temporary)
	{
		if ((int)temporary->content_size == fd)
			return (temporary);
		temporary = temporary->next;
	}
	if (!(new = (t_list*)malloc(sizeof(t_list))))
		return (NULL);
	if (!(new->content = ft_strnew(BUFF_SIZE)))
		return (NULL);
	new->content_size = fd;
	ft_lstadd(lst, new);
	return (*lst);
}

int			mover(char **src, int jumper)
{
	int hopper;

	hopper = 0;
	if ((*src)[jumper] == '\n')
		jumper++;
	while ((*src)[jumper])
		(*src)[hopper++] = (*src)[jumper++];
	while ((*src)[hopper])
		(*src)[hopper++] = '\0';
	return (0);
}

int			copy_past(int funct, char **dst, char **src)
{
	int		jumper;
	char	*temporary;

	jumper = -1;
	if (!(temporary = ft_strnew(BUFF_SIZE)))
		return (-1);
	if (funct == 1)
	{
		while ((*src)[++jumper] != '\n' && (*src)[jumper])
			temporary[jumper] = (*src)[jumper];
		temporary[jumper] = '\0';
		mover(src, jumper);
		*dst = ft_strjoin(*dst, temporary);
		free(temporary);
		return (0);
	}
	else
	{
		if ((*src)[0] == '\0')
			return (-1);
		while ((*src)[++jumper])
			if ((*src)[jumper] == '\n')
				return (1);
		return (0);
	}
}

int			ghost_reader(char **temporary, int fd, char **line)
{
	int jumper;

	jumper = 0;
	while ((jumper = read(fd, *temporary, BUFF_SIZE)))
	{
		if (jumper < 0)
			return (-1);
		((char*)(*temporary))[jumper] = '\0';
		if (copy_past(0, line, (char**)&(*temporary)) == 1)
		{
			copy_past(1, line, (char**)&(*temporary));
			return (1);
		}
		else if (copy_past(0, line, (char**)&(*temporary)) == 0)
			copy_past(1, line, (char**)&(*temporary));
	}
	return (0);
}

int			get_next_line(const int fd, char **line)
{
	static t_list	*string;
	t_list			*temporary;
	int				jumper;

	if (BUFF_SIZE < 0 || line == NULL || fd < 0)
		return (-1);
	temporary = check_fd_list(&string, fd);
	*line = ft_strnew(1);
	if ((jumper = copy_past(0, line, (char**)&temporary->content)) != -1)
	{
		copy_past(1, line, (char**)&temporary->content);
		if (jumper == 1)
			return (1);
	}
	if (ghost_reader((char**)&temporary->content, fd, line) == 1)
		return (1);
	else if (ghost_reader((char**)&temporary->content, fd, line) == -1)
		return (-1);
	if ((*line)[0] == '\0')
		return (0);
	return (1);
}
