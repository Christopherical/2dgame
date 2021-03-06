#ifndef ENTITYMANAGER_H
#define ENTITYMANAGER_H

#include <vector>

#include "./Component.h"
#include "./Constants.h"
#include "./Entity.h"

class EntityManager {
   private:
    std::vector<Entity*> entities;

   public:
    void ClearData();
    void Update(float deltaTime);
    void Render();
    bool HasNoEntities();
    unsigned int GetEntityCount();
    std::vector<Entity*> GetEntities() const;
    std::vector<Entity*> GetEntitiesByLayer(LayerType layer) const;
    Entity& AddEntity(std::string entityName, LayerType layer);
    CollisionType CheckEntityCollisions() const;
};

#endif