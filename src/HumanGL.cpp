#include "HumanGL.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
HumanGL::HumanGL() : animationTime(0.0f), leftArmAngle(0), rightArmAngle(0), leftLegAngle(0), rightLegAngle(0) {
    // Initialisation des matrices et transformations
}
HumanGL::~HumanGL() {}

void HumanGL::draw() {
    matrixStack.push();
    drawTorso(); // Dessiner le torse

    // Dessiner la tête
    matrixStack.push();
    matrixStack.translate(0.0f, 1.2f, 0.0f);
    drawHead();
    matrixStack.pop();

    // Bras gauche avec rotation
    matrixStack.push();
    matrixStack.translate(-0.6f, 0.9f, 0.0f);
    matrixStack.rotate(leftArmAngle, 1.0f, 0.0f, 0.0f);
    drawArm(true);
    matrixStack.pop();

    // Bras droit avec rotation
    matrixStack.push();
    matrixStack.translate(0.6f, 0.9f, 0.0f);
    matrixStack.rotate(rightArmAngle, 1.0f, 0.0f, 0.0f);
    drawArm(false);
    matrixStack.pop();

    // Jambe gauche avec rotation
    matrixStack.push();
    matrixStack.translate(-0.3f, -1.0f, 0.0f);
    matrixStack.rotate(leftLegAngle, 1.0f, 0.0f, 0.0f);
    drawLeg(true);
    matrixStack.pop();

    // Jambe droite avec rotation
    matrixStack.push();
    matrixStack.translate(0.3f, -1.0f, 0.0f);
    matrixStack.rotate(rightLegAngle, 1.0f, 0.0f, 0.0f);
    drawLeg(false);
    matrixStack.pop();

    matrixStack.pop(); // Fin du torse
}

void HumanGL::drawPart()
{
    glPushMatrix(); // Sauvegarde de la transformation actuelle

    // Appliquer la transformation de `matrixStack` à OpenGL
    glMultMatrixf(matrixStack.getData().data());

    std::cout << "Applying Transformation Matrix: ";
    for (float value : matrixStack.getData()) {
        std::cout << value << " ";
    }
    std::cout << std::endl;

    // Dessiner un cube (corps, tête, bras, etc.)
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glEnd();

    glPopMatrix(); // Restaurer l'ancienne transformation
}

void HumanGL::drawArm(bool isLeft) {
    drawUpperArm(isLeft); // Dessiner le bras supérieur

    // Dessiner l'avant-bras attaché au bras supérieur
    matrixStack.push();
    drawForearm(isLeft);
    matrixStack.pop();
}

void HumanGL::drawLeg(bool isLeft) {
    drawThigh(isLeft); // Dessiner la cuisse

    // Dessiner la partie inférieure de la jambe attachée à la cuisse
    matrixStack.push();
    drawLowerLeg(isLeft);
    matrixStack.pop();
}
void HumanGL::drawForearm(bool isLeft)
{
    float sign = isLeft ? -1.0f : 1.0f;
    matrixStack.translate(sign * 0.75f, -0.75f, 0.0f);
    matrixStack.scale(0.25f, 0.75f, 0.25f);
    drawPart();
}
void HumanGL::drawLowerLeg(bool isLeft)
{
    float sign = isLeft ? -0.5f : 0.5f;
    matrixStack.translate(sign, -1.5f, 0.0f);
    matrixStack.scale(0.25f, 0.75f, 0.25f);
    drawPart();
}
void HumanGL::drawHead() {
    glColor3f(1.0f, 0.0f, 0.0f); // Rouge pour la tête
    matrixStack.scale(0.5f, 0.5f, 0.5f); // Taille de la tête
    drawPart();
}

void HumanGL::drawTorso() {
    glColor3f(0.0f, 1.0f, 0.0f); // Vert pour le torse
    matrixStack.scale(1.0f, 1.5f, 0.5f); // Taille du torse
    drawPart();
}
void HumanGL::drawUpperArm(bool isLeft)
{
    float sign = isLeft ? -1.0f : 1.0f;
    matrixStack.translate(sign * 0.75f * armScaleFactor, 0.5f * armScaleFactor, 0.0f);
    matrixStack.scale(0.25f * armScaleFactor, 0.75f * armScaleFactor, 0.25f * armScaleFactor);
    drawPart();
}
void HumanGL::drawThigh(bool isLeft)
{
    float sign = isLeft ? -0.5f : 0.5f;
    matrixStack.translate(sign * legScaleFactor, -0.75f * legScaleFactor, 0.0f);
    matrixStack.scale(0.25f * legScaleFactor, 0.75f * legScaleFactor, 0.25f * legScaleFactor);
    drawPart();
}
void HumanGL::update() {
    animationTime += 0.1f;
    float angle = sin(animationTime) * 30.0f;
    leftArmAngle = angle;
    rightArmAngle = -angle;
    leftLegAngle = -angle;   // Rotation opposée pour les jambes
    rightLegAngle = angle;
}
