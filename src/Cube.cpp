#include <cube.hpp>

Cube::Cube(unsigned int size) : size(size) 
{
	float offset = (size - 1) / 2.0f;

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			for (int k = 0; k < size; k++)
			{	
				if (i != size - 1 && i != 0 && j != size - 1 && j != 0
					&& k != size - 1 && k != 0)
					continue;

				float scale = 1.5f / size;
				glm::vec3 pos = glm::vec3(i - offset, j - offset, k - offset);

				Piece piece = Piece(pos, glm::vec3(1.0f, 1.0f, 1.0f), scale);
				pieces.push_back(piece);
			}
		}
	}
}

void Cube::draw()
{
	Shader cubeShader(VSHADER_PATH, FSHADER_PATH);

	for (Piece piece : pieces)
	{
		cubeShader.use();
		piece.draw(cubeShader);
	}
}