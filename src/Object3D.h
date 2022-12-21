#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"


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

    public:
        Object3D();
        Object3D(std::unique_ptr<Model> modelMesh, float rotAngle);
        Object3D(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle);

        void Create(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle);
        void SetColor(glm::vec3 color);
        void Draw(Shader &shader);
};

#endif
