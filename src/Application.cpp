/*
 * LPC2478.h
 *
 *  Created on: Dec 31, 2009
 *      Author: fgervais
 */

int main() {
/*
	Gpio *gpioA = STM32F103::getGpioA();
	Gpio *gpioB = STM32F103::getGpioB();
	Gpio *gpioC = STM32F103::getGpioC();

	// Set default port behavior
	GpioConfiguration portConfig(Gpio::FLOATING_INPUT);
	gpioA->configure(portConfig);

	// Configure blinking led
	GpioPinConfiguration ledPinConfig;
	ledPinConfig.pin = Gpio::GP_PUSH_PULL_OUTPUT | Gpio::OUTPUT_SPEED_50MHZ;
	gpioA->getPin(0)->configure(ledPinConfig);

	GpioPin *led = gpioA->getPin(0);

	// Blink led
	while(1) {
		led->setHigh();	// On
		for(uint32_t i=0; i<100000; i++);
		led->setLow();	// Off
		for(uint32_t i=0; i<100000; i++);
	}
*/
}
