#ifndef BODY_PARTS_HPP
#define BODY_PARTS_HPP

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GL/glew.h>

// Structure pour le maillage d'un cube
struct Mesh {
    GLuint VAO, VBO, EBO;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    Mesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        setupCube();
    }

    ~Mesh() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void setupCube() {
        // Cube 1x1x1 avec normales
        vertices = {
            // Positions          // Normales
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
             0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,
            -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f,

            -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
             0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f,

            -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f,

             0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
             0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f,
             0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f,

            -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f
        };

        indices = {
            0,1,2, 2,3,0,
            4,5,6, 6,7,4,
            8,9,10, 10,11,8,
            12,13,14, 14,15,12,
            16,17,18, 18,19,16,
            20,21,22, 22,23,20
        };

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Attributs
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }
};

// ---------------------------------------------------------

class BodyPart {
private:
    glm::mat4 localTransform;
    std::vector<BodyPart*> children;
    Mesh mesh;
    glm::vec3 offset;
    glm::vec3 color;

public:
    BodyPart(const glm::vec3& offset, const glm::vec3& color);
    ~BodyPart();
    void addChild(BodyPart* child);
    void updateTransform(const glm::mat4& newTransform);
    void draw(const glm::mat4& parentTransform, GLuint shaderProgram) const;
};

// ---------------------------------------------------------

class Character {
private:
    BodyPart *torso, *head;
    BodyPart *upperArmL, *forearmL, *upperArmR, *forearmR;
    BodyPart *thighL, *calfL, *thighR, *calfR;
    float walkAngle = 0.0f;
    bool isWalking = false;

    void buildSkeleton();

public:
    Character();
    ~Character();
    void draw(GLuint shaderProgram) const;
    void animateWalk(float deltaTime);
    void toggleWalk() { isWalking = !isWalking; }
    void animateJump(float jumpHeight);
    void setWalking(bool state);
    void resetAnimations(); 
};

#endif