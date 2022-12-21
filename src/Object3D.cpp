#include "Object3D.h"


Object3D::Object3D()
{
}


Object3D::Object3D(std::unique_ptr<Model> modelMesh, float rotAngle)
    : m_pModelMesh(std::move(modelMesh)), vPos(glm::vec3(0.0f, 0.0f, 0.0f)), fRotAngle(rotAngle)
{
    vNextPos = vPos;
}


Object3D::Object3D(std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle)
    : m_pModelMesh(std::move(modelMesh)), vPos(pos), fRotAngle(rotAngle)
{
    vNextPos = vPos;
}


void Object3D::Create(b2World* world, std::unique_ptr<Model> modelMesh, glm::vec3 pos, float rotAngle)
{
    pParentWorld = world;
    cPhysicsBodyDef.type = b2_dynamicBody;
    cPhysicsBodyDef.position.Set(vPos.x, vPos.y);
    pPhysicsBody = pParentWorld->CreateBody(&cPhysicsBodyDef);

    Box2D_Vec2_Arr vertices;
    LoadBox2DVertices("../../res/extra/odessa_export_box2d_object.obj", &vertices);

    //b2Vec2 arr[4];
    //arr[0].Set(0.0f, 0.0f);
    //arr[1].Set(1.0f, 0.0f);
    //arr[2].Set(1.0f, 1.0f);
    //arr[3].Set(0.0f, 1.0f);

    cPolygon.Set(vertices.arr, vertices.size);
    cFixtureDef.shape = &cPolygon;
    cFixtureDef.friction = 0.3f;
    pPhysicsBody->CreateFixture(&cFixtureDef);

    m_pModelMesh = std::move(modelMesh);
    vPos = pos;
    vVel = glm::vec3(0.0f);
    fRotAngle = rotAngle;
}


void Object3D::SetColor(glm::vec3 color)
{
    m_vColor = color;
}


void Object3D::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(fRotAngle), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::translate(model, vPos);

    shader.Use();
    shader.SetMat4("model", model);

    m_pModelMesh->Draw(shader);
}


void Object3D::LoadBox2DVertices(const char* path, Box2D_Vec2_Arr* vertices)
{
    // TODO - Read in OBJ file
    std::ifstream file;

    file.open(path);

    std::stringstream ss;
    std::string line;

    while (std::getline(file, line))
    {
        std::cout << line << "NEWLINE" << std::endl;
    }

    file.close();

    //b2Vec2 arr[4];
    b2Vec2 v0(0.0f, 0.0f);
    b2Vec2 v1(0.0f, 1.0f);
    b2Vec2 v2(1.0f, 1.0f);
    b2Vec2 v3(1.0f, 0.0f);

    b2Vec2 arr[4] = { v0, v1, v2, v3 };

    vertices->arr = arr;
    vertices->size = 4;
//    std::cout << "Loading " << path << std::endl;
}
