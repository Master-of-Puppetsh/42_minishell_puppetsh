/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_command.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hjeon <hjeon@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/14 15:08:30 by hyekim            #+#    #+#             */
/*   Updated: 2020/07/01 18:50:16 by hjeon            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			g_target_flag = 0;

size_t		count_command(char *str, char target)
{
	int		target_flag;
	char	quote;
	int		count;
	int		i;

	init_3vars_to_zero(&quote, &count, &i);
	target_flag = 1;
	while (str[i] != '\0')
	{
		quote = check_quote(str, quote, i);
		if (str[i] != target && target_flag == 1)
		{
			count++;
			target_flag = 0;
		}
		if (str[i] == target && (quote == 0))
			target_flag = 1;
		else if (str[i] == target && (quote == 1))
		{
			target_flag = 1;
			quote = 0;
		}
		i++;
	}
	return (count);
}

size_t		ft_wordlen(char *str, char target)
{
	size_t		count;
	char		quote;
	int			i;

	i = 0;
	quote = check_quote(str, 0, i);
	count = 0;
	while ((quote || (str[i] != target || (target != ' '
			&& is_escape(i, quote, str)))) && str[i] != '\0')
	{
		count++;
		i++;
		quote = check_quote(str, quote, i);
	}
	return (count);
}

char		*ft_commanddup(char *str, char target, int *i)
{
	char	*result;
	char	quote;
	int		j;

	if (!(result = ft_calloc(sizeof(char), (ft_wordlen(str + *i, target) + 2))))
		return (NULL);
	j = 0;
	quote = check_quote(str, 0, *i);
	while ((quote || (str[*i] != target || (target != ' '
			&& is_escape(*i, quote, str)))) && str[*i] != '\0')
	{
		result[j] = str[*i];
		j++;
		if (quote == 1)
			quote = 0;
		(*i)++;
		quote = check_quote(str, quote, *i);
	}
	while (str[*i] != target && str[*i] != '\0')
		(*i)++;
	return (result);
}

void		set_flags(char *str, char *quote, char target, int idx)
{
	if (str[idx] == target && (*quote == 0) &&
		!(target != ' ' && is_escape(idx, *quote, str)))
		g_target_flag = 1;
	else if (str[idx] == target && (*quote == 1)
		&& !(target != ' ' && is_escape(idx, *quote, str)))
	{
		g_target_flag = 1;
		*quote = 0;
	}
	return ;
}

char		**split_command(char *str, char target)
{
	char	**result;
	char	quote;
	int		i;
	int		j;

	if (!(result = ft_calloc(sizeof(char *), (count_command(str, target) + 1))))
		return (NULL);
	g_target_flag = 1;
	init_3vars_to_zero(&quote, &i, &j);
	while (str[j] != '\0')
	{
		quote = check_quote(str, quote, j);
		if (str[j] != target && g_target_flag == 1
		&& !(target != ' ' && is_escape(j, quote, str)))
		{
			if (!(result[i++] = ft_commanddup(str, target, &j)))
				return (free_split(result));
			g_target_flag = 0;
			quote = 0;
		}
		set_flags(str, &quote, target, j);
		if (str[j] != '\0')
			j++;
	}
	return (result);
}
