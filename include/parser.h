/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: trupham <trupham@student.hive.fi>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/25 13:54:35 by trupham           #+#    #+#             */
/*   Updated: 2025/08/25 14:09:52 by trupham          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_H
#define PARSER_H
#include "minishell.h"

bool parser_is_valid_pipe(t_token *token);
bool parser_is_valid_redirect(t_token *token);
bool parser_syntax_check(t_token *token);

#endif // !PARSER_H
