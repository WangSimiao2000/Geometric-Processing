# COMP 5812M Assignment 3

Name: Simiao Wang
Student ID: 201702881
University Email: sc23s2w@leeds.ac.uk
Personal Email: mickeymiao2023@163.com
Completion Date: January 10, 2024
University of Leeds 2023-2024

If compilation is needed, please use the "make" command for compilation. After compilation, enter "./TextureProcessing" followed by the path to the model's obj file and press Enter to run the program. For example: "./TextureProcessing ./models/hamishtrunc1.obj".

1. When Texture is selected, the model in the window will be tiled into a square texture map; when Texture is not selected, all points will be displayed in the window in the state of model coordinates.
2. Besides Texture, when only Wireframe is selected, the model or texture map in the window will be presented in the form of a grid.
3. Besides Texture, when only UVM -> RGB is selected, the display in the window will show the color obtained by converting the coordinates of the point on the texture map.
4. Besides Texture, when only Normal Map -> RGB is selected, the display in the window will show the color obtained by converting the normals of each point in the model state (Color = (Normal + 1) * 0.5).
