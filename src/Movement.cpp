#include "Movement.hpp"
#include "BodyParts.hpp"
#include <cmath>

void MovementController::stopAll() {
    currentState = IDLE;
    pendingJump = false;
    pendingWalk = false;
    walkAngle = 0.0f;
    character.getTorso()->updateTransform(glm::mat4(1.0f));
}


void MovementController::adjustSpeed(float factor) {
    animationSpeed = glm::clamp(animationSpeed * factor, MIN_SPEED, MAX_SPEED);
}

void MovementController::animateWalk(float deltaTime) {
    if (currentState != WALKING && currentState != STOPPING) return;

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
    float adjustedHeight = jumpHeight;
    if (jumpTimer > glm::pi<float>()/2) {
        adjustedHeight *= 1.0f - (jumpTimer - glm::pi<float>()/2)/(glm::pi<float>()/2);
    }
    glm::mat4 jumpTransform = glm::translate(glm::mat4(1.0f), glm::vec3(0, jumpHeight, 0));
    character.getTorso()->updateTransform(jumpTransform);
}

void MovementController::resetAnimations() {
    // Réinitialisation progressive des angles
    if (walkAngle > 0.1f) {
        walkAngle *= 0.9f; // Décélération douce
    } else {
        character.getTorso()->updateTransform(glm::mat4(1.0f));
        walkAngle = 0.0f;
    }
}

void MovementController::toggleWalk() {
    inWalk = !inWalk;
    switch(this->currentState) {
        case IDLE:
            currentState = WALKING;
            walkAngle = 0.0f;
            break;
            
        case WALKING:
            currentState = STOPPING;
            break;
            
        case JUMPING:
            pendingWalk = true;
            break;
            
        case STOPPING:
            break;
    }
}

void MovementController::triggerJump() {
    currentState == JUMPING ? ((savedJumpTimer = jumpTimer) && (jumpPaused = true)) : jumpPaused = false;
    switch(currentState) {
        case IDLE:
            currentState = JUMPING;
            jumpTimer = 0.0f;
            break;
            
        case WALKING:
            currentState = STOPPING;
            pendingJump = true;
            break;
            
        case JUMPING:
            currentState = IDLE;
            character.getTorso()->updateTransform(glm::mat4(1.0f));
            jumpTimer = jumpPaused ? jumpTimer : glm::pi<float>();
            break;
            
        case STOPPING:
            pendingJump = true;
            break;
    }
}

void MovementController::update(float deltaTime) {
    float adjustedDelta = deltaTime * animationSpeed;

    switch(currentState) {
        case WALKING:
            animateWalk(adjustedDelta);
            break;

        case JUMPING: {
            if (jumpPaused) break;

            jumpTimer += adjustedDelta * 2.5f;
            float height = sin(jumpTimer) * 1.5f;
            animateJump(height);
            
            if (jumpTimer >= glm::pi<float>()) {
                currentState = IDLE;
                character.getTorso()->updateTransform(glm::mat4(1.0f));
                if (pendingWalk) {
                    currentState = WALKING;
                    pendingWalk = false;
                }
                currentState = (inWalk ? WALKING : currentState);
            }
            break;
        }
            
        case STOPPING:
            if (walkAngle - glm::floor(walkAngle/(2*glm::pi<float>()))*2*glm::pi<float>() < 0.1f) {
                currentState = IDLE;
                walkAngle = 0.0f;
                if (pendingJump) {
                    currentState = JUMPING;
                    jumpTimer = 0.0f;
                    pendingJump = false;
                }
            } else {
                animateWalk(adjustedDelta);
            }
            break;
            
        case IDLE:
            break;
    }
}
