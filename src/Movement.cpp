#include "Movement.hpp"
#include "BodyParts.hpp"
#include <cmath>

void MovementController::triggerJump() {
    if (!isJumping) {
        isJumping = true;
        jumpProgress = 0.0f;
    }
}

void MovementController::update(float deltaTime, Character& character) {
    float adjustedDelta = deltaTime * animationSpeed;
    
    character.setWalking(isWalking);
    // Gestion de la marche
    if (isWalking) {
        character.animateWalk(adjustedDelta);
    }

    // Gestion du saut (trajectoire parabolique avec sinus)
    if (isJumping) {
        jumpTimer += adjustedDelta * 2.5f; // Vitesse de saut
        
        // Hauteur du saut : y = sin(jumpTimer) * amplitude
        float jumpHeight = sin(jumpTimer) * 1.5f;
        character.animateJump(jumpHeight);

        // Fin du saut après une demi-période (π radians)
        if (jumpTimer >= glm::pi<float>()) {
            isJumping = false;
            jumpTimer = 0.0f;
            character.resetAnimations();
        }
    }
}