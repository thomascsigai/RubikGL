#include <cube.hpp>

Cube::Cube(unsigned int size) : size(size), shader(VSHADER_PATH, FSHADER_PATH)
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

				std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;

				Piece* piece = new Piece(pos, glm::vec3(1.0f, 1.0f, 1.0f), scale);
				pieces.push_back(piece);
			}
		}
	}
	load_texture();
}

Cube::~Cube()
{
	for (Piece* piece : pieces)
	{
		piece->cleanup();
		delete piece;
	}
	pieces.clear();
}

void Cube::draw()
{
	for (Piece* piece : pieces)
	{
		shader.use();
		glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
		piece->draw(shader, texture);
	}
}

void Cube::load_texture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(TEXT_PATH, &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}