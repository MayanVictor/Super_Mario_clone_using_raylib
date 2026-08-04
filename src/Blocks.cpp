#include"Blocks.hpp"
#include<raylib.h>

Block::Block(Rectangle rect, BlockType blockType, BlockContent Content) 
        : hitbox(rect), type(blockType), content(Content), isUsed(false), isBroken(false) {}
void Block::draw() 
{
    Color blockColor;
    switch (type) 
    {
        case SOLID:
            blockColor = GRAY;
            break;
        case BRICK:
            blockColor = BROWN;
            break;
        case QUESTION:
            blockColor = YELLOW;
            break;
    }
    DrawRectangleRec(hitbox, blockColor);
}
Rectangle Block::getHitbox() const 
{
    return hitbox;
}
BlockContent Block::getContent() const 
{
    return content;
}
BlockType Block::getType() const 
{
    return type;
}