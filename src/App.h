#ifndef BOIDS_H
#define BOIDS_H

#include <iostream>
#include <vector>
#include <memory>

#include <GL/glew.h>
#include<GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

#include <box2d/box2d.h>

#include "Object.h"
#include "Object3D.h"
#include "Model.h"
#include "Mesh.h"
#include "Quad.h"
#include "Shader.h"


/************************************/
/*        Callback Functions        */
/************************************/
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


enum AppState
{
    NORMAL,
    DEBUG
};


enum Cam_Dir
{
    NONE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};


class App {

    private:
        GLFWwindow *window;

        Shader shader;
        Shader shaderSimple;
        std::unique_ptr<Object> obj;

        std::unique_ptr<Model> cCubeMesh;
        std::unique_ptr<Model> pOdessaMeshes;
        Object3D cOdessa;
        Object3D cCube;

        /***************************************/
        /*               ImGui                 */
        /***************************************/
        bool bDebugScreen = true;
        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        /***************************************/
        /***************************************/
        /*                                     */
        /*               Box2D                 */
        /*                                     */
        /***************************************/
        /*********************************************/
        /*               Ground Body                 */
        /*********************************************/
        b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
        b2World world = b2World(gravity);
        b2BodyDef groundBodyDef;
        b2Body* groundBody;
        b2PolygonShape groundBox;

        float timeStep = 1.0f / 60.0f;
        int32 velocityIterations = 6;
        int32 positionIterations = 2;

    public:
        App();
        ~App();

        bool Construct();
        bool ConfigGlfw();
        bool ConfigShaders();
        void SetLights();
        bool ConfigBox2D();
        void Start();
        void Update(float fDeltaTime);
        void Render();
        void RenderUI();
};

#endif
