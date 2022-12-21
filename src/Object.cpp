#include "Object.h"


Object::Object(glm::vec3 pos, b2World* world)
    : vPos(pos), m_parentWorld(world)
{
    SetupMesh();

    m_physicsBodyDef.type = b2_dynamicBody;
    m_physicsBodyDef.position.Set(pos.x, pos.y);
    m_physicsBody = m_parentWorld->CreateBody(&m_physicsBodyDef);
    m_dynamicBox.SetAsBox(1.0f, 1.0f);

    m_fixtureDef.shape = &m_dynamicBox;
    m_fixtureDef.density = 1.0f;
    m_fixtureDef.friction = 0.3f;
    m_physicsBody->CreateFixture(&m_fixtureDef);
}


Object::~Object()
{
}


void Object::SetColor(glm::vec3 color)
{
    m_vColor = color;
}


void Object::SetupMesh()
{
    // Vertex Buffer
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //glGenBuffers(1, &EBO)

    // Vertex Array
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glEnableVertexAttribArray(0);

    // TODO - Index buffer
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER)

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindVertexArray(0);
}


void Object::Draw(Shader &shader)
{
    glm::mat4 model = glm::mat4(1.0f);
    b2Vec2 position = m_physicsBody->GetPosition();
    float angle = m_physicsBody->GetAngle();
    model = glm::rotate(model, glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::translate(model, glm::vec3(position.x, position.y, 0.0f));
    model = glm::scale(model, vScale);

    shader.Use();
    shader.SetMat4("model", model);
    shader.SetVec3("vColor", m_vColor);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3);

    glBindVertexArray(0);
}
