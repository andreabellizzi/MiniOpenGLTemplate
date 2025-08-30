#pragma once
#include <vector>
#include <string>
#include <array>
#include <glad/glad.h>
#include <Shaders.hpp>
#include <glm/glm.hpp>
#include <Camera.hpp>

struct Point {
	Point(glm::vec3 position) : position(position) {};
	Point() : position(glm::vec3(0.0f, 0.0f, 0.0f)) {}; // default constructor

	glm::vec3 position;
};

struct Color {
	Color(glm::vec3 color) : color(color) {};
	glm::vec3 color;
};

struct Vertex {
	Point point; // position of the vertex
	Color color; // color of the vertex
};

struct Triangle {

public:
	Triangle() {
		vertices.push_back(Vertex{ Point(glm::vec3(0.0f, 0.5f, 0.0f)), Color(glm::vec3(1.0f, 0.0f, 0.0f)) }); // default color red
		vertices.push_back(Vertex{ Point(glm::vec3(0.5f, 0.0f, 0.0f)), Color(glm::vec3(0.0f, 1.0f, 0.0f)) }); // default color green
		vertices.push_back(Vertex{ Point(glm::vec3(-0.5f, 0.0f, 0.0f)), Color(glm::vec3(0.0f, 0.0f, 1.0f)) }); // default color blue
		pos = glm::vec3(0.0f, 0.0f, 0.0f); // default position
	}

	Triangle(Point p1, Point p2, Point p3) {
		vertices.push_back(Vertex{ p1, Color(glm::vec3(1.0f, 0.0f, 0.0f)) }); // default color red
		vertices.push_back(Vertex{ p2, Color(glm::vec3(0.0f, 1.0f, 0.0f)) }); // default color green
		vertices.push_back(Vertex{ p3, Color(glm::vec3(0.0f, 0.0f, 1.0f)) }); // default color blue
		pos = glm::vec3(0.0f, 0.0f, 0.0f); // default position
	}

	Triangle(Point p1, Point p2, Point p3, Color c1, Color c2, Color c3) {
		vertices.push_back(Vertex{ p1, c1 });
		vertices.push_back(Vertex{ p2, c2 });
		vertices.push_back(Vertex{ p3, c3 });
		pos = glm::vec3(0.0f, 0.0f, 0.0f); // default position
	}

	std::vector<Vertex> vertices;
	Point pos;
};



class Graphics {

public:
	Graphics(const std::string vertex_shader, const std::string fragment_shader);

	~Graphics() {
		glDeleteBuffers(batchSize, &vbo);
		glDeleteBuffers(batchSize, &ebo);
		glDeleteVertexArrays(batchSize, &vao);
	}

	void drawTriangle(const Triangle& triangle);
	void drawTriangle(Point& pos, glm::vec3 scale, float rotation, Color& color);
	void endFrame();
	void beginFrame();
private:
	unsigned int vao; //vertex array object
	unsigned int vbo; //vertex buffer object
	unsigned int ebo; //element buffer object
	std::vector<float> vertices; // vertex data to be sent to the GPU
	std::vector<unsigned int> indices; // indices for the vertex data
	Triangle baseTriangle;
	Camera camera;
	Shader defaultShader;
	glm::mat4 model = glm::mat4(1.0f); // identity matrix for model
	glm::vec4 basicTriangle[3] = { 
		// used to draw triangles, we need w here because we calulate the new vertex here
		// not in the vertex shader, were we simply pass an identity matrix
		//x ,  y,    z, w,
		glm::vec4(0.0f, 0.5f, 0.0f, 1.0f),
		glm::vec4(0.5f, 0.0f, 0.0f, 1.0f),
		glm::vec4(-.5f, 0.0f, 0.0f, 1.0f)
	};
	unsigned int batchSize = 16;

};