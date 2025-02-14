#include "BodyParts.hpp"
#include <iostream>

// ------------------------- BodyPart -------------------------

BodyPart::BodyPart(const glm::vec3& offset, const glm::vec3& color)
    : offset(offset),
      color(color) {
    localTransform = glm::translate(glm::mat4(1.0f), offset);
}

BodyPart::~BodyPart() {
    for (auto child : children) delete child;
}

void BodyPart::addChild(BodyPart* child) {
    children.push_back(child);
}

void BodyPart::updateTransform(const glm::mat4& newTransform) {
    localTransform = newTransform * glm::translate(glm::mat4(1.0f), offset);
}

void BodyPart::draw(const glm::mat4& parentTransform, GLuint shaderProgram) const {
    glm::mat4 globalTransform = parentTransform * localTransform;

    // Envoi des matrices au shader
    GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &globalTransform[0][0]);

    GLuint colorLoc = glGetUniformLocation(shaderProgram, "objectColor");
    glUniform3f(colorLoc, color.r, color.g, color.b);

    // Dessin
    glBindVertexArray(mesh.VAO);
    glDrawElements(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // Dessin des enfants
    for (auto child : children) {
        child->draw(globalTransform, shaderProgram);
    }
}

// ------------------------- Character -------------------------

Character::Character() {
    buildSkeleton();
}

Character::~Character() {
    delete torso; // Destruction récursive
}

void Character::buildSkeleton() {
    // Torso (racine)
    torso = new BodyPart(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.8f, 0.2f, 0.2f));

    // Tête
    head = new BodyPart(glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.9f));
    torso->addChild(head);

    // Bras gauche
    upperArmL = new BodyPart(glm::vec3(-0.5f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.8f));
    forearmL = new BodyPart(glm::vec3(0.0f, -0.8f, 0.0f), glm::vec3(0.4f, 0.4f, 1.0f));
    upperArmL->addChild(forearmL);
    torso->addChild(upperArmL);

    // Bras droit
    upperArmR = new BodyPart(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.2f, 0.2f, 0.8f));
    forearmR = new BodyPart(glm::vec3(0.0f, -0.8f, 0.0f), glm::vec3(0.4f, 0.4f, 1.0f));
    upperArmR->addChild(forearmR);
    torso->addChild(upperArmR);

    // Jambe gauche
    thighL = new BodyPart(glm::vec3(-0.2f, -1.0f, 0.0f), glm::vec3(0.2f, 0.8f, 0.2f));
    calfL = new BodyPart(glm::vec3(0.0f, -0.8f, 0.0f), glm::vec3(0.4f, 1.0f, 0.4f));
    thighL->addChild(calfL);
    torso->addChild(thighL);

    // Jambe droite
    thighR = new BodyPart(glm::vec3(0.2f, -1.0f, 0.0f), glm::vec3(0.2f, 0.8f, 0.2f));
    calfR = new BodyPart(glm::vec3(0.0f, -0.8f, 0.0f), glm::vec3(0.4f, 1.0f, 0.4f));
    thighR->addChild(calfR);
    torso->addChild(thighR);
}

void Character::draw(GLuint shaderProgram) const {
    torso->draw(glm::mat4(1.0f), shaderProgram);
}

void Character::setWalking(bool state) {
    isWalking = state; if (!state) walkAngle = 0.0f; 
}
void Character::animateWalk(float deltaTime) {

    if (!isWalking) return;

    walkAngle += deltaTime * 10.0f;

    float armRotation = sin(walkAngle) * 45.0f;
    float legRotation = cos(walkAngle) * 45.0f;

    // Animation des bras
    upperArmL->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(armRotation), glm::vec3(1, 0, 0)));
    upperArmR->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(-armRotation), glm::vec3(1, 0, 0)));

    // Animation des jambes
    thighL->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(legRotation), glm::vec3(1, 0, 0)));
    thighR->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(-legRotation), glm::vec3(1, 0, 0)));
}

void Character::animateJump(float jumpHeight) {
    glm::mat4 jumpTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, jumpHeight, 0));
    torso->updateTransform(jumpTransform);
}

void Character::resetAnimations() {
    torso->updateTransform(glm::mat4(1.0f));
    walkAngle = 0.0f;
    isWalking = false;
}