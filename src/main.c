#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

// stb_truetype
#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

// opengl : glad and glfw
#include "glad/glad.h"
#include <GLFW/glfw3.h>

// vector library
#include "linmath.h"

#include "common.h"
#include "memory.h"
#include "framebuffer.h"
#include "textsprite.h"
#include "animation.h"

// static variables
static stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs
static GLuint ftex;
static ASCRframeBuffer frameBuffer;

static int screen_width = 640;
static int screen_height = 480;
static float screen_aspect_ratio;

static GLFWwindow* window;

void init()
{
    screen_aspect_ratio = (float)screen_width / (float)screen_height; // w / h;
}

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
	unsigned char* ttf_buffer = (unsigned char*)ascrMalloc((1 << 20) * sizeof(unsigned char));
	unsigned char* bitmap = (unsigned char*)ascrMalloc((512 * 512) * sizeof(unsigned char));

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

	ascrFree(ttf_buffer);
	ascrFree(bitmap);
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
            char value = frameBuffer.char_value[i][j];
            if(value >= 128 || value < 32)
            {
                printf("SYS : frame_buffer contains control character at "
                "i = %zd, j = %zd \n", i, j);
                continue;
            }

            vec4 font_texture_uv;
            get_uv_by_char(font_texture_uv, cdata, value);

            vec4 pos;

            // TODO : place the text in the middle of the area

            float textXInterval = (font_texture_uv[2] - font_texture_uv[0]);
            float textYInterval = (font_texture_uv[3] - font_texture_uv[1]);

            float halfWidth = 0.5f * 32.0f * screen_aspect_ratio * textXInterval * x_interval;
            float halfHeight = 0.5f * 32.0f * textYInterval * y_interval;

            // put the character in the middle of the area in X-axis
            // and on bottom in Y-axis
            pos[0] = (float)(-screen_width_float / 2.0f + (i + 0.5f) * x_interval - halfWidth);
            pos[1] = (float)(pos[0] + 2.0f * halfWidth);
            pos[2] = (float)(-screen_height_float / 2.0f + j * y_interval);
            pos[3] = (float)(pos[2] + 2.0f * halfHeight);

            const float rescaleRatio = 0.95f;

            float mid_x = 0.5f * (pos[0] + pos[1]);
            pos[0] = (pos[0] - mid_x) * rescaleRatio + mid_x;
            pos[1] = (pos[1] - mid_x) * rescaleRatio + mid_x;

            float mid_y = 0.5f * (pos[2] + pos[3]);
            pos[2] = (pos[2] - mid_y) * rescaleRatio + mid_y;
            pos[3] = (pos[3] - mid_y) * rescaleRatio + mid_y;

            glColor3fv(frameBuffer.color_value[i][j]);

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

// walking right
bool idle2WalkingPredicate()
{
    if(
        glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && 
        glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE
    )
        return true;
    return false;
}

bool idle2WalkingLeftPredicate()
{
    if(
        glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && 
        glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE
    )
        return true;
    return false;
}

bool walking2IdlePredicate()
{
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        // if both key is pressed, back to idle
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            return true;
        return false;
    }
    return true;
}

bool walkingLeft2IdlePredicate()
{
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        // if both key is pressed, back to idle
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            return true;
        return false;
    }
    return true;
}

int main(int argc, char* argv[])
{
    init();

    glfwSetErrorCallback(error_callback);
    
    if (!glfwInit())
        exit(EXIT_FAILURE);
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    window = glfwCreateWindow(
        (int)screen_width, (int)screen_height, "ASCIIRenderer", NULL, NULL);
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

    clock_t previousFrameStartTime = clock();

    frameBuffer.center[0] = 0.0f;
    frameBuffer.center[1] = 0.0f;

    ASCRtextSpriteEntity characterEntity;
    characterEntity.transform.position[0] = 10.0f;
    characterEntity.transform.position[1] =  9.0f;
    // depth
    characterEntity.transform.position[2] =  4.0f;

    // ASCRtextSprite* idleSprite = (ASCRtextSprite*) ascrMalloc(sizeof(ASCRtextSprite));
    ASCRtextSprite idleSprite;
    ascrTextSpriteCreateFromFile(&idleSprite, "./resources/texts/humanoid.txt");
    // ASCRtextSprite* walkingSprite = (ASCRtextSprite*) ascrMalloc(sizeof(ASCRtextSprite));
    const size_t walkingSPriteCount = 2;
    ASCRtextSprite walkingSprites[walkingSPriteCount];
    ascrTextSpriteCreateFromFile(&walkingSprites[0], "./resources/texts/humanoid_walking0.txt");
    ascrTextSpriteCreateFromFile(&walkingSprites[1], "./resources/texts/humanoid_walking1.txt");

    ASCRtextSprite walkingLeftSprites[walkingSPriteCount];
    ascrTextSpriteCreateFromFile(&walkingLeftSprites[0], "./resources/texts/humanoid_walking_left0.txt");
    ascrTextSpriteCreateFromFile(&walkingLeftSprites[1], "./resources/texts/humanoid_walking_left1.txt");

    ASCRanimationState characterIdle, characterWalking, characterWalkingLeft;
    ascrAnimationStateInit(&characterIdle);
    ascrAnimationStateInit(&characterWalking);
    ascrAnimationStateInit(&characterWalkingLeft);

    ASCRanimationState* currentState = &characterIdle;

    ASCRanimationClip idleClip, walkingClip, walkingLeftClip;
    ascrAnimationClipInit(&idleClip);
    ascrAnimationClipInit(&walkingClip);
    ascrAnimationClipInit(&walkingLeftClip);
    vec_push(&idleClip.sprites, idleSprite);
    for(size_t idx = 0; idx < walkingSPriteCount; ++idx)
    {
        vec_push(&walkingClip.sprites, walkingSprites[idx]);
        vec_push(&walkingLeftClip.sprites, walkingLeftSprites[idx]);
    }
    	
    characterIdle.clip = idleClip;
    characterWalking.clip = walkingClip;
    characterWalkingLeft.clip = walkingLeftClip;
    
    // idle to walking left and right transition
    ASCRanimationStateTransition idle2WalkingTransition;
    idle2WalkingTransition.predicate = idle2WalkingPredicate;
    idle2WalkingTransition.targetState = &characterWalking;
    
    vec_push(
    	&characterIdle.transitions,
    	idle2WalkingTransition
    );

    ASCRanimationStateTransition idle2WalkingLeftTransition;
    idle2WalkingLeftTransition.predicate = idle2WalkingLeftPredicate;
    idle2WalkingLeftTransition.targetState = &characterWalkingLeft;
    
    vec_push(
    	&characterIdle.transitions,
    	idle2WalkingLeftTransition
    );
    // end of - idle to walking left and right transition

    // walking to idle transition
    ASCRanimationStateTransition walking2IdleTransition;
    walking2IdleTransition.predicate = walking2IdlePredicate;
    walking2IdleTransition.targetState = &characterIdle;
    
    vec_push(
    	&characterWalking.transitions,
    	walking2IdleTransition
    );

    ASCRanimationStateTransition walkingLeft2IdleTransition;
    walkingLeft2IdleTransition.predicate = walkingLeft2IdlePredicate;
    walkingLeft2IdleTransition.targetState = &characterIdle;
    
    vec_push(
    	&characterWalkingLeft.transitions,
    	walkingLeft2IdleTransition
    );
    // end of - walking to idle transition

    characterEntity.animatorState = currentState;

    while (!glfwWindowShouldClose(window))
    {
        // clear the frame buffer
        ascrFrameBufferClear(&frameBuffer);

        int ww, wh;
        glfwGetWindowSize(window, &ww, &wh);
        screen_width = ww;
        screen_height = wh;
        screen_aspect_ratio = screen_width / screen_height;

        clock_t frameStartTime = clock();
        const float deltaTime = (float)(frameStartTime - previousFrameStartTime) / CLOCKS_PER_SEC;
        previousFrameStartTime = frameStartTime;

        // Update
        ascrTextSpriteEntityUpdate(
            &characterEntity,
            deltaTime
        );
        //
        char sampleText[32];
        sprintf(sampleText, "FPS = %f", 1.0f / deltaTime);
        vec3 white = {1.0f, 1.0f, 1.0f};
        ascrFrameBufferWriteStringScreenSpace(
            &frameBuffer, 0, 0, 3, white, sampleText
        );
        sprintf(sampleText, "dT = %f", deltaTime);
        ascrFrameBufferWriteStringScreenSpace(
            &frameBuffer, 0, 2, 3, white, sampleText
        );

        char roadText[FRAME_BUFFER_WIDTH];
        for(size_t idx = 0; idx < FRAME_BUFFER_WIDTH; ++idx)
            roadText[idx] = '_';
        ascrFrameBufferWriteString(
            &frameBuffer, 0, 8, 5, white, roadText
        );

        // for(size_t idx = 0; idx < 10; ++idx)
        // {
        //     vec3 col = {(float)rand() / RAND_MAX, (float)rand() / RAND_MAX, (float)rand() / RAND_MAX};

        //     ascrFrameBufferWriteChar(&frameBuffer, rand()%50, rand()%50, 5, col, 'a');
        // }

        const float velocity = 10.0f;

        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            characterEntity.transform.position[0] += deltaTime * velocity;
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            characterEntity.transform.position[0] += deltaTime * -1 * velocity;

        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            characterEntity.transform.position[1] += deltaTime * velocity;
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            characterEntity.transform.position[1] += deltaTime * -1 * velocity;

        ascrTextSpriteEntityRender(&frameBuffer, &characterEntity);

        int width, height;
        glfwGetFramebufferSize(window, &width, &height);

        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        
        glOrtho( 
            - screen_width / 2, 
              screen_width / 2, 
            - screen_height / 2, 
              screen_height / 2, 
            0.01f, 1000);
        
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

        // TODO : center affecting write char api
        if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            frameBuffer.center[0] += deltaTime * velocity;
        if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            frameBuffer.center[1] += deltaTime * velocity;

        frameBuffer.center[0] = characterEntity.transform.position[0];
        frameBuffer.center[1] = characterEntity.transform.position[1];

        frameBuffer.color_value[0][0][0] = 1.0f;
        frameBuffer.color_value[0][0][1] = 0.0f;
        frameBuffer.color_value[0][0][2] = 0.0f;

        glPushMatrix();
        {
            glColor3f(1.0f, 1.0f, 1.0f);
            stbtt_render();
        }
        glPopMatrix();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    ascrFreeTextSprite(&idleSprite);
    for(size_t idx = 0; idx < walkingSPriteCount; ++idx)
    {
        ascrFreeTextSprite(&walkingSprites[idx]);
    }
    ascrFreeASCRanimationClip(&idleClip);
    ascrFreeASCRanimationClip(&walkingClip);
    ascrFreeASCRanimationState(&characterIdle);
    ascrFreeASCRanimationState(&characterWalking);
    printf("allocated memory = %zu bytes \n", ascrGetAllocatedMemory());

    return 0;
}