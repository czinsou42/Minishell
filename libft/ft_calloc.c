/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_calloc.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: czinsou <czinsou@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 20:13:36 by czinsou           #+#    #+#             */
/*   Updated: 2025/04/30 20:13:36 by czinsou          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_calloc( size_t nmenb, size_t size)
{
	char	*result;

	result = malloc(nmenb * size);
	if (!result)
		return (0);
	ft_memset(result, 0, nmenb * size);
	return (result);
}
