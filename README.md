# FastlaneFury
Small university project that include using pthread and Allegro library and ulti thread programming. The program will be a small dimulation of a fraction of an highway with  ultiple veicle and every veicle will be a task-

# Final result
In final result feature will be:

- User thread will handle input part.
- Grafics thread will handle the screen refresh and zoom function.
- Car spwan over user command.
- Userwill be able to zoom on a singol car.
- Cars will interact using sensor avoiding other car and slow down if needed.

# Actual Feature

- User thread  handle input part and grafics thread handle the screen refresh.
- There are more than 150 different car bitmap preloaded.
- Car spwan over user command.
- Car distance is implemented using a sensor.
- User is able to select a car and see some information about it and thread itseaf

# How to use

Pressing space after the program start car appear demostrating how sensor work if you make spawn another car.

# Future feature 

- User will be able to zoom on a singol car.
- there will be a pause funtion.
- There will be a menu to change some option.
- Car will have a much realistic movement.
- Car will be able to overtake other car.

# Actual problem 

- If car start overtaking it will not stop changing line.
- I'm still not able to load different font from the default one.
- Pause function kill the program.
- Sometime due to a thread problem closing the game crash it;

# Tips 

- To create a correct bitmap for the program use the following command (before use sudo apt install imagemagick):
```mogrify -format bmp -depth 16 -define bmp:format=bmp3 -define bmp:subtype=RGB *.png && rm *.png```
 