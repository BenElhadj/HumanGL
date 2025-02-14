#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include <glm/glm.hpp>
class Character; // Forward declaration

class MovementController {
private:
    float animationSpeed = 1.0f; // Facteur de vitesse (1 = normal)
    bool isWalking = false;
    bool isJumping = false;
    float jumpProgress = 0.0f; // Progression du saut [0-π]
    float jumpTimer = 0.0f;    // Timer pour le saut

public:
    MovementController() = default;
    
    void setSpeed(float speed) { animationSpeed = speed; }
    void toggleWalk() { isWalking = !isWalking; }
    void triggerJump();
    void stopAll() { isWalking = false; isJumping = false; }
    bool getWalkState() const { return isWalking; }
    
    void update(float deltaTime, Character& character);
};

#endif