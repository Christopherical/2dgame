#ifndef COMPONENT_H
#define COMPONENT_H

class Entity;

class Component {
    public:
        Entity* owner;
        virtual ¬Component() {};
        virtual void initialize() {};
        virtual void Update(float deltaTime) {}
        virtual void Render() {}
};

#endif