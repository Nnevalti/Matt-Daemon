/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdescham <vdescham@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:48:57 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 16:17:15 by vdescham         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "matt-daemon.hpp"

t_glob g_global = {0};

void signal_handler(int signum)
{
	(void)signum;
	// close client fd
	flock(g_global.fd_lock, LOCK_UN);
	close(g_global.fd_lock);
	unlink(LOCK_FILE);
	exit(EXIT_SUCCESS);
}

int main()
{
	// if (getuid() != 0) {
	// 	std::cerr << "You must be root to run this program." << std::endl;
	// 	return (EXIT_FAILURE);
	// }
	if (signal(SIGINT, signal_handler) == SIG_ERR)
		return (EXIT_FAILURE);
	if (!handle_lock_file())
		return(EXIT_FAILURE);

	std::cout << "Running, created lock file" << std::endl;
	while (1);
	return (EXIT_SUCCESS);
}
