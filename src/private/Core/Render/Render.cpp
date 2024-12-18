#include "Core/Render/Render.h"
#include "Core/Render/Shader.h"
#include "Core/Materials/GizmosMaterial.h"
#include "Core/Global.h"
#include <glad/glad.h>
#include <iostream>

Render::Render() {

	gizmosMaterial = new GizmosMaterial();

	InitQuad();
	InitLine();
}

Render::~Render() {

}

Vector3 Render::TransformWorldToScreen(Vector3 worldScale) {
	return Vector3(
		(Global::METER / Global::SCREEN_FORMAT) * worldScale.x,
		Global::METER * worldScale.y, 0);
}

void Render::InitLine() {
	glGenVertexArrays(1, &VAO_line);

	glBindVertexArray(VAO_line);

	glGenBuffers(1, &VBO_line);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

void Render::InitQuad() {

	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenVertexArrays(1, &VAO_quad);

	glBindVertexArray(VAO_quad);

	glGenBuffers(1, &VBO_quad);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &EBO_quad);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_quad);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

Shader* Render::CreateShader(const std::string vertexPath, const std::string fragmentPath) {

	for (Shader* shader : shaders) {
		if (shader->CompareFiles(vertexPath, fragmentPath)) {
			return shader;
		}
	}

	Shader* newShader = new Shader(vertexPath, fragmentPath);
	shaders.push_back(newShader);
	return newShader;
}

void Render::DrawLineSegment(Vector3 start, Vector3 end, Vector3 color) {

	start = TransformWorldToScreen(start);
	end = TransformWorldToScreen(end);

	gizmosMaterial->SetColor(color);

	gizmosMaterial->shader->Use();
	gizmosMaterial->SetColorUniform();

	float vertices[] = {
		(float)start.x, (float)start.y, (float)start.z,
		(float)end.x, (float)end.y, (float)end.z
	};

	glLineWidth(1);

	glBindVertexArray(VAO_line);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_line);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 6 * sizeof(float), vertices);

	glDrawArrays(GL_LINES, 0, 2);

	glBindVertexArray(0);
}

void Render::DrawQuad(Vector3 center, Vector3 scale) {

	Vector3 scaleScreen = TransformWorldToScreen(scale);
	Vector3 positionScreen = TransformWorldToScreen(center);

	float vertices[] = {
		//bottom left
		(float)(positionScreen.x - scaleScreen.x / 2),
		(float)(positionScreen.y - scaleScreen.y / 2),
		0,

		//bottom right
		(float)(positionScreen.x + scaleScreen.x / 2),
		(float)(positionScreen.y - scaleScreen.y / 2),
		0,

		//top right
		(float)(positionScreen.x + scaleScreen.x / 2),
		(float)(positionScreen.y + scaleScreen.y / 2),
		0,

		//top left
		(float)(positionScreen.x - scaleScreen.x / 2),
		(float)(positionScreen.y + scaleScreen.y / 2),
		0,
	};

	glBindVertexArray(VAO_quad);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_quad);
	glBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(float), vertices);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}

void Render::DrawQuadLine(Vector3 center, Vector3 scale, Vector3 color) {

	Vector3 points[] = {
		Vector3(center.x - scale.x / 2, center.y - scale.y / 2, 0), //bottom left
		Vector3(center.x + scale.x / 2, center.y - scale.y / 2, 0), //bottom right
		Vector3(center.x + scale.x / 2, center.y + scale.y / 2, 0), //top right
		Vector3(center.x - scale.x / 2, center.y + scale.y / 2, 0), //top left
	};

	DrawLineSegment(points[0], points[1], color);
	DrawLineSegment(points[1], points[2], color);
	DrawLineSegment(points[2], points[3], color);
	DrawLineSegment(points[3], points[0], color);

}