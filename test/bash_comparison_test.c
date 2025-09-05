/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_comparison_test.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:37:23 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/05 17:25:01 by jyniemit         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "munit.h"
#include <stdio.h>

static const char *test_strings[] = {
#define X(str) str,
#include "test_cases.def"
#undef X
	NULL
};

void	bash_comparison(void)
{
	int		i = -1;

	while (test_strings[++i])
	{
		char buffer[256];
		sprintf(buffer, "./runner.sh \"%s\"", test_strings[i]);
		system(buffer);
	}
}
