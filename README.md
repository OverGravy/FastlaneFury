# FastlaneFury
Small university project that include using pthread and Allegro library and multi thread programming. The program will be a small dimulation of a fraction of an highway with  ultiple veicle and every veicle will be a task-

# Final result
In final result feature will be:

- User thread will handle input part.
- Grafics thread will handle the screen refresh and zoom function.
- Car spwan over user command.
- Userwill be able to zoom on a singol car.
- Cars will interact using sensor avoiding other car and slow down if needed.

# Actual Feature

- User thread  handle input part and grafics thread handle the screen refresh.
- There are mosre than 150 different car bitmap preloaded and scaled.
- Car spwan over user command.
- Car distance is implemented using a sensor.
- User is able to select a car and see some information about it and thread itseaf
- Is possible to pause the game.
- Is possible to see lot of information about the car and the thread by clicking over a car.
- When cliccking over a car the fov is realistic on what car see.
- Is NOW possible to pause all the car by leaving Graphics thread and User thread running.
- Car handle better with a NEW system to keep track of multiple direction
- Car Statistics such as MaxSpeed or MaxAcceleration are now loaded from an extern file giving mach variability to car behaviour.
- Is NOW present a menu to abilitate teh autospawn and chore between differnt time of spawn.
- There are NOW different Veicle type with different statistics in term of max speed acceleration and deceleration.


# How to use
Before compiling using make sure you have install the right allegro library version witch is allegro 4.2 using the following command:
```sudo apt install liballegro4-dev```
- Pressing space after the program spawn 1 car pressing another time spawn another car that perform overtake.
- Pressing 'p' will pause the game.
- Pressing 'p' again resume the game.
- Pressing 'esc' will close the game.
- Pressing 'space' will spawn a car.
- Pressing 'm' a configuration menu will appear.


# Future feature 

- User will be able to zoom on a singol car.
- There will be some information that will work as button on the bottom of the screen.

# Actual problem 

- I'm still not able to load different font from the default one.
- Sometime due to a thread problem closing the game crash it;
- Still need a litte bit of work on the car movement.
- Need a full code review to make it more readable and more efficient.

# Tips 

- To create a correct bitmap for the program use the following command (before use sudo apt install imagemagick):

```mogrify -format bmp -depth 16 -define bmp:format=bmp3 -define bmp:subtype=RGB *.png && rm *.png```
 