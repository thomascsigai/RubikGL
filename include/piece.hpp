#ifndef PIECE_HPP
#define PIECE_HPP

#include <glew.h>
#include <glfw3.h>
#include <shader.hpp>

#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Piece
{
public:
	Piece(glm::vec3 pos, glm::vec3 color, float scale);

	void draw(Shader& shader);

private:
	glm::vec3 pos;
	glm::vec3 color;
	float scale;
	unsigned int VAO;
};

#endif