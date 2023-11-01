/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 18:34:18 by lucocozz          #+#    #+#             */
/*   Updated: 2023/11/01 17:51:22 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

void	cleanup(void)
{
	g_global.logger.logInfo("Quitting.");
	flock(g_global.fd_lock, LOCK_UN);
	close(g_global.fd_lock);
	unlink(LOCK_FILE);
}
