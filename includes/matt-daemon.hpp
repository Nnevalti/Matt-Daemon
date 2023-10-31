/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   matt-daemon.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 13:49:39 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 14:44:46 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>
#include <exception>
#include <string>
#include <unistd.h>
#include <signal.h>

#include "class/tintin_reporter.hpp"

#define PORT 4242
#define LOG_FILE "/var/log/matt_daemon.log"
#define LOCK_FILE "/var/lock/matt_daemon.lock"
#define MAX_CLIENT 3
