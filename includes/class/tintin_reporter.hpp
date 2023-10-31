/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tintin_reporter.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lucocozz <lucocozz@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 14:04:39 by lucocozz          #+#    #+#             */
/*   Updated: 2023/10/31 14:22:45 by lucocozz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

class TintinReporter {
public:
	explicit TintinReporter();
	TintinReporter(TintinReporter const &instance);
	~TintinReporter();

	TintinReporter &operator=(TintinReporter const &instance);
};
