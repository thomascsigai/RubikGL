#include <cube.hpp>

Cube::Cube(unsigned int size) : size(size) 
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{
				float scale = 1.0f / size;
				glm::vec3 pos = glm::vec3((float)k - 1, (float)j - 1, (float)i - 1);
				Piece piece = Piece(pos, glm::vec3(1.0f, 1.0f, 1.0f), scale);
				pieces.push_back(piece);
			}
		}
	}
}

void Cube::draw()
{
	for (Piece piece : pieces)
	{
		piece.draw();
	}
}