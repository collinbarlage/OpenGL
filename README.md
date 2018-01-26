# OpenGL 🔥
CS432 hw1
Collin Barlage  
1/24/2018

##HW1 NOTES - Change Log

I have abstracted the `Square` class to a `Shape` class which takes in a dynamic vector of verticies. This way a square and a triangle can be drawn with the same class.

I have also created the `Circle` class which is very much identical to the `Shape` class, however it takes in a radius and shades it with random colors.

This multicolored shading was achieved by making a new fshader and vshader, and putting color data on the BufferSubData of the VBO.

## Features

In this program, the libraries GLUT and Glew are initialized utilized to set up shaders and shapes in an OpenGL environment. 

Also included in this folder is a jpg screencap of my program running. See `screenshot.jpg`

## Languages

Languages used to implement this program include C++, GLUT, Glew, and OpenGL.  

Note: This program is not designed to run on the UNIX environment *TUX*.

## Compiler

This program compiles with Visual Studio 2012

## Main()

The `main()` method for this program can be found in `main.cpp`.

## How to run

Because the assignment instructions did not specify a running environment, I can only guarantee this program runs smoothly in a Windows environment with Visual Studio 2012.

Included is the .vcxproj file. Not sure if this will help you set up the project quicker in Visual Studio. Let us know what kind of makefile we should produce for future assignments :)
 

🔥🔥🔥