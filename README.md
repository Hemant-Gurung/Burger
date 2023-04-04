# Manaengine

# Description
This is a 2D engine made with C++. It is made with the help of professors and the from the multiple resources.
This engine is also based on a book by Robert Nystrom "Game Programming Patterns".

Programming Patterns Used:
> Command Pattern,
> Singleton Pattern,
> Game Loop,
> Update Method,
> Component Pattern,
> Service Locator,
> Observer Pattern

All the objects are treated as components and those components are kept in a scene. This engine used scenemanager to manage all the scenes on how they 
will be played.

The level are read from the file, and the collisions are all done using a raycast on the list of vertices which are kept in a list as a 2D array.

![Tron](https://user-images.githubusercontent.com/84324141/199493264-08760bd9-7062-4c01-b4b1-8e471ffb0fea.gif)

 
 
For the graphic user interface Imgui is used. It is a graphical user interface design pattern 
which uses an immediate mode graphics library to create the GUI.
