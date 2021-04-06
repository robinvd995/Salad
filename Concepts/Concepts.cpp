#include "Entity/Entity.h"
#include "Entity/EntityComponentTest.h"
#include <memory>

int main()
{
    std::shared_ptr<EntityComponentTest> ect = std::make_shared<EntityComponentTest>();
    Ent e;
    Ent::attachComponent(e, ect);

    auto comp = e.getComponent<EntityComponentTest>();

    e.update();
}
