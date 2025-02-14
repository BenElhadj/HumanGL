#include "Camera.hpp"

void Camera::moveHorizontal(float delta) {
    theta += delta;
    // Boucle entre 0 et 2π pour éviter les overflow
    theta = glm::mod(theta, glm::two_pi<float>());
}

void Camera::moveVertical(float delta) {
    phi += delta;
    // Limite l'angle vertical
    phi = glm::clamp(phi, MIN_PHI, MAX_PHI);
}

void Camera::zoomIn() {
    radius -= 0.5f;
    radius = glm::clamp(radius, MIN_RADIUS, MAX_RADIUS);
}

void Camera::zoomOut() {
    radius += 0.5f;
    radius = glm::clamp(radius, MIN_RADIUS, MAX_RADIUS);
}

glm::mat4 Camera::getViewMatrix() const {
    // Conversion coordonnées sphériques → cartésiennes
    glm::vec3 eye(
        radius * sin(theta) * sin(phi),
        radius * cos(phi),
        radius * cos(theta) * sin(phi)
    );

    return glm::lookAt(
        eye + center,       // Position caméra
        center,             // Point cible (le personnage)
        glm::vec3(0, 1, 0)  // Vecteur "haut"
    );
}