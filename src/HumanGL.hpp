#ifndef HUMANGL_HPP
#define HUMANGL_HPP
#include "Matrix.hpp"
#include <vector>
class HumanGL
{
public:
    float animationTime = 0.0f;
    float torsoScaleFactor = 1.0f;
    float armScaleFactor = 1.0f;
    float legScaleFactor = 1.0f;
    HumanGL();
    ~HumanGL();
    void draw();
    void update();

private:
    Matrix matrixStack;
    void drawPart();
    void drawHead();
    void drawTorso();
    void drawUpperArm(bool isLeft);
    void drawForearm(bool isLeft);
    void drawThigh(bool isLeft);
    void drawLowerLeg(bool isLeft);
    void drawArm(bool isLeft);
    void drawLeg(bool isLeft);
};
#endif