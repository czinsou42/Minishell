/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlen_tab.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amwahab <amwahab@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/28 15:28:44 by amwahab           #+#    #+#             */
/*   Updated: 2025/09/22 12:06:15 by amwahab          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_strlen_tab(char **tab)
{
	int	count;

	count = 0;
	if (!tab)
		return (0);
	while (tab[count])
		count++;
	return (count);
}
