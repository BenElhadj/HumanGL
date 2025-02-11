#include "HumanGL.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
HumanGL::HumanGL() : animationTime(0.0f)
{
    // Initialisation des matrices et transformations
}
HumanGL::~HumanGL() {}
void HumanGL::draw() {
    matrixStack.push();
    drawTorso(); // Dessiner le torse

    // Dessiner la tête attachée au torse
    matrixStack.push();
    matrixStack.translate(0.0f, 1.2f, 0.0f); // Position de la tête par rapport au torse
    drawHead();
    matrixStack.pop();

    // Dessiner le bras gauche
    matrixStack.push();
    matrixStack.translate(-0.6f, 0.9f, 0.0f); // Position du bras gauche par rapport au torse
    drawArm(true);
    matrixStack.pop();

    // Dessiner le bras droit
    matrixStack.push();
    matrixStack.translate(0.6f, 0.9f, 0.0f); // Position du bras droit par rapport au torse
    drawArm(false);
    matrixStack.pop();

    // Dessiner la jambe gauche
    matrixStack.push();
    matrixStack.translate(-0.3f, -1.0f, 0.0f); // Position de la jambe gauche par rapport au torse
    drawLeg(true);
    matrixStack.pop();

    // Dessiner la jambe droite
    matrixStack.push();
    matrixStack.translate(0.3f, -1.0f, 0.0f); // Position de la jambe droite par rapport au torse
    drawLeg(false);
    matrixStack.pop();

    matrixStack.pop(); // Fin du torse
}
void HumanGL::drawPart()
{
    glBegin(GL_QUADS);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glEnd();
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

    // Mouvement du bras gauche
    matrixStack.push();
    matrixStack.translate(-0.6f, 0.9f, 0.0f); // Position initiale du bras gauche
    matrixStack.rotate(angle, 1.0f, 0.0f, 0.0f); // Rotation du bras gauche
    drawArm(true);
    matrixStack.pop();

    // Mouvement du bras droit
    matrixStack.push();
    matrixStack.translate(0.6f, 0.9f, 0.0f); // Position initiale du bras droit
    matrixStack.rotate(-angle, 1.0f, 0.0f, 0.0f); // Rotation du bras droit
    drawArm(false);
    matrixStack.pop();

    // Mouvement de la jambe gauche
    matrixStack.push();
    matrixStack.translate(-0.3f, -1.0f, 0.0f); // Position initiale de la jambe gauche
    matrixStack.rotate(-angle, 1.0f, 0.0f, 0.0f); // Rotation de la jambe gauche
    drawLeg(true);
    matrixStack.pop();

    // Mouvement de la jambe droite
    matrixStack.push();
    matrixStack.translate(0.3f, -1.0f, 0.0f); // Position initiale de la jambe droite
    matrixStack.rotate(angle, 1.0f, 0.0f, 0.0f); // Rotation de la jambe droite
    drawLeg(false);
    matrixStack.pop();
}