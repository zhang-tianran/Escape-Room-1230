# Project Realtime

**Known Bugs:** n/a

**Overview**: this project applies trimesh and phong lighting to render a scene using OpenGL pipeline. 

## Lights, Camera
- Implementation steps: 
    1. Parse the scene. 
    2. Create shapes consisted of vertices and normals
    3. Establish VAO/VBO objects for shapes
    4. Establish camera with view and projection matrices
    5. Establish shaders and pass in uniforms containing scene information
    6. Compute Phong lighting within fragment shader using light and object material information
- Shapes: 
    - This project supports Cube, Sphere, Cone, and Cylinder
    - The shapes are established using Triangle tiles. The shape approximation is controled by 2 changable parameters. 
- VAO/VBO: 
    - The object contains 4 VAOs and 4 VBOs, each corresponds to a shape
- Shaders: 
    - This project contains vertex and fragment shaders
    - The shaders support Phong illumination with ambient, diffuse, and specular

## Action!
- Camera Interactability
    - Translational movement: the user could navigate through the scene through keyboard control
    - Rotational movement: the user could use mouse click events to change the view window. 
- Lights: 
    - The project supports 8 lights in total through Phone illumination. The light types include directional light, point light, and spot light. 
- FBOs
    - Per-Pixel Filter: invert
    - Kernel-Based Filter: sharpen

**Extra Credit**

Additional filters: 
1. Grayscale (extra credit 1)
2. Blurring (extra credit 2)
