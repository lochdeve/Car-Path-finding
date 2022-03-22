# Car-Path-finding
## Description
This proyect is a simulation of a car that find the best path from the start to the end using differents heuristics.
### Heuristics
* [Manhattan distance](https://en.wikipedia.org/wiki/Taxicab_geometry)
* [Euclidean distance](https://en.wikipedia.org/wiki/Euclidean_distance#:~:text=In%20mathematics%2C%20the%20Euclidean%20distance,being%20called%20the%20Pythagorean%20distance.) 
## How to use it
First of all we have to install the grafics library called sfml using this command  ```sudo apt-get install libsfml-dev```

Then to execute it you have to write ```make run``` and it will appear the menu of the program.

To run the program, open a terminal capable of compiling and executing C++ programs. 
This terminal must have as directory the folder where this file is located.

The program has a Makefile file, which has the commands "make" (to compile), "make clean" (to remove the compilation files) and "make run" (to run the program).

Once the program has started, a menu appears. You must choose by typing the number (This applies to all stages of execution that require you to choose an option within a menu to choose an option within a menu).

To generate the environment by file, you must indicate the name of the file in .txt format file must be located in the same folder as this file, otherwise it will not be possible to open it. It will not be possible to open it. There are two files already made as examples ("ent1.txt" and "ent2.txt") but you can add and study as many as you want as long as they have the indicated format.

In order to generate the environment with the random obstacles, the user will be asked to initialise the environment. Except for the dimensions of the environment, which can be entered as desired the numbers indicated in the options must be entered. Otherwise the question will be the question will be repeated until a valid option is given.

To generate the environment manually, it is done in a similar way to the automatic one. You will be asked for the dimensions and the heuristic function that you want to use, after indicating these options the environment will be opened with empty boxes for the user to enter them manually. 
