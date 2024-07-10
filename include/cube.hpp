#ifndef CUBE_HPP
#define CUBE_HPP

#include <piece.hpp>
#include <vector>
#include <settings.hpp>
#include <random>

#define DEFAULT_SIZE 3
#define ROTATION_DURATION 0.4f
#define SCRAMBLE_ROTATION_DURATION 0.3f

#define VSHADER_PATH "resources/shaders/basicCube.vert"
#define FSHADER_PATH "resources/shaders/basicCube.frag"
#define TEXT_PATH "resources/textures/cubeTexture.png"

enum RotateDirection { line, col, face};

struct RotationParams {
	int faceIndex;
	bool contrary;
	RotateDirection dir;
};

class Cube
{
public:

	Cube(unsigned int size = DEFAULT_SIZE);
	~Cube();

	unsigned int size;

	void draw(SETTINGS settings, GLfloat deltaTime);
	void rotate_face(int faceIndex, bool contrary, RotateDirection dir);
	void scramble();

private:

	std::vector<Piece*> pieces;
	Shader shader;
	unsigned int texture;

	bool rotating = false;
	std::vector<Piece*> rotatingFacePieces = {};
	int rotatingFaceIndex = 0;
	float totalRotationAngle = 0;
	float currentRotationAngle = 0;
	float rotationSpeed;
	RotateDirection rotationDir;

	bool scrambling = false;
	std::vector<RotationParams> rotParams = {};

	void load_texture();
	std::vector<Piece*> get_face_pieces(int faceIndex);
	void update_face_rotation(GLfloat deltaTime);
};

glm::vec3 roundToNearestHalf(glm::vec3 vec);

#endif