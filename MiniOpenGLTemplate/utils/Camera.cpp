#include <Camera.hpp>
#include <iostream>


void Camera::update(float dt) {
    this->dt = dt;
    if (isShaking) {
        shake(shakeIntensity, shakeDuration);
    }

    if (isViewChanged) {
        view = glm::lookAt(pos, pos + front, worldUp);
        isViewChanged = false;
    }
    
    if (isProjectionChanged) {
        projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
        isProjectionChanged = false;
    }
}


void Camera::moveForward(float dt) {
    // front is already toward the point we want move
    // so simply we move along the front vector
    // by multiplying it with speed and dt and adding to pos
    pos += front * speed * dt;
    pos.y = 0.0f;
    isViewChanged = true;
}


void Camera::moveBackward(float dt) {
    pos -= front * speed * dt;
    // TODO: to make camera generic or create a mechanism to have different camera
    // maybe create a Camera class for each type of camera or use inheritance
    // or use a design pattern to handle different camera types implementation
    pos.y = 0.0f; // keep camera on the ground
    isViewChanged = true;
}


void Camera::moveLeft(float dt) {
    // to move left we need to calculate the right vector respect to the front and up
    // which is perpendicular to the front vector and the world up vector
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    pos -= right * speed * dt;
    isViewChanged = true;
}


void Camera::moveRight(float dt) {
    glm::vec3 right = glm::normalize(glm::cross(front, worldUp));
    pos += right * speed * dt;
    isViewChanged = true;
}

void Camera::zoomIn(float dt) {
    fov -= zoomSpeed * dt;
    if (fov < 1.0f) {
        fov = 1.0f; // prevent zooming in too much
    }
    isProjectionChanged = true;
}

void Camera::zoomOut(float dt) {
    fov += zoomSpeed * dt;
    if (fov >  179.0f) {
        fov = 179.0f; // prevent zooming out too much
    }
    isProjectionChanged = true;
}


void Camera::lookAround(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= sensitivity;
    yoffset *= sensitivity;
    yaw += xoffset;
    pitch += yoffset;
    if (constrainPitch) {
        if (pitch > 89.0f) {
            pitch = 89.0f; // prevent looking up too much otherwise the camera invert the look
        } else if (pitch < -89.0f) {
            pitch = -89.0f; // prevent looking down too much otherwise the camera invert the look
        }
    }
    // convert polar coordinates (yaw, pitch) to cartesian coordinates
    front = glm::normalize(glm::vec3(
        //x
        cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        //y
        sin(glm::radians(pitch)),
        //z
        sin(glm::radians(yaw)) * cos(glm::radians(pitch))));

    isViewChanged = true; // we need to update the view matrix
}



void Camera::shake(float intensity, float duration) {
    static float shakeTime = 0.0f;
    static glm::vec3 originalPos = pos;
    shakeIntensity = intensity;
    shakeDuration = duration;
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_real_distribution<> dist(0.0f,1.0f); // distribution in range [0, 1]
    if (!isShaking) {
        isShaking = true;
        originalPos = pos;
        shakeTime = 0.0f;
    }
    else {
        shakeTime += this->dt;
        pos += glm::vec3(
            dist(rng) * shakeIntensity - shakeIntensity / 2.0f, // random x offset
            dist(rng) * shakeIntensity - shakeIntensity / 2.0f, // random y offset
            dist(rng) * shakeIntensity - shakeIntensity / 2.0f); // random z offset
        isViewChanged = true; // we need to update the view matrix after shaking
        if (shakeTime >= shakeDuration) {
            isShaking = false;
            pos = originalPos; // reset position to original
            shakeTime = 0.0f; // reset shake time
            shakeIntensity = 0.0f; // reset shake intensity
            shakeDuration = 0.0f; // reset shake duration
        }
    }
}