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
		update_face_rotation(0.016f); // Assuming 60 FPS, update with 16ms per frame
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

void Cube::update_face_rotation(float deltaTime) {
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

		for (Piece* piece : rotatingFacePieces)
		{
			if (size % 2 == 0)
				piece->set_pos(roundToNearestHalf(piece->get_pos()));
			else
				piece->set_pos(round(piece->get_pos()));

			piece->update_rotation(rotVec);
		}

		if (rotParams.empty())
			scrambling = false;

		if (scrambling)
		{
			rotate_face(rotParams[0].faceIndex, rotParams[0].contrary, rotParams[0].dir);
			rotParams.erase(rotParams.begin());
			
		}
	}

	for (Piece* piece : rotatingFacePieces)
	{
		glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angleStep), rotVec / angleStep);
		piece->set_pos(glm::vec3(rotation * glm::vec4(piece->get_pos(), 1.0f)));
		piece->update_rotation(rotVec);
	}
}

void Cube::scramble()
{
	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<> faceIndexDist(0, size - 1);
	std::uniform_int_distribution<> boolDist(0, 1);
	std::uniform_int_distribution<> dirDist(0, 2);

	scrambling = true;

	for (int i = 0; i < 10 * size; i++) {
		RotationParams r;

		r.faceIndex = faceIndexDist(gen);
		r.contrary = boolDist(gen) == 0 ? true : false;

		switch (dirDist(gen)) {
		case 0:
			r.dir = col;
			break;
		case 1:
			r.dir = line;
			break;
		case 2:
			r.dir = face;
			break;
		}

		rotParams.push_back(r);
	}

	rotate_face(rotParams[0].faceIndex, rotParams[0].contrary, rotParams[0].dir);
}

glm::vec3 roundToNearestHalf(glm::vec3 vec) {
	float x = round(vec.x * 2.0) / 2.0;
	float y = round(vec.y * 2.0) / 2.0;
	float z = round(vec.z * 2.0) / 2.0;

	return glm::vec3(x, y, z);
}