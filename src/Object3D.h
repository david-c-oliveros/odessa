#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>

#include <box2d/box2d.h>

#include "Model.h"


struct Box2D_Vec2_Arr
{
    int size;
    b2Vec2* arr;
};


class Object3D
{
    private:
        std::unique_ptr<Model> m_pModelMesh;
        glm::vec3 m_vColor;

    public:
        glm::vec3 vPos;
        glm::vec3 vNextPos;
        glm::vec3 vVel;
        float fRotAngle;

        /***************************************/
        /*                Box2D                */
        /***************************************/
        b2World*        pParentWorld;
        
        b2BodyDef       cPhysicsBodyDef;
        b2Body*         pPhysicsBody;
        b2PolygonShape  cPolygon;
        b2FixtureDef    cFixtureDef;

    public:
        Object3D();
        Object3D(std::unique_ptr<Model> modelMesh, float rotAngle);
        Object3D(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle);

        void Create(b2World* world, std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle);
        void SetColor(glm::vec3 color);
        void Draw(Shader &shader);

        void LoadBox2DVertices(const char* path, Box2D_Vec2_Arr* vertices);
};

#endif
