#include <Graphics.hpp>
#include <iostream>
void checkGLError(const char* operation) {
	GLenum error;
	while ((error = glGetError()) != GL_NO_ERROR) {
		std::cout << "OpenGL error after " << operation << ": " << error << std::endl;
	}
}

Graphics::Graphics(const std::string vertex_shader, const std::string fragment_shader) :
	vao(0), 
	vbo(0), 
	ebo(0),
	camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.0f, 0.0f, 0.0f),
	vertices(1000), // reserve space for 1000 vertices
	indices(1000), // reserve space for 1000 indices
	defaultShader(vertex_shader.c_str(), fragment_shader.c_str())
{
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

	// I want a simple 2d, so push camera projection matrix, view matrix to the shader
	// as it will be the same
	defaultShader.use();
	defaultShader.setUniform4fv("projection", camera.getProjectionMatrix());
	defaultShader.setUniform4fv("view", camera.getViewMatrix());
	defaultShader.setUniform4fv("model", glm::value_ptr(model));
	// vertex data are in the format:
	// position (3 floats) + color (3 floats) = 6 floats per vertex
	// each position is separated by 6 floats so stride is 6 * sizeof(float)
	// offset of color is 3*sizeof(float)
	// specifying aPos attribute at index 0
	glEnableVertexAttribArray(0); // enable aPos attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
	// specifying aColor attribute at index 1
	glEnableVertexAttribArray(1); // enable aColor attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
}


void Graphics::drawTriangle(const Triangle& triangle)
{
	glBindVertexArray(vao);
	
	//vertices.push_back(triangle.vertices[0]);
	//vertices.push_back(triangle.vertices[1]);
	//vertices.push_back(triangle.vertices[2]);

	glBindBuffer(GL_ARRAY_BUFFER, vbo); //is it superfluous? we already bound it in the constructor
	// here we send the vertex data to the GPU
	glBufferData(GL_ARRAY_BUFFER, triangle.vertices.size()*sizeof(Vertex), triangle.vertices.data(), GL_STATIC_DRAW);
	// now we need to specify the layout of the vertex data
	// and also send the color data to the GPU
	
	// vertex data are in the format:
	// position (3 floats) + color (3 floats) = 6 floats per vertex
	// each position is separated by 6 floats so stride is 6 * sizeof(float)
	// specifying aPos attribute at index 0
	glEnableVertexAttribArray(0); // enable aPos attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0 * sizeof(float)));
	// specifying aColor attribute at index 1
	glEnableVertexAttribArray(1); // enable aColor attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

	glm::mat4 model = glm::mat4(1.0f); // identity matrix for model
	model = glm::translate(model, triangle.pos.position); // translate the model to the triangle position
	defaultShader.setUniform4fv("model", glm::value_ptr(model));
	
	glDrawArrays(GL_TRIANGLES, 0, 3);
}

void Graphics::drawTriangle(Point& pos, glm::vec3 scale, float rotation, Color& color) {
	// calculate model matrix here,
	// then send all vertex data to the GPU at once in endFrame
	glm::mat4 model = glm::mat4(1.0f); // identity matrix for model
	// order of transformations: scale, rotate, translate, because of matrix multiplication order are from
	// right to left we have to ->translate->rotate->scale
	model = glm::translate(model, pos.position); //move basic triangle to pos
	model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // rotate around x axis
	model = glm::scale(model, glm::vec3(scale.x, scale.y, 1.0f)); // scale only x and y axis
	// compute the transformed vertices and add them to the vertices vector
	for (int i = 0; i < 3; i++) {
		glm::vec4 transformedVertex = model * basicTriangle[i];
		vertices.push_back(transformedVertex.x);
		vertices.push_back(transformedVertex.y);
		vertices.push_back(transformedVertex.z);
		vertices.push_back(color.color.r);
		vertices.push_back(color.color.g);
		vertices.push_back(color.color.b);
	}
}

void Graphics::endFrame()
{
	// at the end of the frame we push data to the gpu and call draw call
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6); // each vertex has 6 floats (3 for position, 3 for color)
}

void Graphics::beginFrame()
{
	// at the beginning of the frame clear the vertex data and indices
	vertices.clear();
	indices.clear();
}

//void Graphics::drawRectangle(const Rectangle& rectangle) 
//{
//
//}