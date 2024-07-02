#include <piece.hpp>

const float cubeVertices[] = {
        // Pos--------------Text coords
        -0.5f,  0.5f, 0.5f, -1.0f,  -1.0f, // front vertices
         0.5f,  0.5f, 0.5f, -1.0f,   0.0f,
        -0.5f, -0.5f, 0.5f, -0.15f,  1.0f,
         0.5f, -0.5f, 0.5f, -0.15f,  0.0f,

        -0.5f,  0.5f, 0.5f, -1.0f,  -1.0f,  // back vertices
         0.5f,  0.5f, 0.5f, -1.0f,   0.0f,
        -0.5f, -0.5f, 0.5f, -0.15f,  1.0f,
         0.5f, -0.5f, 0.5f, -0.15f,  0.0f
};

const unsigned int cubeIndices[] = {
        0, 1, 2, // front
        1, 2, 3,

        0, 2, 4, // left
        2, 4, 6,

        1, 5, 3, // right
        3, 5, 7,

        4, 5, 6, // back
        5, 6, 7,

        0, 1, 5, // top
        0, 4, 5,

        2, 6, 7, // bottom
        2, 3, 7
};

Piece::Piece(glm::vec3 pos, glm::vec3 color, float scale) : pos(pos), color(color), scale(scale) 
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndices), cubeIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void Piece::draw(Shader& shader)
{
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -4.0f));
    view = glm::rotate(view, (float)glfwGetTime() * glm::radians(50.0f), glm::vec3(1.0f, 0.3f, 0.5f));

    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(60.0f), 1600.0f / 900.0f, 0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(scale));
    model = glm::translate(model, pos);

    int viewLoc = glGetUniformLocation(shader.ID, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

    int projLoc = glGetUniformLocation(shader.ID, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

    int modelLoc = glGetUniformLocation(shader.ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void Piece::cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}