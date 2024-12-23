#include "Core/Physics/PhysicsEngine.h"

#include <iostream>

#include "Core/Global.h"
#include "Core/Components/Physics/RigidbodyComponent.h"
#include "Core/Components/Physics/ColliderComponent.h"
#include "Core/Components/Physics/SquareColliderComponent.h"
#include "Core/Objects/AObject.h"
#include "Core/Render/Color.h"

PhysicsEngine::PhysicsEngine() {
	rigidbodies = std::vector<RigidbodyComponent*>();
	colliders = std::vector<ColliderComponent*>();
}

PhysicsEngine::~PhysicsEngine() {
}

void PhysicsEngine::Start(std::vector<AObject*> objects) {
	for (AObject* object : objects) {
		AddObjectRigidbodyAndCollider(object);
	}
}

void PhysicsEngine::Update() {
	for (int i = 0; i < rigidbodies.size(); i++) {
		std::vector<ColliderComponent*> nearColliders = GetNearColliders(rigidbodies[i]);
		rigidbodies[i]->PhysicsUpdate(Global::FIXED_DELTA_TIME, nearColliders);
	}
}

void PhysicsEngine::AddObjectRigidbodyAndCollider(AObject* object) {
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

	for (AObject* child : object->children) {
		AddObjectRigidbodyAndCollider(child);
	}
}

void PhysicsEngine::AddRigidbody(RigidbodyComponent* rigidbody) {
	rigidbodies.push_back(rigidbody);
}

void PhysicsEngine::AddCollider(ColliderComponent* collider) {
	colliders.push_back(collider);
}

std::vector<ColliderComponent*> PhysicsEngine::GetNearColliders(RigidbodyComponent* rigidbody) {
	std::vector<ColliderComponent*> nearColliders = std::vector<ColliderComponent*>();

	glm::vec3 maxDistance = rigidbody->velocity != glm::vec3(0) ?
		rigidbody->velocity : rigidbody->parent->GetGlobalScale();
	
	for (ColliderComponent* collider : colliders) {
		if (rigidbody->parent == collider->parent) continue;

		glm::vec3 distance = rigidbody->parent->GetGlobalPosition() - collider->parent->GetGlobalPosition();
		
		if (glm::length(distance) <= glm::length(maxDistance)) {
			nearColliders.push_back(collider);
		}
	}

	return nearColliders;
}

void PhysicsEngine::CalcMinAndMax(glm::vec2& min, glm::vec2& max, glm::vec2 center, glm::vec2 halfSize) {
	min = center - halfSize;
	max = center + halfSize;
}

bool PhysicsEngine::PointIntersectsSquareCollider(glm::vec2 point, SquareColliderComponent* collider) {
	glm::vec2 min, max;
	CalcMinAndMax(min, max, collider->GetWorldCenter(), collider->GetWorldHalfSize());

	return (
		point.x >= min.x &&
		point.x <= max.x &&
		point.y >= min.y &&
		point.y <= max.y
	);
}

bool PhysicsEngine::SquareColliderIntesectsSquareCollider(
	SquareColliderComponent* collider1,
	SquareColliderComponent* collider2
) {
	glm::vec2 resultPosition = collider1->GetWorldCenter() - collider2->GetWorldCenter();
	glm::vec2 resultHalfSize = collider1->GetWorldHalfSize() + collider2->GetWorldHalfSize();

	glm::vec2 min, max;
	CalcMinAndMax(min, max, resultPosition, resultHalfSize);

	return (
		min.x <= 0 &&
		max.x >= 0 &&
		min.y <= 0 &&
		max.y >= 0
	);
}

glm::vec2 PhysicsEngine::SquareColliderPenetration(
	SquareColliderComponent* collider1,
	SquareColliderComponent* collider2
) {
	glm::vec2 resultPosition = collider1->GetWorldCenter() - collider2->GetWorldCenter();
	glm::vec2 resultHalfSize = collider1->GetWorldHalfSize() + collider2->GetWorldHalfSize();

	glm::vec2 min, max;
	CalcMinAndMax(min, max, resultPosition, resultHalfSize);

	glm::vec2 penetration = glm::vec2(min.x, 0);
	
	float minDistance = std::abs(min.x);

	if (std::abs(max.x) < minDistance) {
		minDistance = std::abs(max.x);
		penetration.x = max.x;
	}

	if (std::abs(min.y) < minDistance) {
		minDistance = std::abs(min.y);
		penetration.x = 0;
		penetration.y = min.y;
	}
	
	if (std::abs(max.y) < minDistance) {
		penetration.x = 0;
		penetration.y = max.y;
	}
	
	return penetration;
}

PhysicsEngine::Hit PhysicsEngine::RaycastSquareCollider(
	glm::vec2 position,
	glm::vec2 direction,
	double distance,
	glm::vec2 colliderPosition,
	glm::vec2 colliderHalfSize) {
	
	Hit hit;
	
	direction = glm::normalize(direction);
	
	glm::vec2 magnitude = direction;
	magnitude *= distance;
	
	glm::vec2 min, max;
	CalcMinAndMax(min, max, colliderPosition, colliderHalfSize);

	float lastEntry = -INFINITY;
	float firstExit = INFINITY;
	
	for (int i = 0; i < 2; i++) {
		if (std::abs(magnitude[i]) != 0) {
			float t1 = (min[i] - position[i]) / magnitude[i];
			float t2 = (max[i] - position[i]) / magnitude[i];

			lastEntry = std::max(lastEntry, std::min(t1, t2));
			firstExit = std::min(firstExit, std::max(t1, t2));
		}
		else if (position[i] <= min[i] || position[i] >= max[i]) {
			return hit;
		}
	}

	if (firstExit > lastEntry && firstExit > 0 && lastEntry < 1) {
		lastEntry = std::max(lastEntry, 0.0f);
		hit.hit = true;
		hit.time = lastEntry;
		hit.position = position + magnitude * lastEntry;
	}

	return hit;
}


