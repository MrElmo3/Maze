#pragma once
#include <vector>
#include <string>

#include "Core/Components/IComponent.h"
#include "Math/Vector3.h"

class AObject {
public:
	std::string name;

	Vector3 position = {0, 0, 0}; //this is the position from the center
	Vector3 rotation = {0, 0, 0}; //this will change in the future
	std::vector<IComponent*> components;

	AObject* parent = nullptr;
	std::vector<AObject*> children;
	
	AObject(AObject* _parent, std::string name);
	virtual ~AObject();
	virtual void Start();
	virtual void Update(float deltaTime);
	virtual void LateUpdate();
	virtual void End();

	IComponent* AddComponent(IComponent* component);

	template<typename T> 
	typename std::enable_if<std::is_base_of<IComponent, T>::value, T*>::type
	GetComponent() {
		for (IComponent* component : components) {
			if (T* t = dynamic_cast<T*>(component)) {
				return t;
			}
		}
		return nullptr;
	}

	void AddChild(AObject* child);

	// Static functions

	static AObject* FindObjectByName(std::string name);
};
