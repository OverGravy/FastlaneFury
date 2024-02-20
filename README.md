# FASTLANE FURY
Small university project that include using pthread and Allegro library and multi thread programming. The program will be a small dimulation of a fraction of an highway with  ultiple veicle and every veicle will be a task-

# Install & usage:

Before compiling using make sure you have install the right allegro library version witch is allegro 4.2 using the following command:
```sudo apt install liballegro4-dev```
- Pressing space after the program spawn 1 car pressing another time spawn another car that perform overtake.
- Pressing 'p' will pause the game.
- Pressing 'p' again resume the game.
- Pressing 'esc' will close the game.
- Pressing 'space' will spawn a car.
- Pressing 'm' a configuration menu will appear.

# OVERVIEW

The game consist in 2 persistent thread. The graphics thread that handle the screen refresh and the user thread that handle the input part. The game consist in a small simulation of a fraction of an highway with multiple veicle and every veicle is a task. The user is able to spawn a car and see some information about it and thread itself. The objective of the game is to avoid collision and to see how the car behave in a real world situation. The game is a small university project that include using pthread and Allegro library and multi thread programming.


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


# Kown bugs 

- Using the brogram over a VM will cause the program to crash or to have a weird amount of dead line miss

# Tips 

- To create a correct bitmap for the program use the following command (before use sudo apt install imagemagick):

```mogrify -format bmp -depth 16 -define bmp:format=bmp3 -define bmp:subtype=RGB *.png && rm *.png```
 
