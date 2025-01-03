#pragma once
#include <map>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include "Util/Singleton.h"

class CameraComponent;
class Shader;
class ColorMaterial;

class Render : public Singleton<Render> {
private:

	CameraComponent* currentCamera;
	
	std::vector<Shader*> shaders;

	std::map<std::string, unsigned int> textures;

	unsigned int VAO_cube;
	unsigned int VBO_cube;
	unsigned int EBO_cube;
	
	unsigned int VAO_quad;
	unsigned int VBO_quad;
	unsigned int EBO_quad;
	
	unsigned int VAO_line;
	unsigned int VBO_line;

public:
	ColorMaterial* gizmosMaterial;

public:
	Render();
	~Render();

private:
	void InitQuad();
	void InitLine();
	void InitCube();

	glm::vec3 TransformWorldToScreen(glm::vec3 worldScale);

public:
	Shader* CreateShader(const std::string vertexPath, const std::string fragmentPath);
	unsigned int GenerateTexture(const std::string texturePath);
	void DrawQuad(glm::mat4 model, Shader* shader, glm::vec3 color = glm::vec3(1.0f));
	void DrawLineSegment(glm::vec3 start, glm::vec3 end, glm::vec3 color);
	void DrawQuadLine(glm::vec3 center, glm::vec3 size, glm::vec3 color);
	void DrawCube(glm::mat4 model, Shader* shader, glm::vec3 color);
	
	void SetCurrentCamera(CameraComponent* camera);
	CameraComponent* GetCurrentCamera() const { return currentCamera; }
};

template class Singleton<Render>;