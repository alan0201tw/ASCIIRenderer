#include "textsprite.h"

#include <stdio.h>

void create_text_sprite(text_sprite_t* text_sprite, const char* const file_name)
{
    FILE* file = fopen(file_name, "r");
    if(!file)
    {
        printf("SYSERROR : file of name %s not exist! \n", file_name);
        return;
    }

    // the length of the content should not be including new line character
    // and \r
    char c;

    size_t rowCount = 0;
    size_t columnCountEachRow[50] = {0};
    rewind(file);
    while(( c = getc(file)) != EOF)
    {
        if(c == '\n')
            ++rowCount;
        else if (c != '\r')
            ++columnCountEachRow[rowCount];
    }
    // this count variable should be 1 unit larger than the index
    rowCount += 1;

    text_sprite->rowCount = rowCount;
    text_sprite->columnCountEachRow = (size_t*)malloc(sizeof(size_t) * rowCount);

    // printf("rowCount = %ld \n", rowCount);
    // printf("text_sprite->rowCount = %ld \n", text_sprite->rowCount);

    text_sprite->content = (char**)malloc(sizeof(char*) * rowCount);
    for(size_t rowIdx = 0; rowIdx < rowCount; ++rowIdx)
    {
        // printf("columnCountEachRow[%ld] = %ld \n", rowIdx, columnCountEachRow[rowIdx]);
        text_sprite->columnCountEachRow[rowIdx] = columnCountEachRow[rowIdx];
        text_sprite->content[rowIdx] = (char*)malloc(sizeof(char) * columnCountEachRow[rowIdx]);
    }

    size_t row = 0, col = 0;
    rewind(file);
    while(( c = getc(file) ) != EOF)
    {
        if(c == '\n')
        {
            ++row;
            col = 0;
        }
        else if (c != '\r')
        {
            text_sprite->content[row][col] = c;
            ++col;
        }
    }

    fclose(file);
}

void render_text_sprite(frame_buffer_t* const target, const text_sprite_t* const text_sprite)
{
    vec2 position;
    position[0] = text_sprite->m_transform.m_position[0];
    position[1] = text_sprite->m_transform.m_position[1];
    
    float depth = text_sprite->m_transform.m_position[2];

    vec3 color = {1.0f, 1.0f, 1.0f};

    for(size_t rowIdx = 0; rowIdx < text_sprite->rowCount; ++rowIdx)
    {
        for(size_t colIdx = 0; colIdx < text_sprite->columnCountEachRow[rowIdx]; ++colIdx)
        {
            size_t char_position_x = (size_t)position[0] + colIdx;
            // inverse y
            size_t char_position_y = (size_t)position[1] + (text_sprite->rowCount - rowIdx - 1);

            fb_write_char(
                target, char_position_x, char_position_y, depth, color,
                text_sprite->content[rowIdx][colIdx]
                );
        }
    }
}