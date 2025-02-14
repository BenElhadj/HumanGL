#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include <glm/glm.hpp>
class Character; // Forward declaration

class MovementController {
private:
    Character& character;
    float animationSpeed = 0.7f; // Facteur de vitesse (1 = normal)
    float walkAngle = 0.0f;
    bool isWalking = false;
    bool isJumping = false;
    float jumpProgress = 0.0f; // Progression du saut [0-π]
    float jumpTimer = 0.0f; // Timer pour le saut
    static constexpr float MIN_SPEED = 0.1f;
    static constexpr float MAX_SPEED = 5.0f;

public:
    // void setSpeed(float speed) { animationSpeed = speed; }
    explicit MovementController(Character& chara) : character(chara) {}
    void adjustSpeed(float factor);
    void setSpeed(float speed) { 
        animationSpeed = glm::clamp(speed, MIN_SPEED, MAX_SPEED); 
    }
    void toggleWalk();
    void triggerJump();
    void stopAll() { isWalking = false; isJumping = false; }
    bool getWalkState() const { return isWalking; }
    
    // void update(float deltaTime, Character& character);
    void update(float deltaTime);
    void animateWalk(float deltaTime);
    void animateJump(float jumpHeight);
    void resetAnimations();
};

#endif