#include <piece.hpp>

const float cubeVertices[] = {
      // Pos                // Tex Coords
        -0.5f, -0.5f, -0.5f,  0.25f, 0.5f, // Red
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
        -0.5f,  0.5f, -0.5f,  0.5f, 0.0f,               
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f,               
        -0.5f, -0.5f, -0.5f,  0.5f, 0.5f,               
        -0.5f, -0.5f,  0.5f,  0.25f, 0.5f,              
        -0.5f,  0.5f,  0.5f,  0.25f, 0.0f,              

         0.5f,  0.5f,  0.5f,  0.5f, 0.5f, // Blue
         0.5f,  0.5f, -0.5f,  0.75f, 0.5f,
         0.5f, -0.5f, -0.5f,  0.75f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.75f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.5f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.5f, 0.5f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.5f,  // White       
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

Piece::Piece(glm::vec3 pos, float scale) : pos(pos), scale(scale), rot(glm::vec3(0.0f))
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);
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
    model = glm::rotate(model, glm::radians(rot.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rot.z), glm::vec3(0.0f, 0.0f, 1.0f));


    int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
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