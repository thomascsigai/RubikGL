#ifndef CUBE_HPP
#define CUBE_HPP

#include <piece.hpp>
#include <vector>

#define DEFAULT_SIZE 3
#define VSHADER_PATH "resources/shaders/basicCube.vert"
#define FSHADER_PATH "resources/shaders/basicCube.frag"


class Cube
{
public:

	Cube(unsigned int size = DEFAULT_SIZE);

	void draw();

private:

	unsigned int size;
	std::vector<Piece> pieces;
	Shader shader;
};

#endif