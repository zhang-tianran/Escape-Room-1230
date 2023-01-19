# Escape Room 1230

![demo image 1](https://github.com/zhang-tianran/Escape-Room-1230/blob/main/assets/img1.png)
![demo image 2](https://github.com/zhang-tianran/Escape-Room-1230/blob/main/assets/img2.png)
![demo image 3](https://github.com/zhang-tianran/Escape-Room-1230/blob/main/assets/img3.png)
Click [here](https://youtu.be/VeZaOQJnLn4v) to check out the demo!  

## Overview
This project simulates an interactive dungeon escape room based on OpenGL pipeline. The basic scene is rendered using trimesh, phong lighting, and camera movements. We also implemented shadows, depth of field, anti-aliasing, and collision detection to enhance the render quality. 

## Running the code
The project can be executed in Qt Creator IDE taking in the following Command line arguments: 
1. Scene light filepath (light.xml in repo)
2. Scene obj filepath (escape_room_scene.obj in repo)
3. Whether to render the objects in the scene.xml (set to false, mainly used for debugging)
4. Whether to render the objects in the .obj file (set to true if you want to see anything)

## Keyboard control
- W: move forward
- A: move backward
- S: move left
- D: move right
- 1: turn on/off anti-aliasing
- 2: turn on/off depth of field
- 3: turn on/off shadow
- 4: turn on/off collision
