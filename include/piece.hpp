#ifndef PIECE_HPP
#define PIECE_HPP

#include <glew.h>
#include <glfw3.h>
#include <shader.hpp>
#include <stb_image.h>

#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Piece
{
public:
	Piece(glm::vec3 pos, glm::vec3 color, float scale);

	void draw(Shader& shader, unsigned int& texture, float rotationAngle, float zoom, float flipAngle);
	void cleanup();

private:
	glm::vec3 pos;
	glm::vec3 color;
	float scale;
	unsigned int VAO, VBO;

	void apply_transformations(Shader& shader, float rotationAngle, float zoom, float flipAngle);
};

#endif