# Demonstration of PWM using Timer 2 and IC using Time 1

Based on ``ch32fun/examples/tim2_pwm/`` and ``ch32fun/examples/input_capture/``

## Use

Short pins PD3 (PWM output) and PD2 (Input capture) with an oscilloscope probe
for observing the varying duty cycle of the PWM and the timer measurement when
triggering on the rising edge (constant period, 10 ms) or falling edge (periodic
jump by 100 us when increasing the duty cycle).

Monitor PD3 to observe the PWM period and duty cycle evolution:

<img src="pwm.png">

Short circuit PD3 and PD2 with the oscilloscope probe to tigger the input capture:

<img src="IMG_20260712_170347_345_small.jpg">

<img src="IC.png">

Input Capture edge selection register, from the CH32V003 Reference Manual:

<img src="TIM2_CCER.png">
