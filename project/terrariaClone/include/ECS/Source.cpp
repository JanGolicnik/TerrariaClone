#include <derivedsystems.h>
#include <ECS.h>
#include <core.h>

std::unordered_map<const char*, std::shared_ptr<SystemBase>> ECS::systems;
std::unordered_map<int, Entity> ECS::entities;
std::vector<int> ECS::idsToServe;
std::unordered_map<const char*, const char*> ECS::compToSystem;

int main()
{
	ECS::registerSystem<firstSystem, firstComponent>();
	ECS::registerSystem<secondSystem, secondComponent>();
	ECS::registerSystem<thirdSystem, thirdComponent>();

	auto sys1 = ECS::getSystem<firstSystem>();
	auto sys2 = ECS::getSystem<secondSystem>();

	int player = ECS::newEntity();
	ECS::addComponent<firstComponent>(player, { 13 });

	int boss = ECS::newEntity();
	ECS::addComponent<firstComponent>(boss, { 114 });
	ECS::addComponent<thirdComponent>(boss, { "ooo ooo aa aa" });

	int rabbit = ECS::newEntity();
	ECS::addComponent<firstComponent>(rabbit, { 11115 });

	ECS::deleteEntity(player);

	int entities[10];
	for (int i = 0; i < 10; i++) {
		entities[i] = ECS::newEntity();
		firstComponent a = { 10-i };
		ECS::addComponent<firstComponent>(entities[i], a);
	}

	player = ECS::newEntity();
	ECS::addComponent<firstComponent>(player, { 13 });
	ECS::addComponent<secondComponent>(player, { "aaa" });

	for (int i = 0; i < 10; i++) {
		ECS::deleteEntity(entities[i]);
	}

	while (1) {
		for (int i = 0; i < 10; i++) {
			entities[i] = ECS::newEntity();
			firstComponent a = { 10 - i };
			ECS::addComponent<firstComponent>(entities[i], a);
			ECS::addComponent<secondComponent>(entities[i], { "aaaaaaaaaaaa" });
		}
		int beast = ECS::newEntity();
		ECS::addComponent<firstComponent>(beast, { 123 });
		ECS::addComponent<secondComponent>(beast, { "aaaaaaaaaaaa" });
		ECS::addComponent<thirdComponent>(beast, { "qeqqqqqq"});

		ECS::Update();
		ECS::deleteEntity(beast);
		for (int i = 0; i < 10; i++) {
			ECS::deleteEntity(entities[i]);
		}
	}
}