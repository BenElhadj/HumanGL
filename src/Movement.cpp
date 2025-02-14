#include "Movement.hpp"
#include "BodyParts.hpp"
#include <cmath>

void MovementController::triggerJump() {
    if (!isJumping) {
        isJumping = true;
        jumpProgress = 0.0f;
    }
}

void MovementController::adjustSpeed(float factor) {
    animationSpeed = glm::clamp(animationSpeed * factor, MIN_SPEED, MAX_SPEED);
}

void MovementController::toggleWalk() { 
    isWalking = !isWalking; 
}

// void MovementController::update(float deltaTime) {
//     float adjustedDelta = deltaTime * animationSpeed;

//     // Gestion de la marche
//     if (isWalking) {
//         animateWalk(adjustedDelta);
//     }

//     // Gestion du saut (trajectoire parabolique avec sinus)
//     if (isJumping) {
//         jumpTimer += adjustedDelta * 2.5f; // Vitesse de saut
        
//         // Hauteur du saut : y = sin(jumpTimer) * amplitude
//         float jumpHeight = sin(jumpTimer) * 1.5f;
//         animateJump(jumpHeight);

//         // Fin du saut après une demi-période (π radians)
//         if (jumpTimer >= glm::pi<float>()) {
//             isJumping = false;
//             jumpTimer = 0.0f;
//             resetAnimations();
//         }
//     }
// }

void MovementController::animateWalk(float deltaTime) {
    if (!isWalking) return;

    walkAngle += deltaTime * 10.0f;
    float armRotation = sin(walkAngle) * 45.0f;
    float legRotation = cos(walkAngle) * 45.0f;

    // Mise à jour des bras
    character.getUpperArmL()->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(armRotation), glm::vec3(1, 0, 0)));
    character.getUpperArmR()->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(-armRotation), glm::vec3(1, 0, 0)));

    // Mise à jour des jambes
    character.getThighL()->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(legRotation), glm::vec3(1, 0, 0)));
    character.getThighR()->updateTransform(glm::rotate(glm::mat4(1.0f), glm::radians(-legRotation), glm::vec3(1, 0, 0)));
}

void MovementController::animateJump(float jumpHeight) {
    glm::mat4 jumpTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, jumpHeight, 0));
    character.getTorso()->updateTransform(jumpTransform);
}

void MovementController::resetAnimations() {
    character.getTorso()->updateTransform(glm::mat4(1.0f));
    walkAngle = 0.0f;
    isWalking = false;
    isJumping = false;
}

void MovementController::update(float deltaTime) {
    float adjustedDelta = deltaTime * animationSpeed;

    if (isWalking) {
        animateWalk(adjustedDelta);
    }

    if (isJumping) {
        jumpTimer += adjustedDelta * 2.5f;
        animateJump(sin(jumpTimer) * 1.5f);
        
        if (jumpTimer >= glm::pi<float>()) {
            resetAnimations();
        }
    }
}
