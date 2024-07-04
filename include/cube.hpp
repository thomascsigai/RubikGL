#ifndef CUBE_HPP
#define CUBE_HPP

#include <piece.hpp>
#include <vector>
#include <settings.hpp>

#define DEFAULT_SIZE 3
#define VSHADER_PATH "resources/shaders/basicCube.vert"
#define FSHADER_PATH "resources/shaders/basicCube.frag"
#define TEXT_PATH "resources/textures/cubeTexture.png"

class Cube
{
public:

	Cube(unsigned int size = DEFAULT_SIZE);
	~Cube();

	unsigned int size;

	void draw(SETTINGS settings);

private:

	std::vector<Piece*> pieces;
	Shader shader;
	unsigned int texture;

	void load_texture();
};

#endif