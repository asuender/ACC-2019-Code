# ACC-2019-Code

Code for Level 3 of the Advent Coding Contest, written in C++.

## HOW-TOs

#### Executing:

- There are precompiled binaries that you can just run. If you can't find a matching executable, please go to the section about compiling.

##### Run (Linux):
`./ACC_Andi` make sure you are in the right directory.

If you are using a different OS, it is required to recompile the source code to run it.


| (Windows) It could be that the source code won't be able to compile since Winodws is a different OS. Maybe the reason is `std::mutex`. If you have a solution for that error, just create an issue. |
| --- |

#### Description of included files
- `ACC_Andi`: executable file. See chapter above.
- `ACC_Andi.cpp`: Source code.
- `ACC_Andi.o`: object file. only needed for compiling process
- `Changelog.log`: as the name it says
- `README.md`: this readme
- `doc/` contains the documentation
- `include/` ignore this folder now

#### Compiling:

- You can use the C++ compiler of your choice, however official support only covers the g++ command from GCC (GNU Compiler Collection).
- You compile the program using the command `g++ ./ACC_Andi.cpp -o ACC_Andi -pthread`. This will create a file named `ACC_Andi` which is the desired executable.

#### Bug reporting:

- If you find any bug that isn't already known, please open a new bug report in the bug tracker. We will fix it as soon as possible.
- We realised that numbers higher than 100 won't be calculated; the reason could be an overflow. When you find that error in the code, please create an issue and we will fix it as soon as possible.

## FEATURES
These following things are possible/implemented since the last update of this project:

- Multiple Numbers Handling
- Calculating the number/s in more than one task possible
- Duration output
- Caching of the results makes the code pretty fast
- Formatted progress printing of all threads

## CHANGELOG

See `Changelog.log`

## SOLUTIONS

| ---------------------------------- | 10        | 200         | 500          | 1000         | 10000         | 2000000000 |
| ---------------------------------- | --------- | ----------- | ------------ | ------------ | ------------- | ---------- |
| ---------------------------------- | 432256955 | 61956210911 | 149585554326 | 322833621931 | 3264567774119 | ?          |
| ---------------------------------- | --------- | ----------- | ------------ | ------------ | ------------- | ---------- |
| Duration on a Lenovo ThinkPad T430 |  < 1 sec. |  < 1 sec.   |   3 sec.     |    26 sec.   | ?             | ?          |

