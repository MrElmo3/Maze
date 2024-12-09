#include "Core/Physics/PhysicsEngine.h"
#include "Core/Components/Physics/RigidbodyComponent.h"
#include "Core/Components/Physics/ColliderComponent.h"
#include "Core/Objects/AObject.h"

PhysicsEngine::PhysicsEngine() {
	rigidbodies = std::vector<RigidbodyComponent*>();
	colliders = std::vector<ColliderComponent*>();
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::Start(std::vector<AObject*> objects) {
	for (AObject* object : objects) {
		for (IComponent* component : object->components) {

			RigidbodyComponent* rigidbody = dynamic_cast<RigidbodyComponent*>(component);
			if (rigidbody != nullptr) {
				AddRigidbody(rigidbody);
				continue;
			}

			ColliderComponent* collider = dynamic_cast<ColliderComponent*>(component);
			if (collider != nullptr) {
				AddCollider(collider);
				continue;
			}
		}
	}
}

void PhysicsEngine::Update(float deltaTime) {
	for (int i = 0; i < rigidbodies.size(); i++) {
		rigidbodies[i]->PhysicsUpdate(deltaTime);
	}
}

void PhysicsEngine::AddRigidbody(RigidbodyComponent* rigidbody) {
	rigidbodies.push_back(rigidbody);
}

void PhysicsEngine::AddCollider(ColliderComponent* collider) {
	colliders.push_back(collider);
}
