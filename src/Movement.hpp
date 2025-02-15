#ifndef MOVEMENT_HPP
#define MOVEMENT_HPP

#include <glm/glm.hpp>
class Character; // Forward declaration

class MovementController {
public:
    enum State { IDLE, WALKING, JUMPING, STOPPING };
    bool getWalkState() const { return currentState == WALKING; }

    explicit MovementController(Character& chara) : character(chara) {}
    
    void toggleWalk();
    void triggerJump();
    void stopAll();
    void update(float deltaTime);
    void adjustSpeed(float factor);

    void setSpeed(float speed) { 
        animationSpeed = glm::clamp(speed, MIN_SPEED, MAX_SPEED); 
    }
    void animateWalk(float deltaTime);
    void animateJump(float jumpHeight);
    void resetAnimations();
private:
    Character& character;
    State currentState = IDLE;
    
    float animationSpeed = 0.7f; // Facteur de vitesse (1 = normal)
    float walkAngle = 0.0f;
    float jumpTimer = 0.0f; // Timer pour le saut
    float savedJumpTimer = 0.0f;
    
    bool jumpPaused = false;
    bool pendingJump = false;
    bool pendingWalk = false;
    bool inWalk = false;
    
    float jumpProgress = 0.0f; // Progression du saut [0-π]

    static constexpr float MIN_SPEED = 0.1f;
    static constexpr float MAX_SPEED = 5.0f;

};

#endif