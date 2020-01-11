# Design note

* Need a system to render a set of characters to the framebuffer
    * Entity and/or IRenderable

* Separate key event and render loop
    * And possibly a physics (fixed) update loop?

* Separate object implementations into classes / structs
    * Add callback fields to camera in order to make it keep tracks of a certain entity

```cpp

struct Humanoid
{
    char m_render_char[3][3];

    void render(frame_buffer_t* target, transform_t transform)
    { ... }
}

```

## Update 0111

* Need a Transform class to determine the position and the depth of the characters
* The frame buffer class should also support color and depth value writing