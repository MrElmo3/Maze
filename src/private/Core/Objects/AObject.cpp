#include "Core/Objects/AObject.h"

#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Core/Global.h"
#include "Core/Scenes/ASceneController.h"
#include "Core/Window.h"

AObject::AObject(AObject* _parent, std::string name) {
	this->parent = _parent;
	this->name = name;
	if (parent == nullptr) return;
	parent->AddChild(this);
}

AObject::~AObject() {
}

void AObject::Awake() {
	for (auto element : components) {
		if (!element->isEnabled) continue;
		element->Awake();
	}
	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->Awake();
	}
}

void AObject::Start() {
	for (auto element : components) {
		if (!element->isEnabled) continue;
		element->Start();
	}
	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->Start();
	}
}

void AObject::FixedUpdate() {
	
	if (parent != nullptr) {
		glm::mat4 trans = glm::mat4(1);
		
		glm::scale(trans, parent->worldScale);
		worldScale = trans * glm::vec4(scale, 1);

		glm::translate(trans, parent->worldPosition);
		worldPosition = trans * glm::vec4(position, 1);
		
		worldRotation = parent->worldRotation * rotationQuat;
	}
	else {
		this->worldScale = scale;
		this->worldPosition = position;
		this->worldRotation = rotationQuat;
	}

	for (auto element : components) {
		if (!element->isEnabled) continue;
		element->FixedUpdate();
	}

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->FixedUpdate();
	}
}


void AObject::Update(double deltaTime) {

	for (auto element : components) {
		if (!element->isEnabled) continue;
		element->Update(deltaTime);
	}

	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->Update(deltaTime);
	}
}

void AObject::LateUpdate() {
	for (auto element : components) {
		if (!element->isEnabled) continue;
		element->LateUpdate();
	}
	for (auto element : children) {
		if (!element->isEnabled) continue;
		element->LateUpdate();
	}
}

void AObject::End() {
	for (auto element : components) {
		delete element;
	}
	for (auto element : children) {
		delete element;
	}
}

IComponent* AObject::AddComponent(IComponent* component) {
	components.push_back(component);
	return component;
}

void AObject::AddChild(AObject* child) {
	children.push_back(child);
}

void AObject::Rotate(glm::quat rotation) {
	rotation = glm::normalize(rotation);
	this->rotationQuat = rotation * this->rotationQuat;
}

void AObject::RotateEuler(glm::vec3 rotation) {
	rotation = glm::radians(rotation);
	Rotate(glm::quat(rotation));
}

glm::vec3 AObject::Forward() {
	glm::vec4 forward = glm::vec4(Global::FORWARD, 1);

	glm::mat4 trans = glm::mat4(1.0f);
	glm::quat rotation = GetWorldRotation();
	glm::vec3 vectorRotation = glm::vec3(rotation.x, rotation.y, rotation.z);
	float angle = -2 * glm::acos(rotation.w);
	if (vectorRotation != glm::vec3(0))
		trans = glm::rotate(trans, angle, vectorRotation);

	forward = trans * forward;
	
	return forward;
}

glm::vec3 AObject::Right() {
	glm::vec4 right = glm::vec4(Global::RIGHT, 1);

	glm::mat4 trans = glm::mat4(1.0f);
	glm::quat rotation = GetWorldRotation();
	glm::vec3 vectorRotation = glm::vec3(rotation.x, rotation.y, rotation.z);
	float angle = -2 * glm::acos(rotation.w);
	if (vectorRotation != glm::vec3(0))
		trans = glm::rotate(trans, angle, vectorRotation);

	right = trans * right;
	
	return right;
}

glm::vec3 AObject::Up() {
	glm::vec4 up = glm::vec4(Global::UP, 1);

	glm::mat4 trans = glm::mat4(1.0f);
	glm::quat rotation = GetWorldRotation();
	glm::vec3 vectorRotation = glm::vec3(rotation.x, rotation.y, rotation.z);
	float angle = -2 * glm::acos(rotation.w);
	if (vectorRotation != glm::vec3(0))
		trans = glm::rotate(trans, angle, vectorRotation);

	up = trans * up;
	
	return up;
}

void AObject::SetRotation(glm::quat rotation) {
	rotation = glm::normalize(rotation);
	this->rotationQuat = rotation;
}

void AObject::SetWorldPosition(glm::vec3 position) {
	if (parent != nullptr) {
		this->position = glm::vec3(
			(position.x - parent->worldPosition.x) / parent->worldScale.x,
			(position.y - parent->worldPosition.y) / parent->worldScale.y,
			(position.z - parent->worldPosition.z) / parent->worldScale.z
		);
	}
	else {
		this->position = position;
	}
	this->worldPosition = position;
}

void AObject::SetWorldRotation(glm::quat rotation) {
	// rotation = glm::normalize(rotation);
	// this->rotationQuat = rotation;
}

void AObject::SetWorldScale(glm::vec3 scale) {
	if (parent != nullptr) {
		this->scale = glm::vec3(
			scale.x / parent->worldScale.x,
			scale.y / parent->worldScale.y,
			scale.z / parent->worldScale.z
		);
	}
	else {
		this->scale = scale;
	}
	this->scale = scale;
}

AObject* AObject::FindObjectByName(std::string name) {
	
	ASceneController* scene = Window::GetInstance().GetActualScene();

	for (AObject* object : scene->objects) {
		if (object->name == name) {
			return object;
		}
	}
	return nullptr;
}


