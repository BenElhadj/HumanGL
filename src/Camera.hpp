#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
    float radius = 8.0f;    // Distance initiale
    float theta = 0.0f;     // Angle horizontal (radians)
    float phi = glm::radians(45.0f); // Angle vertical (radians)
    glm::vec3 center = glm::vec3(0.0f); // Centre de rotation

    // Limites pour éviter les inversions
    const float MIN_PHI = glm::radians(10.0f);
    const float MAX_PHI = glm::radians(170.0f);
    const float MIN_RADIUS = 1.5f;
    const float MAX_RADIUS = 15.0f;

public:
    Camera() = default;
    
    void moveHorizontal(float delta); // Flèches gauche/droite
    void moveVertical(float delta);   // Flèches haut/bas
    void zoomIn();                    // Touche 'A'
    void zoomOut();                   // Touche 'R'
    
    glm::mat4 getViewMatrix() const;
};

#endif