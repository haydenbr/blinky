/*
 * blinky.c
 *
 * author: Furkan Cayci
 * description:
 *    Blinks 1 on-board LED at roughly 1 second intervals.
 *    system clock is running from HSI which is 16 Mhz.
 *    Delay function is just a simple counter so is not
 *    accurate and the delay time will change based on the
 *    optimization flags.
 *
 * gpio setup steps:
 *   There are at least three steps associated with GPIO:
 *   1. enable GPIOx clock from RCC
 *   2. set the direction of the pins from MODER (input / output)
 *   3. (optional) set the speed of the pins from OSPEEDR
 *   4. (optional) set pins to pull-up or pull-down or
 *         leave them floating from PUPDR
 *   5. (optional) set output type register to push-pull or
 *         open-drain from OTYPER
 *   6. either read from IDR or write to ODR depending on
 *         input or output configuration
 */

#include "stm32f429xx.h"

// create a led delay. Just a rough estimate
// for one second delay
#define LEDDELAY    1000000

/*************************************************
* function declarations
*************************************************/
int main(void);
void delay(volatile uint32_t);

/*************************************************
* main code starts from here
*************************************************/

const uint32_t LED_PIN = 13;
const uint32_t BUTTON_PIN = 0;

int main(void)
{
  // enable clock for GPIOA port
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  // button pin input mode
  GPIOA->MODER &= ~(0x3 << (BUTTON_PIN*2));
  // button pin pull-down
  GPIOA->PUPDR &= ~(0x3 << (BUTTON_PIN*2));
  GPIOA->PUPDR |= (0x2 << (BUTTON_PIN*2));

  // enable clock for GPIOG port
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOGEN;
  // write 00 (reset) for LED pin mode
  GPIOG->MODER  &= ~(0x3 << (LED_PIN*2));
  // write 01 (general purpose output) for LED pin mode
  GPIOG->MODER  |=  (0x1 << (LED_PIN*2));
  // write 0 (output push-pull) for LED pin output type
  GPIOG->OTYPER &= ~(1 << LED_PIN);

  // RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
  EXTI->IMR |= (1 << BUTTON_PIN);
  SYSCFG->EXTICR[0] |= 0x00;
  //Attach interrupt
  // enable rising edge trigger
  EXTI->RTSR |= (1 << BUTTON_PIN);
  NVIC_SetPriority(EXTI0_IRQn, 1);
  NVIC_EnableIRQ(EXTI0_IRQn);

  while(1) {}

  return 0;
}

void EXTI0_IRQHandler(void) {
  GPIOG->ODR |= (1 << LED_PIN);
  EXTI->PR = EXTI_PR_PR0;
}
