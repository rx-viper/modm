/*
 * Copyright (c) 2018 Christopher Durand
 *
 * This file is part of the modm project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
// ----------------------------------------------------------------------------

#include <modm/board.hpp>

#include <modm/processing/timer.hpp>
#include <modm/processing/protothread.hpp>
#include <modm/driver/motion/pat9125el.hpp>

//using I2c = I2cMaster1;
//using Scl = GpioB8;
//using Sda = GpioB9;

using Spi = SpiBidiMaster3;
using Sck = GpioB3;
using Mosi = GpioB5;
//using Sda = GpioB9;

// int pin is optional, set to void for polling mode
using Int = GpioInputB4;
using Cs = GpioOutputF12;

class Thread : public modm::pt::Protothread
{
public:
	Thread()// : sensor{0x75}
	{
	}

	bool
	update()
	{
		PT_BEGIN();

		MODM_LOG_INFO << "Ping device" << modm::endl;
		// ping the device until it responds
		while(true)
		{
			if (PT_CALL(sensor.ping())) {
				break;
			}
			// otherwise, try again in 100ms
			this->timeout.restart(100);
			PT_WAIT_UNTIL(this->timeout.isExpired());
		}
		MODM_LOG_INFO << "Ping successful" << modm::endl;

		PT_CALL(sensor.configure(0x14, 0x14));

		while (true)
		{
			PT_CALL(sensor.readData());
			if(sensor.hasMoved()) {
				position += sensor.getData();

				Board::Leds::write(0b111);
				MODM_LOG_INFO << "Position: " << position.x << ", " << position.y << modm::endl;
				sensor.resetMoved();
			} else {
				Board::Leds::write(0b000);
			}
		}

		PT_END();
	}

	modm::ShortTimeout timeout;
	modm::pat9125el::Motion2D position;
	modm::Pat9125el<modm::Pat9125elSpiTransport<Spi, Cs>, Int> sensor;
	uint8_t value = 0;

};

Thread thread;

// ----------------------------------------------------------------------------
int
main()
{
	Board::initialize();
	Board::Leds::setOutput();

	MODM_LOG_INFO << "\n\nPAT9125EL I2C example\n\n";

	Cs::setOutput(true);
	Sck::setOutput(false);
	Mosi::setInput(Mosi::InputType::PullDown);

	Int::setInput();

	Spi::connect<Sck::Sck, Mosi::Mosi>();
	Spi::initialize<Board::systemClock, 350'000, modm::Tolerance::TwentyPercent>();

	while (1)
	{
		thread.update();
	}

	return 0;
}
