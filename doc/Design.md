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

## Update 0206

* Need an animation system to animate different states of animator
    * Support state transition
    * Each state consists of seveeral key-frames, it should be a dictionary of frame-number and character sprite, and maintain a "current frame" member
        * Each frame we increase the "current frame" counter and if it passes a key-frame, replace the sprite pointer

* Need serious re-factoring
    * Use universal C-style naming and coding style
        * Like... typedef struct makes an anonymous struct, perhaps replace it?
        * use camelCase or snake_case?
        * ...
        ```
        Discussion : 
            * glfw uses camelCase for its function, like : glfwSetErrorCallback, glfwInit, structs as "struct GLFWimage"
            * From https://github.com/glfw/glfw/blob/master/include/GLFW/glfw3.h#L1770
            * stb, linmath.h uses snake_case : stbtt_bakedchar, stbtt_init, size_t, vec3_mul_cross
            * C standard library basically uses some snake_case and sometimes without underscore
            * From snake_case wiki : https://en.wikipedia.org/wiki/Snake_case
            * C, for some type names in the standard library, but not for function names. (update : function names are like getchar, strcat, they are abbreviated)
            * C++, for the standard library and Boost

        Conclusion :

            1. Avoid using typedef on anonymous structs, separate struct definition and typedef, or use typedef struct NAME {...} NAME;

            2. For structs, use camelCase with full caps prefix for project abbreviation

                typedef struct ASCRframeBuffer
                {...} ASCRframeBuffer;
            
            3. For functions, use camelCase with lower case prefix for project abbreviation and optional abbreviation of structs

                void fb_clear_frame_buffer(frame_buffer_t* fb);
                    will be changed to ...
                void ascrFrameBufferClear(ASCRframeBuffer* fb);

        ```