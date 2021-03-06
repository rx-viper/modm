/*
 * Copyright (c) 2018, Niklas Hauser
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_COMMON_CLOCK_HPP
#define MODM_COMMON_CLOCK_HPP

#include <stdint.h>
#include <modm/architecture/utils.hpp>

namespace modm
{

namespace clock
{

/**
 * Errors that might prevent using a certain clock configuration
 * @ingroup modm_platform_clock
 */
enum class
StartupError : uint8_t
{
	None = 0,
	InternalClock,
	ExternalClock,
	ExternalCrystal,
	LowSpeedInternalClock,
	LowSpeedExternalClock,
	LowSpeedExternalCrystal,
	Pll,
	SystemClock,
	ClockOutput,
	RealTimeClock,
	WatchdogClock
};

/// @cond
extern uint32_t modm_fastdata fcpu;
extern uint32_t modm_fastdata fcpu_kHz;
extern uint16_t modm_fastdata fcpu_MHz;
extern uint16_t modm_fastdata ns_per_loop;
/// @endcond

}	// namespace clock

}	// namespace modm

#endif	// MODM_COMMON_CLOCK_HPP
