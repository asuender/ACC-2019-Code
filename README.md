# ACC-2019-Code

Code [C++] for Level 3 of the Advent Coding Contest, written in C++.

## HOW-TOs

#### Executing:

- There are precompiled binaries that you can just run. If you can't find a matching executable, please go to the section about compiling.

#### Compiling:

- You can use the C++ compiler of your choice, however official support only covers the g++ command from GCC (GNU Compiler Collection).
- You compile the program using the command `g++ ./ACC_Andi.cpp -o ACC_Andi -pthread`. This will create a file named 'ACC_Andi' which is the desired executable.

#### Bug reporting:

- If you find any bug that isn't already known, please open a new bug report in the bug tracker. We will fix it as soon as possible.
- We realised that numbers higher than 100 won't be calculated; maybe 'cause there's an overflow. When you find that error in the code, please create an issue and we will fix it as soon as possible.

## CHANGELOG

#### 2019.12.22-14.00
 - added two more threads
 - removed ```std::async```; calculate is managed by ```std::future``` and ```std::promise``` 

#### 2019.12.22-11.00

- first working build

## SOLUTIONS

| 10        | 200         | 500          | 1000         | 10000         | 2000000000 |
| --------- | ----------- | ------------ | ------------ | ------------- | ---------- |
| 432256955 | 61956210911 | 149585554326 | 322833621931 | 3264567774119 | ?          |

