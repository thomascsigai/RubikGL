#ifndef CUBE_HPP
#define CUBE_HPP

#include <piece.hpp>
#include <vector>

#define DEFAULT_SIZE 3

class Cube
{
public:

	Cube(unsigned int size = DEFAULT_SIZE);

	void draw();

private:

	unsigned int size;
	std::vector<Piece> pieces;
};

#endif