/*
 *
 * Copyright (c) 2018, Nick Sarten
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#ifndef MODM_STM32_F103C8T6_BLACK_PILL_HPP
#define MODM_STM32_F103C8T6_BLACK_PILL_HPP

#include <modm/platform.hpp>
#include <modm/architecture/interface/clock.hpp>

using namespace modm::platform;

/// @ingroup modm_board_black_pill
namespace Board
{

/// STM32F103 running at 72MHz generated from the external 8MHz crystal
// Dummy clock for devices
struct systemClock {
	static constexpr uint32_t Frequency = MHz72;
	static constexpr uint32_t Ahb = Frequency;
	static constexpr uint32_t Apb1 = Frequency / 2;
	static constexpr uint32_t Apb2 = Frequency;

	static constexpr uint32_t Adc1 = Apb2;
	static constexpr uint32_t Adc2 = Apb2;
	static constexpr uint32_t Adc3 = Apb2;

	static constexpr uint32_t Spi1 = Apb2;
	static constexpr uint32_t Spi2 = Apb1;
	static constexpr uint32_t Spi3 = Apb1;

	static constexpr uint32_t Usart1 = Apb2;
	static constexpr uint32_t Usart2 = Apb1;
	static constexpr uint32_t Usart3 = Apb1;
	static constexpr uint32_t Uart4  = Apb1;
	static constexpr uint32_t Uart5  = Apb1;

	static constexpr uint32_t Can    = Apb1;

	static constexpr uint32_t I2c1   = Apb1;
	static constexpr uint32_t I2c2   = Apb1;

	static constexpr uint32_t Apb1Timer = Apb1 * 2;
	static constexpr uint32_t Apb2Timer = Apb2 * 1;
	static constexpr uint32_t Timer1  = Apb2Timer;
	static constexpr uint32_t Timer2  = Apb1Timer;
	static constexpr uint32_t Timer3  = Apb1Timer;
	static constexpr uint32_t Timer4  = Apb1Timer;

	static bool inline
	enable()
	{
		ClockControl::enableExternalCrystal();

		// external clock * 9 = 72MHz, => 72/1.5 = 48 => good for USB
		ClockControl::enablePll(ClockControl::PllSource::ExternalCrystal, 9);

		// set flash latency for 72MHz
		ClockControl::setFlashLatency(Frequency);

		// switch system clock to PLL output
		ClockControl::enableSystemClock(ClockControl::SystemClockSource::Pll);

		// AHB has max 72MHz
		ClockControl::setAhbPrescaler(ClockControl::AhbPrescaler::Div1);

		// APB1 has max. 36MHz
		ClockControl::setApb1Prescaler(ClockControl::Apb1Prescaler::Div2);

		// APB2 has max. 72MHz
		ClockControl::setApb2Prescaler(ClockControl::Apb2Prescaler::Div1);
		// update frequencies for busy-wait delay functions


		// update frequencies for busy-wait delay functions
		modm::clock::fcpu     = Frequency;
		modm::clock::fcpu_kHz = Frequency / 1000;
		modm::clock::fcpu_MHz = Frequency / 1000000;
		modm::clock::ns_per_loop = ::round(3000.f / (Frequency / 1000000));

		return true;
	}
};

// User LED (inverted, because connected to 3V3)
using LedGreen = GpioInverted< GpioOutputB12 >;
using Leds = SoftwareGpioPort< LedGreen >;

using Button = GpioUnused;

inline void
initialize()
{
	systemClock::enable();
	modm::cortex::SysTickTimer::initialize<systemClock>();

	LedGreen::setOutput(modm::Gpio::Low);
}

} // Board namespace

#endif	// MODM_STM32_F103C8T6_BLACK_PILL_HPP
