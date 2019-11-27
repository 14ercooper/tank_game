The CMakeLists.txt file included here is file from the template, as well as the files from the template.

I'm just including this note since I don't have a windows computer (I use Linux) so I coded it on a linux system.

I tested the template on the lab machines, and everything seemed to be working; but the SFML templates are horrible at being cross-platform. Seeing as I don't know what system you have as the grader, I figured I'd drop a line here.

If for whatever reason the template they provided doesn't work on your system; here are the files you need to compile & run:

main.cpp
Board.h
Board.cpp
Enemy.h
Enemy.cpp
Game.h
Game.cpp
Player.h
Player.cpp
Weapon.h
Weapon.cpp

Copying those three files to whatever SFML template works on your system and adding them to the CMakeLists.txt executable means everything should work just fine.
