/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lock.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 15:17:19 by vdescham          #+#    #+#             */
/*   Updated: 2023/10/31 15:54:01 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

bool handle_lock_file(void) {
	g_global.fd_lock = open(LOCK_FILE, O_CREAT | O_RDWR, 644);
	if (flock(g_global.fd_lock, LOCK_EX | LOCK_NB) < 0) {
		// if (errno == EWOULDBLOCK)
			std::cerr << "Error: " << LOCK_FILE << " is locked." << std::endl;
		return false;
	}
	return true;
}