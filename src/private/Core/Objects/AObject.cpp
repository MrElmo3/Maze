#include "Core/Objects/AObject.h"

AObject::AObject() {
}

AObject::~AObject() {
}


void AObject::Start() {
}

void AObject::Update() {
	for (auto element : components) {
		element->Update();
	}
}

void AObject::End() {
}

void AObject::addComponent(IComponent* component) {
	components.push_back(component);
}



