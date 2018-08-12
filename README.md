# Stev-e_ENPH253

This is the software for the robot Stev-e. More information on Stev-e can be found here (Daniel put your link here).
The setting is Star Wars, and Stev-e's mission was to navigate the enemy stronghold and rescue four ewoks and chewbacca 
without being detected. 

This code was written on platfomio with arduino framework. The code ran on two Bluepill boards (STM32) which used I2C 
communication.

Due to the complexity of the course Stev-e had to navigate, a state machine was used in order to keep track of which sensors 
Stev-e should respond to, as well as ensuring certain obstacles were avoided.

Our goal was to make Stev-e reliable in scoring as many points as possible everytime. Since this was a competetion, speed was a factor, and we tuned Stev-e to preform optimally on the specific competition surface.
