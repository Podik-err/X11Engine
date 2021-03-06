#include "Window.h"
#include <thread>
#include "Graphics/Graphics.h"
#include "ComponentManager.h"
#include "EntityManager.h"
#include "SystemManager.h"
#include "Systems/RenderSystem.h"
#include "Systems/MovementSystem.h"
#include "Systems/LookSystem.h"
#include "Components/RenderComponent.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Entities/Cube.h"
#include "TaskManager/TaskManager.h"
#include "Loader/Loader.h"
#include "Event/EventManager.h"
#include "Controls/Keyboard.h"
#include "Controls/Mouse.h"

#include "Logger/Logger.h"

using std::thread;

void Update() {
	while (Window::get().IsRunning()) {
		EventManager::get()->DispatchEvents();
		ECS::SystemManager::Get()->PreUpdate();
		ECS::SystemManager::Get()->Update();
		ECS::SystemManager::Get()->PostUpdate();
	}
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	Window::get();
	Model* m = Loader::get().LoadModelFromFile("\\aseets\\Minecraft_Axolotl.fbx");

	EntityId a = ECS::EntityManager::Get()->CreateEntity<Cube>();
	EntityId light = ECS::EntityManager::Get()->CreateEntity<Cube>();
	EntityId camera = ECS::EntityManager::Get()->CreateEntity<Cube>();
	ECS::ComponentManager::Get()->AddComponent<TransformComponent>(a, vector3( 5.f, 0.0f, 0.f ), vector3(0.f, 0.f, 0.f));
	ECS::ComponentManager::Get()->AddComponent<TransformComponent>(light, vector3( 0.f, 0.f, 0.f ));
	TransformComponent* cameraPos = ECS::ComponentManager::Get()->AddComponent<TransformComponent>(camera, vector3( 0.0f, 0.0f, 0.0f ));
	ECS::ComponentManager::Get()->AddComponent<CameraComponent>(camera, vector3(0.f, 0.f, 1.f));

	ECS::ComponentManager::Get()->AddComponent<RenderComponent>(a, m);

	ECS::SystemManager::Get()->AddSystem<MovementSystem>(cameraPos);
	ECS::SystemManager::Get()->AddSystem<LookSystem>(cameraPos);
	ECS::SystemManager::Get()->AddSystem<RenderSystem>();

	EventDelegate down = {&Keyboard::OnKeyDown, Keyboard::get()};
	EventDelegate up = {&Keyboard::OnKeyUp, Keyboard::get()};
	EventManager::get()->AddEventCallback(EventType::KeyDown, &down);
	EventManager::get()->AddEventCallback(EventType::KeyUp, &up);
	EventDelegate mouse = { &Mouse::OnMove, Mouse::get() };
	EventManager::get()->AddEventCallback(EventType::MouseMove, &mouse);

	Graphics::get().SetDirLight({ { 0.f, 0.f, 1.f }, { .3f, .0f, .0f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f } });
	Graphics::get().SetPointLight({ 1.f, .09f, 0.032f, { .0f, .0f, .3f }, { 1.f, 1.f, 1.f }, { 1.f, 1.f, 1.f } }, {0.f, 0.f, 0.f});

	thread th2(Update);
	Window::get().Run();
	th2.join();
	return 0;
}
