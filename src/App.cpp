#include "App.h"


unsigned int SCR_WIDTH = 2000;
unsigned int SCR_HEIGHT = 2000;

float fDeltaTime = 0.0f;
float fLastFrame = 0.0f;
float fCurrentFrame = 0.0f;

glm::vec3 vCameraPos = glm::vec3(0.0f, 0.0f, -10.0f);
glm::vec3 vFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);

Cam_Dir DIRECTION;

float MOVE_SPEED = 0.2f;

App::App()
{
}


App::~App()
{
}


bool App::ConfigGlfw()
{
    const char* glsl_version = "#version 330";
    /**********************************************************/
    /**********************************************************/
    /*               Initialize GLFW and GLEW                 */
    /**********************************************************/
    /**********************************************************/
    if (!glfwInit())
    {
        std::cout << "Failed to initialize GLFW\n";
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "App", NULL, NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "\nERROR: Failed to initialize GLEW\n";
        return false;
    }

    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);


    /********************************************/
    /********************************************/
    /*               GL Configs                 */
    /********************************************/
    /********************************************/
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    stbi_set_flip_vertically_on_load(true);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    return true;
}


bool App::ConfigShaders()
{
    /*****************************************/
    /*****************************************/
    /*               Shaders                 */
    /*****************************************/
    /*****************************************/
    shader.Create("../../shaders/vLight.shader", "../../shaders/fLight.shader");
    shaderSimple.Create("../../shaders/vSimple.shader", "../../shaders/fSimple.shader");

    SetLights();

    return true;
}


void App::SetLights()
{
    shader.Use();

    glm::vec3 lightPos = glm::vec3(0.0f, 0.0f, -5.0f);
    glm::vec3 lightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
    glm::vec3 lightDiffuse = glm::vec3(0.0f, 0.1f, 0.0f);
    glm::vec3 lightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);

    shader.SetVec3("viewPos", vCameraPos);
    shader.SetFloat("material.shininess", 32.0f);
    shader.SetVec3("dirLight.direction", lightPos);
    shader.SetVec3("dirLight.ambient", lightAmbient);
    shader.SetVec3("dirLight.diffuse", lightDiffuse);
    shader.SetVec3("dirLight.specular", lightSpecular);

    glm::vec3 pointLightPos = glm::vec3(0.0f, 2.0f, -4.0f);
    glm::vec3 pointLightAmbient = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 pointLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
    glm::vec3 pointLightSpecular = glm::vec3(1.0f, 1.0f, 1.0f);
    shader.SetVec3("pointLight.position", pointLightPos);
    shader.SetVec3("pointLight.ambient", pointLightAmbient);
    shader.SetVec3("pointLight.diffuse", pointLightDiffuse);
    shader.SetVec3("pointLight.specular", pointLightSpecular);
    shader.SetFloat("pointLight.constant", 1.0f);
    shader.SetFloat("pointLight.linear", 1.0f);
    shader.SetFloat("pointLight.quadratic", 0.032f);

    glm::vec3 spotLightPos = vCameraPos;
    glm::vec3 spotLightDir = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 spotLightAmbient = glm::vec3(0.0f);
    glm::vec3 spotLightDiffuse = glm::vec3(8.0f);
    glm::vec3 spotLightSpecular = glm::vec3(8.0f);
    shader.SetVec3("spotLight.position", spotLightPos);
    shader.SetVec3("spotLight.ambient", spotLightAmbient);
    shader.SetVec3("spotLight.diffuse", spotLightDiffuse);
    shader.SetVec3("spotLight.specular", spotLightSpecular);
    shader.SetFloat("spotLight.constant", 1.0f);
    shader.SetFloat("spotLight.linear", 1.09f);
    shader.SetFloat("spotLight.constant", 1.0f);
    shader.SetFloat("spotLight.quadratic", 0.032f);
    shader.SetFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
    shader.SetFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));
}


bool App::ConfigBox2D()
{
    groundBodyDef.position.Set(0.0f, -10.0f);
    groundBody = world.CreateBody(&groundBodyDef);
    groundBox.SetAsBox(50.0f, 10.0f);
    groundBody->CreateFixture(&groundBox, 0.0f);

    return true;
}


bool App::Construct()
{
    if (!ConfigGlfw())
        return false;

    if (!ConfigShaders())
        return false;

    if (!ConfigBox2D())
        return false;

    cCubeMesh = std::make_unique<Model>("../../res/models/cube_scifi/obj/Cube.obj");
    //cCubeMesh = std::make_unique<Model>("../../res/models/backpack/backpack.obj");
    cCube.Create(&world, std::move(cCubeMesh), glm::vec3(5.0f, 0.0f, 0.0f), 0.0f);

    pOdessaMeshes = std::make_unique<Model>("../../res/models/odessa/odessa_test.obj");
    cOdessa.Create(&world, std::move(pOdessaMeshes), glm::vec3(0.0f), 0.0f);

    obj = std::make_unique<Object>(glm::vec3(0.0f, 10.0f, 0.0f), &world);
    obj->SetColor(glm::vec3(0.5f, 0.3f, 0.5f));

    return true;
}


void App::Start()
{ 
    // Main Loop
    while (!glfwWindowShouldClose(window))
    {
        fDeltaTime = fCurrentFrame - fLastFrame;
        fLastFrame = fCurrentFrame;

        Update(fDeltaTime);
        Render();
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}


void App::Update(float fDeltaTime)
{
    fCurrentFrame = static_cast<float>(glfwGetTime());

    shader.Use();
    shader.SetVec3("viewPos", vCameraPos);
    shader.SetVec3("spotLight.position", vCameraPos);

    processInput(window);

    /****************************/
    /*        Box2D Step        */
    /****************************/
    world.Step(timeStep, velocityIterations, positionIterations);
    //b2Vec2 position = body->GetPosition();
    //float angle = body->GetAngle();
    b2Vec2 position = obj->m_physicsBody->GetPosition();
    float angle = obj->m_physicsBody->GetAngle();
    //printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

    /***********************************************/
    /*        Set Model / View / Projection        */
    /***********************************************/
    glm::vec3 vLookDir = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 vTargetPos = vCameraPos + vLookDir;
    glm::vec3 vUp = glm::vec3(0.0f, 1.0f, 0.0f);

    /*****************************/
    /*        Perspective        */
    /*****************************/
    glm::mat4 projection = glm::perspective(glm::radians<float>(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(vCameraPos, vTargetPos, vUp);

    shader.Use();
    shader.SetMat4("projection", projection);
    shader.SetMat4("view", view);

    shaderSimple.Use();
    shaderSimple.SetMat4("projection", projection);
    shaderSimple.SetMat4("view", view);
}


void App::Render()
{
    glClearColor(0.05f, 0.05f, 0.25f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    RenderUI();

    obj->Draw(shaderSimple);
    cCube.Draw(shader);
    cOdessa.Draw(shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
}


void App::RenderUI()
{
    // Start the Dear ImGui frame
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (bDebugScreen)
        {
            ImGui::Begin("Debug", &bDebugScreen);

            ImGui::Text("fDeltaTime: %f", fDeltaTime);

            if (ImGui::Button("Close"))
                bDebugScreen = false;
            ImGui::End();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }
}


/************************************/
/************************************/
/*                                  */
/*        Callback Functions        */
/*                                  */
/************************************/
/************************************/


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        vCameraPos.y += MOVE_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        vCameraPos.y -= MOVE_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        vCameraPos.x += MOVE_SPEED;
    }

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        vCameraPos.x -= MOVE_SPEED;
    }
}
