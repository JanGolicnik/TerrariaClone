#include <ECS/systems.h>

void SystemBase::clean()
{

}

void SystemBase::deleteComponent(int entity)
{
    componentArray.get()->EntityDestroyed(entity);
}

void SystemBase::print()
{
    componentArray.get()->print();
}

void SystemBase::empty()
{
    componentArray.get()->empty();
}

void SystemBase::commitQueued() {
    componentArray.get()->commitQueued();
}