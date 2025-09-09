/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bash_comparison_test.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jyniemit <jyniemit@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/05 15:37:23 by jyniemit          #+#    #+#             */
/*   Updated: 2025/09/09 11:41:24 by jyniemit         ###   ########.fr       */
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
		sprintf(buffer, "./test/runner.sh \"%s\"", test_strings[i]);
		system(buffer);
	}
}
