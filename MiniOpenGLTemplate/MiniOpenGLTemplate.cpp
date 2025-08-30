// MiniOpenGLTemplate.cpp : Defines the entry point for the application.
//
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <Graphics.hpp>
#include "MiniOpenGLTemplate.h"
#include <filesystem>
#include <random>

static unsigned int WINDOW_WIDTH  = 800;
static unsigned int WINDOW_HEIGHT = 600;
static float deltaTime = 0.0f; // time between current frame and last frame
static float lastFrame = 0.0f; // time of last frame
static float currentFrame = 0.0f; // time of current frame
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

static float randomFloat(float min, float max) {
    static std::random_device rd;  // obtain a random number from hardware
    static std::mt19937 eng(rd()); // seed the generator
    std::uniform_real_distribution<> distr(min, max); // define the range
	return distr(eng);
}

int main()
{

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MiniOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // init glad before any opengl
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
        std::cerr << "OpenGL Debug Message: " << message << std::endl;
        }, nullptr);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	Graphics graphics("shaders/vertex_shader.glsl", "shaders/fragment_shader.glsl");
    glEnable(GL_DEPTH_TEST);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // set polygon mode to fill

    std::vector<Triangle> triangles(100);
    for (Triangle& t : triangles) {
		float x, y, z;
		x = randomFloat(-1.0f, 1.0f);
		y = randomFloat(-1.0f, 1.0f);
		z = randomFloat(-1.0f, 1.0f);
        t.pos = Point(glm::vec3(
			x, y, z
            ));
    }
    while (!glfwWindowShouldClose(window)) {
        currentFrame = glfwGetTime();
        if (deltaTime >= 1.0f / 60.0f) {
            // check input
            
            // rendering
			graphics.beginFrame();
            // set clear color (it is the background)
            glClearColor(0.2f, 0.5f, 0.4f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the color and depth buffer
            Triangle tri = Triangle(
                Point(glm::vec3(0.0f, 0.5f, 0.0f)),
                Point(glm::vec3(0.5f, 0.0f, 0.0f)),
                Point(glm::vec3(-0.5f, 0.0f, 0.0f)));
			tri.pos = Point(glm::vec3(sin(glfwGetTime()), cos(glfwGetTime()), 0.0f));
            for (Triangle &t : triangles) {
                float x, y, z;
                x = randomFloat(-1.0f, 1.0f);
                y = randomFloat(-1.0f, 1.0f);
                z = randomFloat(-1.0f, 1.0f);
                t.pos = Point(glm::vec3(
                    x, y, z
                ));
				//std::cout << "Drawing triangle at position: (" << t.pos.position.x << ", " << t.pos.position.y << ", " << t.pos.position.z << ")\n";
				//graphics.drawTriangle(t);
                Color color = Color(glm::vec3(
                    (t.pos.position.x + 1.0f) / 2.0f,
                    (t.pos.position.y + 1.0f) / 2.0f,
                    (t.pos.position.z + 1.0f) / 2.0f
				));
				graphics.drawTriangle(t.pos, glm::vec3(1.0f), randomFloat(0.0f, 360.0f), color);

            }
			graphics.endFrame();

            glfwPollEvents();
            glfwSwapBuffers(window);
            deltaTime = deltaTime - 1.0f / 60.0f;
        }
        deltaTime += currentFrame - lastFrame;
        lastFrame = currentFrame;
    }

    glfwTerminate();
    return 0;
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
}

