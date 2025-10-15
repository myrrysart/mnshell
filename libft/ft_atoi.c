/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: Jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:52:10 by Jyniemit          #+#    #+#             */
/*   Updated: 2025/10/15 18:51:45 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_iswhitespace(int c)
{
	if (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\v'
		|| c == '\r')
		return (1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int	i;
	int	sign;
	int	res;

	i = 0;
	sign = 1;
	res = 0;
	while (ft_iswhitespace((int)str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit((int)str[i]))
	{
		res = res * 10 + (str[i] - '0');
		i++;
	}
	return (res * sign);
}

long long	ft_atol(const char *str)
{
	int						i;
	int						sign;
	unsigned long long int	raw;

	i = 0;
	raw = 0;
	sign = 1;
	while (ft_iswhitespace((int)str[i]))
		i++;
	if (str[i] == '-')
	{
		sign = -1;
		i++;
	}
	else if (str[i] == '+')
		i++;
	while (ft_isdigit((int)str[i]))
	{
		raw = raw * 10 + (str[i] - '0');
		i++;
	}
	if (raw > LLONG_MAX)
		return (-1);
	return ((long long int)raw * sign);
}
