#pragma once
#include <vector>
#include "vector3.h"
#include "Component.h"

using std::vector;

struct MeshComponent : public ECS::Component<MeshComponent> {
	MeshComponent(EntityId owner, vector<vector3> vertices = {}, vector<uint32_t> indices = {}) : Component(owner), vertices(vertices), indices(indices) {}
	vector<vector3> vertices;
	vector<uint32_t> indices;
};