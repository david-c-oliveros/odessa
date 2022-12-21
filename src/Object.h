#ifndef BOID_H
#define BOID_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <box2d/box2d.h>

#include "Shader.h"



class Object
{
    private:
        glm::vec3 m_vColor;
        glm::vec3 vScale = glm::vec3(1.0f, 1.0f, 1.0f);

    public:
        static constexpr glm::vec3 vertices[3] =
        {
            glm::vec3( -0.5f, -0.5f, 0.0f ),
            glm::vec3(  0.5f, -0.5f, 0.0f ),
            glm::vec3(  0.0f,  0.5f, 0.0f ) 
        };

        /***************************************/
        /***************************************/
        /*                                     */
        /*               Box2D                 */
        /*                                     */
        /***************************************/
        /***************************************/
        b2World* m_parentWorld;

        b2BodyDef      m_physicsBodyDef;
        b2Body*         m_physicsBody;
        b2PolygonShape  m_dynamicBox;
        b2FixtureDef   m_fixtureDef;

        static constexpr float cube[] = {
            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f, -0.5f,  
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 

            -0.5f, -0.5f,  0.5f, 
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f, -0.5f,  0.5f, 

            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f, -0.5f, 
            -0.5f, -0.5f,  0.5f, 
            -0.5f,  0.5f,  0.5f, 

             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  

            -0.5f, -0.5f, -0.5f, 
             0.5f, -0.5f, -0.5f,  
             0.5f, -0.5f,  0.5f,  
             0.5f, -0.5f,  0.5f,  
            -0.5f, -0.5f,  0.5f, 
            -0.5f, -0.5f, -0.5f, 

            -0.5f,  0.5f, -0.5f, 
             0.5f,  0.5f, -0.5f,  
             0.5f,  0.5f,  0.5f,  
             0.5f,  0.5f,  0.5f,  
            -0.5f,  0.5f,  0.5f, 
            -0.5f,  0.5f, -0.5f, 
        };
        glm::vec3 vPos;

        unsigned int VBO, VAO, EBO;

        Object(glm::vec3 pos, b2World* world);
        ~Object();

        void SetColor(glm::vec3 color);

        void SetupMesh();
        void Draw(Shader &shader);
};

#endif
