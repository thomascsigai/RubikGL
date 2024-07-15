#include <piece.hpp>

const float cubeVertices[] = {
      // Pos                // Tex Coords
        -0.5f, -0.5f, -0.5f,  0.25f, 0.5f, // Red - Back
         0.5f, -0.5f, -0.5f,  0.5f, 0.5f,
         0.5f,  0.5f, -0.5f,  0.5f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.5f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.25f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.25f, 0.5f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  // Orange - Front
         0.5f, -0.5f,  0.5f,  0.25f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.25f, 0.5f,
         0.5f,  0.5f,  0.5f,  0.25f, 0.5f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.5f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  0.25f, 0.0f, // Green - Left
        -0.5f,  0.5f, -0.5f,  0.5f,  0.0f,               
        -0.5f, -0.5f, -0.5f,  0.5f,  0.5f,               
        -0.5f, -0.5f, -0.5f,  0.5f,  0.5f,               
        -0.5f, -0.5f,  0.5f,  0.25f, 0.5f,              
        -0.5f,  0.5f,  0.5f,  0.25f, 0.0f,              

         0.5f,  0.5f,  0.5f,  0.5f,  0.5f,  // Blue - Right
         0.5f,  0.5f, -0.5f,  0.75f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.75f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.75f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.5f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.5f,  0.5f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f,  // White - Bottom 
         0.5f, -0.5f, -0.5f,  0.25f, 0.5f,                
         0.5f, -0.5f,  0.5f,  0.25f, 1.0f,                
         0.5f, -0.5f,  0.5f,  0.25f, 1.0f,                
        -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,                 
        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f,                 

        -0.5f,  0.5f, -0.5f,  0.75f, 0.5f, //Yellow - Top
         0.5f,  0.5f, -0.5f,  1.0f, 0.5f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.75f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.75f, 0.5f
};

Piece::Piece(glm::vec3 pos, float scale, unsigned int cubeSize) : pos(pos), scale(scale), rot(glm::vec3(0.0f)), orientation(glm::quat(glm::radians(rot))), cubeSize(cubeSize)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    float vertices[180];
    setup_vertices(vertices);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Piece::setup_vertices(float* vertices)
{
    float offset = (cubeSize - 1) / 2.0f;

    for (int i = 0; i < 180; i++)
    {
        vertices[i] = cubeVertices[i];
    }

    if (pos.x + offset != cubeSize - 1)
    {
        // right face becomes black
        vertices[93] = 0.5f;    vertices[94] = 0.0f;
        vertices[98] = 0.75f;   vertices[99] = 0.0f;
        vertices[103] = 0.75f;  vertices[104] = 0.5f;
        vertices[108] = 0.75f;  vertices[109] = 0.5f;
        vertices[113] = 0.5f;   vertices[114] = 0.5f;
        vertices[118] = 0.5f;   vertices[119] = 0.0f;
    }

    if (pos.x + offset != 0)
    {
        //left face becomes black
        vertices[63] = 0.5f;    vertices[64] = 0.0f;
        vertices[68] = 0.75f;   vertices[69] = 0.0f;
        vertices[73] = 0.75f;   vertices[74] = 0.5f;
        vertices[78] = 0.75f;   vertices[79] = 0.5f;
        vertices[83] = 0.5f;    vertices[84] = 0.5f;
        vertices[88] = 0.5f;    vertices[89] = 0.0f;
    }

    if (pos.y + offset != cubeSize - 1)
    {
        //top face becomes black
        vertices[153] = 0.5f;   vertices[154] = 0.0f;
        vertices[158] = 0.75f;  vertices[159] = 0.0f;
        vertices[163] = 0.75f;  vertices[164] = 0.5f;
        vertices[168] = 0.75f;  vertices[169] = 0.5f;
        vertices[173] = 0.5f;   vertices[174] = 0.5f;
        vertices[178] = 0.5f;   vertices[179] = 0.0f;
    }

    if (pos.y + offset != 0)
    {
        //bottom face becomes black
        vertices[123] = 0.5f;   vertices[124] = 0.0f;
        vertices[128] = 0.75f;  vertices[129] = 0.0f;
        vertices[133] = 0.75f;  vertices[134] = 0.5f;
        vertices[138] = 0.75f;  vertices[139] = 0.5f;
        vertices[143] = 0.5f;   vertices[144] = 0.5f;
        vertices[148] = 0.5f;   vertices[149] = 0.0f;
    }

    if (pos.z + offset != cubeSize - 1)
    {
        //back face becomes black
        vertices[33] = 0.5f;    vertices[34] = 0.0f;
        vertices[38] = 0.75f;   vertices[39] = 0.0f;
        vertices[43] = 0.75f;   vertices[44] = 0.5f;
        vertices[48] = 0.75f;   vertices[49] = 0.5f;
        vertices[53] = 0.5f;    vertices[54] = 0.5f;
        vertices[58] = 0.5f;    vertices[59] = 0.0f;
    }

    if (pos.z + offset != 0)
    {
        //back face becomes black
        vertices[3] = 0.5f;     vertices[4] = 0.0f;
        vertices[8] = 0.75f;    vertices[9] = 0.0f;
        vertices[13] = 0.75f;   vertices[14] = 0.5f;
        vertices[18] = 0.75f;   vertices[19] = 0.5f;
        vertices[23] = 0.5f;    vertices[24] = 0.5f;
        vertices[28] = 0.5f;    vertices[29] = 0.0f;
    }
}

void Piece::draw(Shader& shader, unsigned int& texture, float rotationAngle, float zoom, float flipAngle)
{
    apply_transformations(shader, rotationAngle, zoom, flipAngle);
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Piece::apply_transformations(Shader& shader, float rotationAngle, float zoom, float flipAngle)
{
    const float radius = zoom;

    float camX = sin(glm::radians(rotationAngle)) * radius;
    float camZ = cos(glm::radians(rotationAngle)) * radius;

    glm::mat4 view;
    view = glm::lookAt(glm::vec3(camX, 1.5f, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 1600.0f / 900.0f, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(scale));        
    model = glm::rotate(model, glm::radians(flipAngle), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, pos);
    model = model * glm::mat4_cast(orientation);

    int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
}

void Piece::update_rotation(glm::vec3 rotationDelta)
{
    glm::quat qPitch = glm::angleAxis(glm::radians(rotationDelta.x), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat qYaw = glm::angleAxis(glm::radians(rotationDelta.y), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat qRoll = glm::angleAxis(glm::radians(rotationDelta.z), glm::vec3(0.0f, 0.0f, 1.0f));

    orientation = qYaw * qPitch * qRoll * orientation;
    orientation = glm::normalize(orientation);

    rot = glm::eulerAngles(orientation);
    rot = glm::degrees(rot);
}

void Piece::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

glm::vec3 Piece::get_pos()
{
    return pos;
}

void Piece::set_pos(glm::vec3 newPos)
{
    pos = newPos;
}

glm::vec3 Piece::get_rot()
{
    return rot;
}

void Piece::set_rot(glm::vec3 newRot)
{
    rot = newRot;
}