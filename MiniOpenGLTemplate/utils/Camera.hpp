#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>

class Camera {
public:
	Camera(glm::vec3 pos, glm::vec3 front, float pitch, float yaw, float roll, float fov = 45.0f, float aspectRatio = 4.0f/3.0f, glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
	float near = 0.1f, float far = 100.0f) {
		this->pos = pos;
		this->front = front;
		this->pitch = pitch;
		this->yaw = yaw;
		this->roll = roll;
		this->fov = fov;
		this->aspectRatio = aspectRatio;
		this->worldUp = worldUp;
		this->near = 0.1f;
		this->far = 100.0f;
		view = glm::lookAt(pos, pos + front, worldUp);
		projection = glm::perspective(glm::radians(fov), aspectRatio, near, far);
		isProjectionChanged = false;
		isViewChanged = false;
	}
	~Camera() {};

	void update(float dt);
	
	float* getViewMatrix() {
        return glm::value_ptr(view);
    }

	float* getProjectionMatrix() {
        return glm::value_ptr(projection);
    }

	void lookAround(float xoffset, float yoffset, bool constrainPitch = true);
	void moveForward(float dt);
	void moveBackward(float dt);
	void moveLeft(float dt);
	void moveRight(float dt);
	void zoomIn(float dt);
	void zoomOut(float dt);
	void shake(float intensity, float duration);

	float speed       = 1.0f; // movement speed
	float sensitivity = 0.1f; // mouse sensitivity
	float zoomSpeed   = 1.0f; // zoom speed

private:
	// angles always in radians
	float pitch;
	float yaw;
	float roll;
	float fov;
	glm::mat4 view;
	glm::mat4 projection;
	bool isProjectionChanged;
	bool isViewChanged;
	float aspectRatio;
	float near;
	float far;
	glm::vec3 pos;
	glm::vec3 front;
	glm::vec3 worldUp;
	float dt = 0.0f; //current delta time passed in update function
	bool isShaking = false;
	float shakeIntensity = 0.0f;
	float shakeDuration = 0.0f;
};
