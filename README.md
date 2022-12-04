# Projects 5 & 6: Lights, Camera & Action!

All project handouts can be found [here](https://cs1230.graphics/projects).



## Project 6

### Overall Design

For this part of the project, I added 2 new shaders: on top of the original 2 for rendering the
scene, there are 2 additional shaders to handle the framebuffers.

To handle camera movement, I check which keys have been held down in the last update window,
calculate a direction in camera space based off of this and use the previous view matrix to
transform the movement direction to world space. Once in world space, I normalize the vector and
then scale it to correspond to 5 world-space units per second based on deltaTime. I then update the
camera's `pos` data accordingly and then rerender the scene using the updated camera position.

The camera rotation is similar, where instead of calculating a movement direction, I calculate the
rotation matrix corresponding to the mouse movement. I then apply this rotation to each of the
camera's original `pos`, `look`, and `up` values to get the updated ones, which are then used to
calculate the updated view matrix after rotation.

Finally, FBOs are implemented largely the same as in lab 11: the scene is rendered to `m_fbo` first,
which has the `m_fbo_texture` texture attachment. This texture attachment is rendered using
`m_texture_shader`, which applies the filters as selected in the settings.

### Changing the Default Framebuffer

On line 32 in `realtime.cpp`, modify the value that `m_defaultFBO` is being set to.

### Extra Credit

I implemented 5 items on the extra credit list for this homework:

- Extra credit 1: PER-PIXEL Grayscale filter. Implementation in `texture.frag`.

- Extra credit 2: KERNEL-BASED Sharpen filter. Implementation in `texture.frag`.

- Extra credit 3: PER-PIXEL Sepia filter. Implementation in `texture.frag`.

- Extra credit 4: KERNEL-BASED Vertical sobel filter. Implementation in `texture.frag`.

- Extra credit 5: PROJECT 5 Adaptive level of detail. Implementation in `realtime.cpp`, in functions
  `update<Shape>Data` on lines 65-98`. Shape parameters are scaled down in an inversely logarithmic
relation to the number of total shapes in the scene.
