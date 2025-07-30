/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstclear_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/26 13:55:21 by trupham           #+#    #+#             */
/*   Updated: 2025/04/26 14:08:40 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*curr;

	if (!*lst || !del)
		return ;
	while (*lst)
	{
		curr = (*lst)->next;
		ft_lstdelone(*lst, del);
		*lst = curr;
	}
	*lst = NULL;
}
