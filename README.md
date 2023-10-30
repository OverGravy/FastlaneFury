# FastlaneFury
Small university project that include using pthread and Allegro library and ulti thread programming. The program will be a small dimulation of a fraction of an highway with  ultiple veicle and every veicle will be a task-

# Final result
In final result feature will be:

- User thread will handle input part.
- Grafics thread will handle the screen refresh and zoom function.
- Car spwan over user command.
- Userwill be able to zoom on a singol car.
- Cars will interact using sensor avoiding other car and slow down if needed.

# Tips 



- To create a correct bitmap for the program use the following command (before use sudo apt install imagemagick):
```mogrify -format bmp -depth 16 -define bmp:format=bmp3 -define bmp:subtype=RGB *.png && rm *.png```
 