#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include "BodyParts.hpp"
#include "Camera.hpp"
#include "Movement.hpp"

// Variables globales
Camera camera;
// MovementController movementCtrl;
MovementController* movementCtrl = nullptr;
// Character character;
GLuint shaderProgram;
float lastFrame = 0.0f;

// Shaders simples (à intégrer dans des fichiers séparés en production)
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec3 aNormal;
    
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    uniform vec3 objectColor;
    out vec4 FragColor;
    
    void main() {
        FragColor = vec4(objectColor, 1.0);
    }
)";

// Callback clavier
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    (void)scancode;
    (void)mods;
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        switch(key) {
            case GLFW_KEY_LEFT:     camera.moveHorizontal(-0.05f);                 break;
            case GLFW_KEY_RIGHT:    camera.moveHorizontal(0.05f);                  break;
            case GLFW_KEY_DOWN:     camera.moveVertical(0.05f);                    break;
            case GLFW_KEY_UP:       camera.moveVertical(-0.05f);                   break;
            case GLFW_KEY_A:        camera.zoomIn();                               break;
            case GLFW_KEY_R:        camera.zoomOut();                              break;
            case GLFW_KEY_S:        movementCtrl->adjustSpeed(0.9f);               break;
            case GLFW_KEY_F:        movementCtrl->adjustSpeed(1.1f);               break;
            case GLFW_KEY_W:        movementCtrl->toggleWalk();                    break;
            case GLFW_KEY_J:        movementCtrl->triggerJump();                   break;
            case GLFW_KEY_SPACE:    movementCtrl->stopAll();                       break;
            case GLFW_KEY_ESCAPE:   glfwSetWindowShouldClose(window, GL_TRUE);     break;
        }
    }
}

// Initialisation des shaders
GLuint compileShaders() {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

int main() {
    // Initialisation GLFW
    if (!glfwInit()) return -1;

    // Création fenêtre
    GLFWwindow* window = glfwCreateWindow(1280, 720, "HumanGL", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    // Initialisation GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Erreur GLEW" << std::endl;
        return -1;
    }

    // Création du personnage
    Character character;
    movementCtrl = new MovementController(character);

    // Configuration OpenGL
    glEnable(GL_DEPTH_TEST);
    shaderProgram = compileShaders();
    glUseProgram(shaderProgram);

    // Matrice de projection
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f/720.0f, 0.1f, 100.0f);
    GLuint projLoc = glGetUniformLocation(shaderProgram, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection[0][0]);

    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        // Calcul deltaTime
        float currentFrame = glfwGetTime();
        float deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Nettoyage écran
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Mise à jour des composants
        movementCtrl->update(deltaTime);

        // Rendu
        glm::mat4 view = camera.getViewMatrix();
        GLuint viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);

        character.draw(shaderProgram);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    delete movementCtrl;
    glfwTerminate();
    return 0;
}