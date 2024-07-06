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

				Piece* piece = new Piece(pos, scale);
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

void Cube::draw(SETTINGS settings)
{
	for (Piece* piece : pieces)
	{
		shader.use();
		glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0);
		piece->draw(shader, texture, settings.rotationAngle, settings.zoom, settings.flipAngle);
	}

	if (rotating) {
		updateFaceRotation(0.016f); // Assuming 60 FPS, update with 16ms per frame
	}
}

std::vector<Piece*> Cube::get_face_pieces(int faceIndex)
{
	std::vector<Piece*> facePieces;
	float offset = (size - 1) / 2.0f;

	for (Piece* piece : pieces)
	{
		// cols rotation
		if (piece->get_pos().x + offset == faceIndex && rotationDir == col)
		{
			facePieces.push_back(piece);
		}

		// lines rotation
		if (piece->get_pos().y + offset == faceIndex && rotationDir == line)
		{
			facePieces.push_back(piece);
		}

		// faces rotation
		if (piece->get_pos().z + offset == faceIndex && rotationDir == face)
		{
			facePieces.push_back(piece);
		}
	}

	return facePieces;
}

void Cube::rotate_face(int faceIndex, bool contrary, RotateDirection dir)
{
	if (rotating) return;

	float angle = 90.0f, duration = ROTATION_DURATION;
	if (!contrary) angle *= -1;

	rotatingFaceIndex = faceIndex;
	rotationSpeed = angle / duration;
	totalRotationAngle = angle;
	rotationDir = dir;

	rotatingFacePieces = get_face_pieces(faceIndex);

	rotating = true;
	currentRotationAngle = 0.0f;	
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

void Cube::updateFaceRotation(float deltaTime) {
	if (!rotating) return;

	float angleStep = rotationSpeed * deltaTime;
	currentRotationAngle += angleStep;

	glm::vec3 rotVec;
	if (rotationDir == line) rotVec = glm::vec3(0.0f, angleStep, 0.0f);
	if (rotationDir == col) rotVec = glm::vec3(angleStep, 0.0f, 0.0f);
	if (rotationDir == face) rotVec = glm::vec3(0.0f, 0.0f, angleStep);

	// Stop condition
	if ((angleStep >= 0 && currentRotationAngle >= totalRotationAngle) ||
		(angleStep <= 0 && currentRotationAngle <= totalRotationAngle))
	{
		angleStep -= (currentRotationAngle - totalRotationAngle);
		if (rotationDir == line) rotVec = glm::vec3(0.0f, angleStep, 0.0f);
		if (rotationDir == col) rotVec = glm::vec3(angleStep, 0.0f, 0.0f);
		if (rotationDir == face) rotVec = glm::vec3(0.0f, 0.0f, angleStep);
		rotating = false;

		std::cout << "new Pos :" << std::endl;

		for (Piece* piece : rotatingFacePieces)
		{
			piece->set_pos(round(piece->get_pos()));
			piece->set_rot(round(piece->get_rot() + rotVec));
;			std::cout << piece->get_rot().x << " " << piece->get_rot().y << " " << piece->get_rot().z << std::endl;
		}

		std::cout << std::endl << std::endl;
		rotatingFacePieces.clear();
		
	}

	for (Piece* piece : rotatingFacePieces)
	{
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleStep), rotVec / angleStep);
		piece->set_pos(glm::vec3(rotation * glm::vec4(piece->get_pos(), 1.0f)));
		piece->set_rot(piece->get_rot() + rotVec);
	}
}