#include "SheetSprite.h"
SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height, float size) :
	textureID(textureID), u(u), v(v), width(width), height(height), size(size), reversedImage(false) {}

SheetSprite::SheetSprite(unsigned int textureID, int index, int spriteX, int spriteY, float aspect, float size) :
	textureID(textureID), aspect(aspect), size(size), reversedImage(false) {
	
	width = 1.0 / (float)spriteX;
	height = 1.0 / (float)spriteY;
	u = (float)(index % spriteX) / (float)spriteX;
	v = (float)(index / spriteX) / (float)spriteY;
}
void SheetSprite::Draw(ShaderProgram *program) {
	glBindTexture(GL_TEXTURE_2D, textureID);
	float aspect = this->aspect;

	GLfloat texCoords[] = {
		u, v + height,
		u + width, v,
		u, v,
		u + width, v,
		u, v + height,
		u + width, v + height
	};

	if (this->reversedImage == true) {
		aspect *=  -1;
	}
	float vertices[] = {
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, 0.5f * size,
		0.5f * size * aspect, 0.5f * size,
		-0.5f * size * aspect, -0.5f * size,
		0.5f * size * aspect, -0.5f * size };
	
	glVertexAttribPointer(program->positionAttribute, 2, GL_FLOAT, false, 0, vertices);
	glEnableVertexAttribArray(program->positionAttribute);
	glVertexAttribPointer(program->texCoordAttribute, 2, GL_FLOAT, false, 0, texCoords);
	glEnableVertexAttribArray(program->texCoordAttribute);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	glDisableVertexAttribArray(program->positionAttribute);
	glDisableVertexAttribArray(program->texCoordAttribute);

}