#include "Game/Scenes/Maze3DScene.h"

#include "Core/Components/Render/CameraComponent.h"
#include "Core/Objects/EmptyObject.h"
#include "Game/Objects/Maze.h"
#include "Game/Objects/Player.h"

Maze3DScene::Maze3DScene() {

	// EmptyObject* player = new EmptyObject(nullptr, "Camera");
	// CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(
	// 	player->AddComponent(new CameraComponent(player))
	// 	);
	// cameraComponent->orthographic = true;
	// cameraComponent->mainCamera = true;
	// cameraComponent->orthoSize = 1.0f;
	// player->position = glm::vec3(0, 0, 3);
	// AddObject(player);

	Maze* maze = new Maze(nullptr, "Maze");
	AddObject(maze);
	
	Player* player = new Player(nullptr, "Player");
	AddObject(player);
}