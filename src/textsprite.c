#include "textsprite.h"

#include <stdio.h>

void ascrTextSpriteCreateFromFile(ASCRtextSprite* textSprite, const char* const file_name)
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

    textSprite->rowCount = rowCount;
    textSprite->columnCountEachRow = (size_t*)malloc(sizeof(size_t) * rowCount);

    // printf("rowCount = %ld \n", rowCount);
    // printf("text_sprite->rowCount = %ld \n", text_sprite->rowCount);

    textSprite->content = (char**)malloc(sizeof(char*) * rowCount);
    for(size_t rowIdx = 0; rowIdx < rowCount; ++rowIdx)
    {
        // printf("columnCountEachRow[%ld] = %ld \n", rowIdx, columnCountEachRow[rowIdx]);
        textSprite->columnCountEachRow[rowIdx] = columnCountEachRow[rowIdx];
        textSprite->content[rowIdx] = (char*)malloc(sizeof(char) * columnCountEachRow[rowIdx]);
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
            textSprite->content[row][col] = c;
            ++col;
        }
    }

    fclose(file);
}

void ascrTextSpriteEntityRender(ASCRframeBuffer* const target, const ASCRtextSpriteEntity* const entity)
{
    vec2 position;
    position[0] = entity->transform.position[0];
    position[1] = entity->transform.position[1];
    
    float depth = entity->transform.position[2];

    vec3 color = {1.0f, 1.0f, 1.0f};

    for(size_t rowIdx = 0; rowIdx < entity->textSprite->rowCount; ++rowIdx)
    {
        for(size_t colIdx = 0; colIdx < entity->textSprite->columnCountEachRow[rowIdx]; ++colIdx)
        {
            size_t char_position_x = (size_t)position[0] + colIdx;
            // inverse y
            size_t char_position_y = (size_t)position[1] + (entity->textSprite->rowCount - rowIdx - 1);

            ascrFrameBufferWriteChar(
                target, char_position_x, char_position_y, depth, color,
                entity->textSprite->content[rowIdx][colIdx]
                );
        }
    }
}