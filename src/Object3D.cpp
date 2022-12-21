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

    //cPolygon.Set(vertices.arr, vertices.size);
    //cFixtureDef.shape = &cPolygon;
    //cFixtureDef.friction = 0.3f;
    //pPhysicsBody->CreateFixture(&cFixtureDef);

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

    std::vector<std::vector<float>> vertex_data;
    while (std::getline(file, line))
    {
        // Skip ahead to the vertex data
        if (line[0] != 'v')
            continue;

        std::stringstream ss(line);
        std::string token;

        std::vector<float> xy;

        int i = 0;
        bool is_vertex = false;
        while (ss >> token)
        {
            // Set flag to 'true' if vertex found
            if (token == "v")
            {
                is_vertex = true;
                continue;
            }
            else if (!is_vertex)
                break;

            if (i >= 2)
            {
                is_vertex = false;
                break;
            }

            if (is_vertex)
            {
                xy.push_back(std::stof(token));
                i++;
            }
        }

        if (xy.size() > 0)
            vertex_data.push_back(xy);
    }

    file.close();

    // Convert to C-style array
    vertices->size = vertex_data.size();
    b2Vec2 arr[vertices->size];
    for (int i = 0; i < vertices->size; i++)
    {
        arr[i] = b2Vec2(vertex_data[i][0], vertex_data[i][1]);
    }

    vertices->arr = arr;
}
