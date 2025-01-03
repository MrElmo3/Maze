#pragma once
#include <vector>
#include "Core/Physics/PhysicsEngine3D.h"
#include "Core/Objects/AObject.h"

class ASceneController {

private:
	double current;
	double previous;
	double physicsTimeOffset = 0.0;
	
public:
	std::vector<AObject*> objects = std::vector<AObject*>();
	
	ASceneController();
	virtual ~ASceneController();

	virtual void SceneUpdate();

	virtual void Awake();
	virtual void Start();
	virtual void FixedUpdate();
	virtual void Update(double deltaTime);
	virtual void LateUpdate();
	virtual void End();

	void AddObject(AObject* object);
};