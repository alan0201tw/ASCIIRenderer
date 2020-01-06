#include <stdio.h>
#include <string.h>
// stb_truetype
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"
// opengl : glad and glfw
#include "glad/glad.h"
#include <GLFW/glfw3.h>
// vector library
#include "linmath.h"

#include "common.h"
#include "framebuffer.h"
#include "character.h"

// static variables
static stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
static GLuint ftex;
static frame_buffer_t frame_buffer;

static size_t screen_width = 640;
static size_t screen_height = 480;

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

void get_uv_by_char(vec4 out_vec4, const stbtt_bakedchar *chardata, const char c)
{
    const stbtt_bakedchar *b = chardata + c - 32;
    float ipw = 1.0f / 512, iph = 1.0f / 512;

    out_vec4[0] = b->x0 * ipw;
    out_vec4[1] = b->y0 * iph;
    out_vec4[2] = b->x1 * ipw;
    out_vec4[3] = b->y1 * iph;
}

void stbtt_init(void)
{
    unsigned char ttf_buffer[1 << 20];
    unsigned char bitmap[512 * 512];

    FILE* font_file = fopen("./resources/clacon.ttf", "rb");
    size_t fread_size = fread(ttf_buffer, 1, 1 << 20, font_file);
    if(fread_size <= 0)
        printf("SYS : fread error in stbtt_init \n");

    stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0f, bitmap, 512, 512, 32, 96, cdata); // no guarantee this fits!
    // can free ttf_buffer at this point
    glGenTextures(1, &ftex);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, 512, 512, 0, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap);
    // can free temp_bitmap at this point
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

// this reads the entire frame_buffer and render it on the screen
void stbtt_render()
{
    const float screen_width_float = (float)screen_width;
    const float screen_height_float = (float)screen_height;
    const float x_interval = screen_width_float / FRAME_BUFFER_WIDTH;
    const float y_interval = screen_height_float / FRAME_BUFFER_HEIGHT;

    // assume orthographic projection with units = screen pixels, origin at top left
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, ftex);
    glBegin(GL_QUADS);
    for(size_t i = 0; i < FRAME_BUFFER_WIDTH; ++i)
    {
        for(size_t j = 0; j < FRAME_BUFFER_HEIGHT; ++j)
        {
            char value = frame_buffer.char_value[i][j];
            if(value >= 128 || value < 32)
            {
                printf("SYS : frame_buffer contains control character at "
                "i = %ld, j = %ld \n", i, j);
                continue;
            }

            vec4 font_texture_uv;
            get_uv_by_char(font_texture_uv, cdata, value);

            vec4 pos;
            pos[0] = (float)(-screen_width_float / 2.0f + i * x_interval);
            pos[1] = (float)(pos[0] + x_interval);
            pos[2] = (float)(-screen_height_float / 2.0f + j * y_interval);
            pos[3] = (float)(pos[2] + y_interval);

            float mid_x = 0.5f * (pos[0] + pos[1]);
            pos[0] = (pos[0] - mid_x) * 0.9f + mid_x;
            pos[1] = (pos[1] - mid_x) * 0.9f + mid_x;

            float mid_y = 0.5f * (pos[2] + pos[3]);
            pos[2] = (pos[2] - mid_y) * 0.9f + mid_y;
            pos[3] = (pos[3] - mid_y) * 0.9f + mid_y;

            glColor3fv(frame_buffer.color_value[i][j]);

            glTexCoord2f(font_texture_uv[0],font_texture_uv[3]);
            glVertex2f(pos[0],pos[2]);
            glTexCoord2f(font_texture_uv[2],font_texture_uv[3]);
            glVertex2f(pos[1],pos[2]);
            glTexCoord2f(font_texture_uv[2],font_texture_uv[1]);
            glVertex2f(pos[1],pos[3]);
            glTexCoord2f(font_texture_uv[0],font_texture_uv[1]);
            glVertex2f(pos[0],pos[3]);
        }
    }
    glEnd();
}

int main(int argc, char* argv[])
{
    glfwSetErrorCallback(error_callback);

    GLFWwindow* window;
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(
        screen_width, screen_height, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    // gladLoadGL(glfwGetProcAddress);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    stbtt_init();


    humanoid_t mainCharacter;
    mainCharacter.m_position[0] = 10.0f;
    mainCharacter.m_position[1] =  9.0f;

    while (!glfwWindowShouldClose(window))
    {
        for(size_t i = 0; i < FRAME_BUFFER_WIDTH; ++i)
        {
            for(size_t j = 0; j < FRAME_BUFFER_HEIGHT; ++j)
            {
                frame_buffer.char_value[i][j] = ' ';

                if(i + j > 21 && i + j < 80 && j > 30)
                {
                    frame_buffer.char_value[i][j] = 'a';
                    frame_buffer.color_value[i][j][0] = 1.0f;
                    frame_buffer.color_value[i][j][1] = 1.0f;
                    frame_buffer.color_value[i][j][2] = 1.0f;
                }
            }
        }

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            ++mainCharacter.m_position[0];
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            --mainCharacter.m_position[0];
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            ++mainCharacter.m_position[1];
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            --mainCharacter.m_position[1];

        render_humanoid(&frame_buffer, &mainCharacter);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-320, 320, -240, 240, 0.01f, 1000);
        
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        mat4x4 view_matrix;
        vec3 camera_pos = {0.0f, 0.0f, 10.0f};
        vec3 look_at = {0.0f, 0.0f, -1.0f};
        vec3 look_up = {0.0f, 1.0f, 0.0f};
        mat4x4_look_at(
            view_matrix, 
            camera_pos,
            look_at,
            look_up
            );
        glLoadMatrixf((const GLfloat*) &view_matrix);

        glPushMatrix();
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            // frame_buffer.char_value[rand() % 50][rand() % 50] = 'U';
            // frame_buffer.char_value[rand() % 50][rand() % 50] = ' ';
            stbtt_render();
        }
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    return 0;
}