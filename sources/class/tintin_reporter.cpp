/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tintin_reporter.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:04:36 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 14:23:03 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "class/tintin_reporter.hpp"

TintinReporter::TintinReporter() {}

TintinReporter::TintinReporter(TintinReporter const &instance) {
	*this = instance;
}

TintinReporter::~TintinReporter() {}

TintinReporter &TintinReporter::operator=(TintinReporter const &instance) {
	if (this != &instance)
		*this = instance;
	return (*this);
}
