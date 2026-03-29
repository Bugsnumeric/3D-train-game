#include "lab_m1/Tema2/tema2.h"
#include "lab_m1/Tema2/object3D.h"
#include <vector>
#include <iostream>

using namespace std;
using namespace m1;


Tema2::Tema2() {}
Tema2::~Tema2() {}

void Tema2::Init() {
    renderCameraTarget = false;
    perspective = true;
    FoV = 60.0f;
    orthoWidth = 5.0f;
    orthoHeight = 5.0f;
    camera = new implemented::Camera();
    camera->Set(glm::vec3(0, 2, 3.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    srand(time(NULL));

    {
        Shader* shader = new Shader("Tema2Shader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    {
		Mesh* circle = object3D::CreateCircle("circle", glm::vec3(1, 0, 0), 1.0f, glm::vec3(0, 0, 1), false, true);
        meshes["circle"] = circle;
    }

    {
        Mesh* mesh = new Mesh("plain");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("water");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mountain = object3D::CreateMountain("mountain", glm::vec3(0.4f, 0.3f, 0.2f));
        meshes["mountain"] = mountain;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);

    {
        Mesh* tunnel = object3D::CreateCylinder("motor", glm::vec3(0.5f, 0.5f, 0.5f), 1.0f, 5.0f, 32);
        meshes["motor"] = tunnel;
    }

    {
        Mesh* sign = object3D::CreateTriangle("sign", glm::vec3(0, 3, 0), 2.0f, glm::vec3(1, 0, 0), true);
		meshes["sign"] = sign;
    }

    textRenderer = new gfxc::TextRenderer(window->props.selfDir, window->GetResolution().x, window->GetResolution().y);
    textRenderer->Load("assets/fonts/Hack-Bold.ttf", 20);
}

void Tema2::FrameStart() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glm::ivec2 resolution = window->GetResolution();
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema2::Update(float deltaTimeSeconds) {
	//State game == GAME
    textRenderer->RenderText("minutes playing: " + std::to_string(int(glfwGetTime()) / 60) + ":" + std::to_string(int(glfwGetTime()) % 60), 5, 5, 1, glm::vec3(1, 1, 1));
    if (gameState == GAME) {
        textRenderer->RenderText("coins collected: " + std::to_string(coins), 5, 25, 1, glm::vec3(1, 1, 1));

	    //muntele, apa
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -10));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(70, 2.2f, -30));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.55f, 0.13f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 25));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(70, 2.2f, 30));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.55f, 0.13f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -40));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(70, 2.2f, 30));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.55f, 0.13f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, -15));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 10, 10));
            RenderMesh(meshes["mountain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.36f, 0.25f, 0.20f), false, true);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 1.1f, -15));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 10, 20));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.36f, 0.25f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(23, 1.1f, -15));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f, 10, 20));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.36f, 0.25f, 0.20f), false, false);

            for (int x = 0; x < riverLength; x++) {
                for (int z = 0; z < riverWidth; z++) {
                    float height = 0.5f + 0.1f * sin(deltaTimeSeconds + x);
                    glm::vec3 pos = glm::vec3(x - 34.5f, height / 2, z + 5.5f);
                    glm::vec3 scale = glm::vec3(1, height, 1);
                    RenderMesh(meshes["water"], shaders["Tema2Shader"],
                        glm::translate(glm::mat4(1), pos) * glm::scale(glm::mat4(1), scale),
                        glm::vec3(0, 0.4, 0.8), true, false);
                }
            }
        }

        //rails + deocatiuni
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 7.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.35f, 0.35f, 0.67f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, 7.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.35f, 0.35f, 0.67f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, 7.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.35f, 0.35f, 0.67f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, 0.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-27, 1.1f, 0.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-29, 2.5f, 7.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.41f, 0.13f, 0.55f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-31, 2.5f, 7.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.41f, 0.13f, 0.55f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, 0.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 1.1f, 0.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(21, 2.5f, 7.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.41f, 0.13f, 0.55f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 2.5f, 7.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.41f, 0.13f, 0.55f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 0.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 0.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(1, 2.5f, 7.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.41f, 0.13f, 0.55f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-1, 2.5f, 7.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.41f, 0.13f, 0.55f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 14.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 14.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, 14.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-27, 1.1f, 14.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, 14.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 1.1f, 14.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, -14.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 23));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.55f, 0.13f, 0.13f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(21, 1.2f, -14.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.1f, 23));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 1.2f, -14.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.1f, 23));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.1f, 0.1f, 0.1f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, -47.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(18, 1.1f, -50));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, -47.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, -50));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, -47.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-28, 1.1f, -50));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 36));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 6));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 38));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, 36));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 6));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(18, 1.1f, 38));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, 36));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 6));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-28, 1.1f, 38));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.85f, 0.85f, 0.95f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, 25.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 15));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, 25.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 15));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, 25.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 15));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.1f, -35));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 18));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.1f, -23.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 41));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-30, 1.1f, -23.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 41));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(9, 1.1f, -50));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 13));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(9, 1.1f, 38));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 13));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(8.5, 1.1f, 14.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 12));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(8.5, 1.1f, 0.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 12));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-13.5, 1.1f, 0.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 22));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-13.5, 1.1f, 14.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 22));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-14, 1.1f, 38));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 23));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-14, 1.1f, -50));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 23));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.0f, 0.95f, 0.498f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -8));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -16));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -12));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 3));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -8));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-45.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -14));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-45.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 6));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 2, -12));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-95.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 2, -20));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-95.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 7));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(10, 1.1f, -24));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-40.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.1f, 5));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.3f, 0.18f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(14, 1.2f, -5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.6f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(11, 1.2f, -5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.6f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(12.5f, 1.1f, -5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(20.0f), glm::vec3(0, 1, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.6f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(15.5f, 1.1f, -5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-45.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.6f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(23.5f, 1.1f, -5));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(45.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.6f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 1.2f, -5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.6f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 6.5f, -9));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f, 0.1f, 0.05f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.45f, 0.18f), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(22, 6.5f, -9));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f, 0.1f, 0.05f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.45f, 0.18f), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 6.5f, -9));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f, 0.1f, 0.05f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.45f, 0.18f), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20.5f, 6.5f, -9.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f, 0.1f, 0.05f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.45f, 0.18f), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20.4f, 7.5f, -9.2f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f, 0.1f, 0.05f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.45f, 0.18f), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(16.5f, 6.5f, -9.7f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3.5f, 0.1f, 0.05f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.13f, 0.45f, 0.18f), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 1.1f, -26));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.74f, 0.84f, 0.84f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(23, 1.1f, -26));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.74f, 0.84f, 0.84f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 2.1f, -26));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.74f, 0.84f, 0.84f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 11.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(23, 11.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(21, 11.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(22, 9.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(22, 13.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(21.5, 12.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 13.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 12.1f, -20));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(16, 13.5f, -19));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 12.8f, -19));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 20, -15));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(19, 16.8f, -17));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(99.0f), glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(21, 15.8f, -18));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(99.0f), glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 1, 2));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);
        }

        UpdateTrain(deltaTimeSeconds);

	    //camera urmareste trenul
        {
            glm::vec3 dir = glm::normalize(train.rail->end - train.rail->start);
            float backOffset = 2.0f;
            float upOffset = 4.5f;

            glm::vec3 eye = train.pos + glm::vec3(0, upOffset, 0) - dir * backOffset;
            glm::vec3 lookAt = train.pos + dir * 10.0f;
            camera->Set(eye, lookAt, glm::vec3(0, 1, 0));
        }

        //trenul
        {
            glm::vec3 tr = glm::normalize(train.rail->end - train.rail->start);
            //motorul
            {
                glm::mat4 model = glm::mat4(1);
                glm::vec3 cylinderAxis(0, 1, 0);
                glm::vec3 rotationAxis = glm::cross(cylinderAxis, tr);
                float angle = acos(glm::dot(cylinderAxis, tr));

                model = glm::translate(model, train.pos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                model = glm::scale(model, glm::vec3(1, 1, 1));
                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0.85f, 0.53f, 0.13f), false, false);
            }

            //placajul
            {
                glm::vec3 objectAxis(0, 0, 1);
                glm::vec3 rotationAxis = glm::cross(objectAxis, tr);
                float angle = acos(glm::clamp(glm::dot(objectAxis, tr), -1.0f, 1.0f));
                glm::vec3 basePos = train.pos;
                float newY = train.pos.y - 1.0f;
                glm::vec3 objPos = glm::vec3(train.pos.x, newY, train.pos.z);
                glm::mat4 model = glm::mat4(1);
                model = glm::translate(model, objPos);
                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));
                model = glm::scale(model, glm::vec3(2, 0.1f, 14));
                RenderMesh(meshes["plain"], shaders["Tema2Shader"], model, glm::vec3(0.1f, 0.1f, 0.1f), false, false);

                //vagonul
                wagon.pos = train.pos - tr * 5.0f;
                model = glm::mat4(1);
                model = glm::translate(model, wagon.pos);
                model = glm::scale(model, glm::vec3(2, 2, 2));
                RenderMesh(meshes["plain"], shaders["Tema2Shader"], model, glm::vec3(1, 0, 0), false, false);

                wagon.pos = train.pos - tr * 0.1f;
                model = glm::mat4(1);
                model = glm::translate(model, glm::vec3(wagon.pos.x, wagon.pos.y + 1, wagon.pos.z));
                model = glm::scale(model, glm::vec3(2, 4, 2));
                RenderMesh(meshes["plain"], shaders["Tema2Shader"], model, glm::vec3(0.4f, 0.15f, 0.28f), false, false);
            }

            //rotile
            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos(train.pos.x, wheelsY, train.pos.z);
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * 1.0f;
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * 2.0f;
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * 3.0f;
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * (-1.0f);
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * 4.0f;
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }
            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * 6.0f;
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * 5.0f;
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * (-4.0f);
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }

            {
                glm::vec3 cylinderWheels(0, 1, 0);
                glm::vec3 wheelAxis = glm::normalize(glm::cross(tr, cylinderWheels));

                glm::vec3 rotationAxis = glm::cross(cylinderWheels, wheelAxis);
                float angle = acos(glm::clamp(glm::dot(cylinderWheels, wheelAxis), -1.0f, 1.0f));

                glm::mat4 model = glm::mat4(1);

                float wheelsY = train.pos.y - 1.4f;
                glm::vec3 wheelsPos = train.pos + tr * (-6.5f);
                wheelsPos.y = wheelsY;
                model = glm::translate(model, wheelsPos);

                if (glm::length(rotationAxis) > 0.0001f)
                    model = glm::rotate(model, angle, glm::normalize(rotationAxis));

                glm::vec3 side = glm::normalize(glm::cross(wheelAxis, tr));
                model = glm::translate(model, -side * 1.0f);
                model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.5f));

                RenderMesh(meshes["motor"], shaders["Tema2Shader"], model, glm::vec3(0, 0, 1), false, false);
            }
        }

        if (showTask) {
            textRenderer->RenderText("ice: " + std::to_string(ice) + "/" + std::to_string(randIce), 1100, 10, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("wood: " + std::to_string(wood) + "/" + std::to_string(randWood), 1100, 30, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("water: " + std::to_string(water) + "/" + std::to_string(randWater), 1100, 50, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("time left: " + std::to_string(remainingTime), 1100, 70, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("workers: " + std::to_string(workEfficiency), 1100, 90, 1, glm::vec3(1, 1, 1));

            float t = glm::clamp(remainingTime / 60.0f, 0.0f, 1.0f);
            glm::vec3 colorSign;
            if (t > 0.5f) {
                float k = (t - 0.5f) * 2.0f;
                float r = 1.0f - k;
                float g = 1.0f;
                colorSign = glm::vec3(r, g, 0);
            }
            else {
                float k = t * 2.0f;
                float r = 1.0f;
                float g = k;
                colorSign = glm::vec3(r, g, 0);
            }

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(2, -1, -25));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            RenderMesh(meshes["sign"], shaders["Tema2Shader"], modelMatrix, colorSign, false, false);
            remainingTime -= deltaTimeSeconds;
        }

        //gara centrala
        {
            if (train.pos.x == 0 && train.pos.z > -30 && train.pos.z < -10) {
                textRenderer->RenderText("PRESS [F]", 600, 20, 1, glm::vec3(1, 1, 1));
                workEfficiency = workMaxEfficiency;
            }

		    glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 1, -28));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
		    RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.72f, 0.42f, 0.22f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 1, -22));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.72f, 0.42f, 0.22f), false, false);

		    modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-10, 1, -25));
		    modelMatrix = glm::scale(modelMatrix, glm::vec3(10, 10, 10));
		    RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 7, -22));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 4, 4));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.72f, 0.42f, 0.8f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-3, 7, -28));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 4, 4));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.72f, 0.42f, 0.8f), false, false);

		    modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-10, 6, -25));
		    modelMatrix = glm::scale(modelMatrix, glm::vec3(5, 5, 5));
		    RenderMesh(meshes["mountain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f), false, false);

		    modelMatrix = glm::mat4(1);
		    modelMatrix = glm::translate(modelMatrix, glm::vec3(-5, 2, -25));
		    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
		    modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5f, 3, 0.1f));
		    RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.5f, 0.5f), false, false);
        }

        //depozit gheata
        {
            if (train.pos.x == 20 && train.pos.z > -45 && train.pos.z < -30) {
                textRenderer->RenderText("PRESS [1]", 600, 20, 1, glm::vec3(1, 1, 1));
            }

            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 0, -40));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.8f, 1.0f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 1.5f, -37.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.8f, 1.0f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 2.5f, -42.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.8f, 1.0f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 2.5f, -34));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 4, 3));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.8f, 1.0f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 1, -34));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));
            RenderMesh(meshes["mountain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.7f, 0.8f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 1, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1.5f, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0.25f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(23, 1, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1.5f, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0.25f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(26, 9, -28.5f));
		    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2.4f, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.1f, 0.3f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 7, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.1f, 0.3f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 7, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.1f, 0.3f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(15, 7, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.3f, 0.3f));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.1f, 0.3f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(25, 6.8f, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 0.5f, 0), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(15, 6.8f, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 0.5f, 0), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 6.8f, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.8f, 0.8f));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 0.5f, 0), true, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(23, 1.7f, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f, 1.5f, 2.5f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.7f, 0.3f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(17, 1.7f, -28.5f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2.5f, 1.5f, 2.5f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.7f, 0.3f, 0.20f), false, false);
	    }

        //depozit lemn
        {
            if (train.pos.x > 5 && train.pos.x < 20 && train.pos.z == 38) {
                textRenderer->RenderText("PRESS [2]", 600, 20, 1, glm::vec3(1, 1, 1));
            }

		    glm::mat4 modelMatrix = glm::mat4(1);
		    modelMatrix = glm::translate(modelMatrix, glm::vec3(28, 2, 30));
		    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
		    modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0.55f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(28, 2, 28));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0.25f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(28, 2, 32));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0.35f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(26.5f, 4, 27.5f));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.66f, 0.25f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(15, 1.5f, 30));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 1.5f, 3));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.75f, 0.75f, 0.75f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(15, 2, 30));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 2.5f, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.5f, 0.65f, 0.8f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(21, 15, 30));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1.5f, 1));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.7f, 0.25f, 0.20f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 11, 30));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8f, 0.6f, 0.8f));
            RenderMesh(meshes["motor"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.9f, 0.8f, 0.7f), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 11, 30));
		    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 1, 1));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
            RenderMesh(meshes["mountain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.56f, 0.25f, 0.20f), false, false);
        }

	    //depozit apa
        {
            if (train.pos.x < -5 && train.pos.x > -20 && train.pos.z > 14 && train.pos.z < 16) {
                textRenderer->RenderText("PRESS [3]", 600, 20, 1, glm::vec3(1, 1, 1));
            }

            for (int i = 0; i < 50; i++) {
                float offset = float(i) / 10.0f;
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-15.0f - offset, 1, 7.5f));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 6, 4));
                RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.74f, 0.84f, 0.84f), false, true);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-15.0f + offset, 1, 7.5f));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 6, 4));
                RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.74f, 0.84f, 0.84f), false, true);

			    float offsetGate = float(i) / 100.0f;
                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-15.3f + offsetGate, 1, 25));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 6, 4));
                RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.18f, 0.30f), false, false);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-15.0f - offsetGate, 1.2f, 25));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 0, 1));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 4, 4));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.18f, 0.30f), false, false);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-15.3f + offsetGate, 1, 25));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4.2f, 6, 4.3f));
			    modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.18f, 0.30f), false, false);

                modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(-15.0f + offsetGate, 1.2f, 25));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(0, 1, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(4, 7, 4));
                modelMatrix = glm::rotate(modelMatrix, RADIANS(90.0f), glm::vec3(1, 0, 0));
                RenderMesh(meshes["circle"], shaders["Tema2Shader"], modelMatrix, glm::vec3(0.18f, 0.18f, 0.30f), false, false);
            }

            float t = Engine::GetElapsedTime();
            float r = 0.5f + 0.5f * sin(t * 2.0f);
            float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
            float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);
            glm::vec3 neonColor(r, g, b);

		    glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-15, 4, 25));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 3, 3));
		    RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, neonColor, true, false);
        }

        //checkpoitns
        {
            //gara centrala
		    glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.2f, -25));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.2f, 2));
		    RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 0), false, false);

            //depozit gheata
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(20, 1.2f, -33));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.2f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 0), false, false);

            //depozit lemn
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(12, 1.2f, 38));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.2f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 0), false, false);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(-10, 1.2f, 14.5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 0.2f, 2));
            RenderMesh(meshes["sphere"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 0), false, false);
        }

		//verificare sfarsit joc
        {
            if (remainingTime <= 0) {
				gameOverDisplayed = true;
				gameState = GAME_OVER;
            }
        }
    }

	//State game == INSTRUCTIONS
    if (gameState == MENU) {
        float t = Engine::GetElapsedTime();
        float r = 0.5f + 0.5f * sin(t * 2.0f);
        float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
        float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);

        glm::vec3 neonColor(r, g, b);
        textRenderer->RenderText("INSTRUCTIONS [F]!", 5, 25, 1, neonColor);
        textRenderer->RenderText("START [ENTER]!", 5, 45, 1, neonColor);
        textRenderer->RenderText("work efficiency: " + std::to_string(workMaxEfficiency), 5, 65, 1, neonColor);
        textRenderer->RenderText("coins: " + std::to_string(totalCoins), 5, 85, 1, neonColor);
        textRenderer->RenderText("speed price: 10", 1070, 5, 1, glm::vec3(1, 1, 1));
        textRenderer->RenderText("work efficiency price: 15", 950, 25, 1, glm::vec3(1, 1, 1));

        if (showInstructions) {
			timerInstructions += deltaTimeSeconds;
            if (timerInstructions >= 10.0f) {
				showInstructions = false;
            }

			textRenderer->RenderText("You are a train conductor!", 350, 160, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("Take a task from the central station and complete it.", 350, 180, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("Move the train with [W] forward, [D] right and [A] left!", 350, 200, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("There are 3 types of materials that need to be taken by you.", 350, 220, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("With each task completed, a new round begin, but much tougher!", 350, 240, 1, glm::vec3(1, 1, 1));
            textRenderer->RenderText("Press [ENTER] to proceed. Good Luck!", 350, 260, 1, glm::vec3(1, 1, 1));
        }

        {
            textRenderer->RenderText("SPEED [1]", 10, 250, 1, glm::vec3(1, 1, 1));
            for (int i = 0; i < int(speedUP) + 1; i++) {//19 max
                float offset = float(i) / 10.0f;
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.9f + offset, 0));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0, 0.2f, 0.2f));
                RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 0, 0), false, false);
            }
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0, 2, 0.15f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);
        }

        {
            textRenderer->RenderText("EFFICIENCY [2]", 10, 500, 1, glm::vec3(1, 1, 1));
            for (int i = 0; i < workMaxEfficiency; i++) {
                float offset = float(i) / 10.0f;
                glm::mat4 modelMatrix = glm::mat4(1);
                modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.9f + offset, -0.4f));
                modelMatrix = glm::scale(modelMatrix, glm::vec3(0, 0.2f, 0.2f));
                RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 0.5f, 0), false, false);
            }
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -0.4f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0, 2, 0.15f));
            RenderMesh(meshes["plain"], shaders["Tema2Shader"], modelMatrix, glm::vec3(1, 1, 1), false, false);
        } 
        camera->Set(glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 0, 1));
    }

    if (gameState == GAME_OVER) {
        if (gameOverDisplayed) {
            float t = Engine::GetElapsedTime();
            float r = 0.5f + 0.5f * sin(t * 2.0f);
            float g = 0.5f + 0.5f * sin(t * 1.5f + 2.0f);
            float b = 0.5f + 0.5f * sin(t * 1.8f + 4.0f);
            glm::vec3 neonColor(r, g, b);
            textRenderer->RenderText("GAME OVER!", 550, 300, 2, neonColor);
            timerGameOver += deltaTimeSeconds;
            if (timerGameOver >= 5) {
                gameOverDisplayed = false;
            }
        }
        if (!gameOverDisplayed) {
            totalCoins += coins;
            timerGameOver = 0;
            gameState = MENU;
        }
	}
}

void Tema2::FrameEnd() {
    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, bool water, bool tunnel)
{
    if (!mesh || !shader)
        return;

    shader->Use();

    int location_model = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(location_model, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int location_view = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(location_view, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));

    int location_proj = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(location_proj, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    int location_color = glGetUniformLocation(shader->program, "object_color");
    glUniform3fv(location_color, 1, glm::value_ptr(color));

    float timeValue = Engine::GetElapsedTime();
    int location_time = glGetUniformLocation(shader->program, "time");
    glUniform1f(location_time, timeValue);

    if (tunnel) {
        glUniform1i(glGetUniformLocation(shader->program, "tunnel"), true);
    } else {
        glUniform1i(glGetUniformLocation(shader->program, "tunnel"), false);
	}

    if (water) {
        glUniform1i(glGetUniformLocation(shader->program, "water"), true);
    }
    else {
        glUniform1i(glGetUniformLocation(shader->program, "water"), false);
    }

    mesh->Render();
}

void Tema2::OnInputUpdate(float deltaTime, int mods) {
    float fovSpeed = 20.0f * deltaTime;
    float orthoSpeed = 2.0f * deltaTime;

    if (perspective) {
        if (window->KeyHold(GLFW_KEY_Z)) FoV += fovSpeed;
        if (window->KeyHold(GLFW_KEY_X)) FoV -= fovSpeed;
        projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, 0.01f, 200.0f);
    }

    if (!perspective) {
        if (window->KeyHold(GLFW_KEY_Z)) { orthoWidth += orthoSpeed; orthoHeight += orthoSpeed; }
        if (window->KeyHold(GLFW_KEY_X)) { orthoWidth -= orthoSpeed; orthoHeight -= orthoSpeed; }
        projectionMatrix = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, 0.01f, 200.0f);
    }
    
    //de aici se misca camera, vreau sa se miste cu trenul
    
        float cameraSpeed = 15.0f;
        if (window->KeyHold(GLFW_KEY_LEFT_SHIFT)) {
            if (window->KeyHold(GLFW_KEY_W)) {
                // TODO(student): Translate the camera forward
                camera->TranslateForward(cameraSpeed * deltaTime);

            }

            if (window->KeyHold(GLFW_KEY_A)) {
                // TODO(student): Translate the camera to the left
                camera->TranslateRight(-cameraSpeed * deltaTime);

            }

            if (window->KeyHold(GLFW_KEY_S)) {
                // TODO(student): Translate the camera backward
                camera->TranslateForward(-cameraSpeed * deltaTime);

            }

            if (window->KeyHold(GLFW_KEY_D)) {
                // TODO(student): Translate the camera to the right
                camera->TranslateRight(cameraSpeed * deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_Q)) {
                // TODO(student): Translate the camera downward
                camera->TranslateUpward(-cameraSpeed * deltaTime);
            }

            if (window->KeyHold(GLFW_KEY_E)) {
                // TODO(student): Translate the camera upward
                camera->TranslateUpward(cameraSpeed * deltaTime);
            }
        }
        
}

void Tema2::OnKeyPress(int key, int mods) {
    {
        if (key == GLFW_KEY_T)
        {
            renderCameraTarget = !renderCameraTarget;
        }
        if (key == GLFW_KEY_P) {
            perspective = true;
            projectionMatrix = glm::perspective(RADIANS(FoV), window->props.aspectRatio, 0.01f, 200.0f);
        }
        else if (key == GLFW_KEY_O) {
            perspective = false;
            projectionMatrix = glm::ortho(-orthoWidth, orthoWidth, -orthoHeight, orthoHeight, 0.01f, 200.0f);
        }
    }

    if (gameState == GAME) {
        if (key == GLFW_KEY_W) {
            train.chosenDir = NORTH;
        }
        else if (key == GLFW_KEY_A) {
            train.chosenDir = WEST;
        }
        else if (key == GLFW_KEY_S) {
            train.chosenDir = SOUTH;
        }
        else if (key == GLFW_KEY_D) {
            train.chosenDir = EAST;
        }

        if (key == GLFW_KEY_F) {
            if (train.pos.x == 0 && train.pos.z > -30 && train.pos.z < -10) {
                if (!showTask) {
                    randIce = rand() % wave + 1;
                    randWood = rand() % wave + 1;
                    randWater = rand() % wave + 1;
                    ice = 0;
                    wood = 0;
                    water = 0;
                }
                showTask = true;

                if (ice == randIce && wood == randWood && water == randWater) {
                    if (ice != 0 && wood != 0 && water != 0) {
                        coins += wave * 10;
                        wave++;
                        remainingTime = 60.0f;
                        randIce = rand() % wave + 1;
                        randWood = rand() % wave + 1;
                        randWater = rand() % wave + 1;
                        ice = 0;
                        wood = 0;
                        water = 0;
                    }
                }
            }
        }

        if (key == GLFW_KEY_1) {
            if (train.pos.x == 20 && train.pos.z > -45 && train.pos.z < -30 && ice < randIce) {
                int miss = randIce - ice;
                int take = std::min(miss, workEfficiency);
                ice += take;
                workEfficiency -= take;
            }
        }

        if (key == GLFW_KEY_2) {
            if (train.pos.x > 5 && train.pos.x < 20 && train.pos.z == 38 && wood < randWood) {
                int miss = randWood - wood;
                int take = std::min(miss, workEfficiency);
                wood += take;
                workEfficiency -= take;
            }
        }

        if (key == GLFW_KEY_3) {
            if (train.pos.x < -5 && train.pos.x > -20 && train.pos.z == 14.5f && water < randWater) {
                int miss = randWater - water;
                int take = std::min(miss, workEfficiency);
                water += take;
                workEfficiency -= take;
            }
        }

        if (key == GLFW_KEY_SPACE) {
            train.moving = !train.moving;
        }
    }

    if (gameState == MENU) {
        if (key == GLFW_KEY_ENTER) {
            showTask = false;
            wave = 1;
            coins = 0;
            remainingTime = 60.0f;
            workEfficiency = workMaxEfficiency;
            StartGame();
            gameState = GAME;
        }

        if (key == GLFW_KEY_F) {
			timerInstructions = 0.0f;
            showInstructions = true;
        }

        if (key == GLFW_KEY_1) {
            if (totalCoins >= 10 && speedUP < 18) {
                speedUP++;
                totalCoins -= 10;
            }
        }

        if (key == GLFW_KEY_2) {
            if (totalCoins >= 15 && workMaxEfficiency < 19) {
                workMaxEfficiency++;
                totalCoins -= 15;
            }
        }
	}

    if (gameState == GAME_OVER) {
        totalCoins += coins;
        if (key == GLFW_KEY_R) {
			showTask = false;
            wave = 1;
            coins = 0;
            remainingTime = 60.0f;
            StartGame();
            gameState = GAME;
            workEfficiency = workMaxEfficiency;
        }
	}
}

void Tema2::OnKeyRelease(int key, int mods) {}

void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) {
    // Add mouse move event
    float sensivityOX = 0.005f;
    float sensivityOY = 0.005f;
    if (window->KeyHold(GLFW_MOUSE_BUTTON_RIGHT)) {
        if (window->GetSpecialKeyState() == 0) {
            //renderCameraTarget = false;

            camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
            camera->RotateFirstPerson_OY(-deltaX * sensivityOY);

        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            //renderCameraTarget = true;

            camera->RotateThirdPerson_OX(-deltaY * sensivityOX);
            camera->RotateThirdPerson_OY(-deltaX * sensivityOY);

        }
    }
}

void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) {}

void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) {}

void Tema2::OnWindowResize(int width, int height) {}

Tema2::Rail* Tema2::NextRail(Train& train)
{
    Rail* r = train.rail;

    Rail* tryWASD = r->next[train.chosenDir];
    if (tryWASD != nullptr) {
        train.dir = train.chosenDir;
        return tryWASD;
    }

    if (r->next[train.dir] != nullptr)
        return r->next[train.dir];

	if (r->next[EAST] != nullptr)
        return r->next[EAST];
    if (r->next[WEST] != nullptr)
		return r->next[WEST];
}

void Tema2::UpdateTrain(float deltaTime)
{
    if (!train.rail || !train.moving) return;

    float railLength = glm::length(train.rail->end - train.rail->start);
    train.progress += ((train.speed + speedUP) / railLength) * deltaTime;

    while (train.progress >= 1.0f) {
        train.progress -= 1.0f;

        Rail* next = NextRail(train);
        if (!next) {
            train.moving = false;
            train.progress = 1;
            break;
        }

        train.rail = next;
        railLength = glm::length(train.rail->end - train.rail->start);
    }
    train.pos = glm::mix(train.rail->start, train.rail->end, train.progress);
}

void Tema2::StartGame() {
    Rail* r1 = new Rail();
    Rail* r2 = new Rail();
    Rail* r3 = new Rail();
    Rail* r4 = new Rail();
    Rail* r5 = new Rail();
    Rail* r6 = new Rail();
    Rail* r7 = new Rail();
    Rail* r8 = new Rail();
    Rail* r9 = new Rail();
    Rail* r10 = new Rail();
    Rail* r11 = new Rail();
    Rail* r12 = new Rail();
    Rail* r13 = new Rail();
    Rail* r14 = new Rail();
    Rail* r15 = new Rail();
    Rail* r16 = new Rail();
    Rail* r17 = new Rail();
    Rail* r18 = new Rail();
    Rail* r19 = new Rail();
    Rail* r20 = new Rail();
    Rail* r21 = new Rail();
    Rail* r22 = new Rail();
    Rail* r23 = new Rail();
    Rail* r24 = new Rail();
    Rail* r25 = new Rail();
    Rail* r26 = new Rail();
    Rail* r27 = new Rail();
    Rail* r28 = new Rail();
    Rail* r29 = new Rail();
    Rail* r30 = new Rail();
    Rail* r31 = new Rail();
    Rail* r32 = new Rail();
    Rail* r33 = new Rail();
    Rail* r34 = new Rail();

    float y_lvl = 3.0f;

    r1->start = glm::vec3(0, y_lvl, 0.5f);
    r1->end = glm::vec3(0, y_lvl, -50);

    r2->start = r1->end;
    r2->end = glm::vec3(-30, y_lvl, -50);

    r3->start = r2->end;
    r3->end = glm::vec3(-30, y_lvl, 0.5f);

    r4->start = r1->end;
    r4->end = glm::vec3(20, y_lvl, -50);

    r5->start = r4->end;
    r5->end = glm::vec3(20, y_lvl, 0.5f);

    r6->start = r5->end;
    r6->end = glm::vec3(20, y_lvl, 14.5f);

    r7->start = r6->end;
    r7->end = glm::vec3(20, y_lvl, 38);

    r8->start = r7->end;
    r8->end = glm::vec3(0, y_lvl, 38);

    r9->start = r8->end;
    r9->end = glm::vec3(0, y_lvl, 14.5f);

    r10->start = r9->end;
    r10->end = r1->start;

    r11->start = r3->end;
    r11->end = glm::vec3(-30, y_lvl, 14.5f);

    r12->start = r11->end;
    r12->end = glm::vec3(-30, y_lvl, 38);

    r13->start = r12->end;
    r13->end = r9->start;

    r14->start = r11->end;
    r14->end = r10->start;

    r15->start = r6->end;
    r15->end = r10->start;

    r16->start = r1->start;
    r16->end = glm::vec3(-30, y_lvl, 0.5f);

    r17->start = r1->start;
    r17->end = r6->start;

    r18->start = glm::vec3(0, y_lvl, 14.5f);
    r18->end = glm::vec3(20, y_lvl, 14.5f);

    r19->start = glm::vec3(20, y_lvl, 14.5f);
    r19->end = r6->start;

    r20->start = r19->end;
    r20->end = r5->start;

    r21->start = r20->end;
    r21->end = r4->start;

    r22->start = r21->end;
    r22->end = r1->start;

    r23->start = r19->end;
    r23->end = r1->start;

    r24->start = r1->start;
    r24->end = r10->start;

    r25->start = r10->start;
    r25->end = r9->start;

    r26->start = r10->start;
    r26->end = r11->end;

    r27->start = r13->end;
    r27->end = r13->start;

    r28->start = r8->end;
    r28->end = r8->start;

    r29->start = r7->end;
    r29->end = r7->start;

    r30->start = r11->end;
    r30->end = r11->start;

    r31->start = r12->end;
    r31->end = r12->start;

    r32->start = r3->end;
    r32->end = r3->start;

    r33->start = r16->end;
    r33->end = r16->start;

    r34->start = r2->end;
    r34->end = r2->start;

    r34->next[NORTH] = r4;
    r34->next[EAST] = r22;

    r33->next[NORTH] = r17;
    r33->next[WEST] = r1;
    r33->next[EAST] = r24;

    r32->next[EAST] = r34;

    r31->next[NORTH] = r30;
    r31->next[EAST] = r14;

    r30->next[NORTH] = r32;
    r30->next[EAST] = r33;

    r22->next[NORTH] = r24;
    r22->next[EAST] = r16;
    r22->next[WEST] = r17;

    r26->next[WEST] = r12;
    r26->next[EAST] = r30;

    r27->next[EAST] = r31;

    r28->next[WEST] = r29;

    r29->next[NORTH] = r19;
    r29->next[WEST] = r15;

    r25->next[EAST] = r27;
    r25->next[WEST] = r28;

    r24->next[NORTH] = r25;
    r24->next[EAST] = r26;
    r24->next[WEST] = r18;

    r23->next[NORTH] = r16;
    r23->next[EAST] = r1;
    r23->next[WEST] = r24;

    r20->next[WEST] = r21;

    r21->next[WEST] = r22;
    r21->next[NORTH] = r2;

    r19->next[NORTH] = r20;
    r19->next[WEST] = r23;

    r18->next[EAST] = r7;
    r18->next[WEST] = r19;

    r1->next[WEST] = r2;
    r1->next[EAST] = r4;

    r2->next[WEST] = r3;

    r3->next[WEST] = r33;
    r3->next[NORTH] = r11;

    r4->next[EAST] = r5;

    r5->next[NORTH] = r6;
    r5->next[EAST] = r23;

    r6->next[NORTH] = r7;
    r6->next[EAST] = r15;

    r7->next[EAST] = r8;

    r8->next[EAST] = r9;
    r8->next[NORTH] = r27;

    r9->next[NORTH] = r10;
    r9->next[EAST] = r18;
    r9->next[WEST] = r26;

    r10->next[NORTH] = r1;
    r10->next[WEST] = r16;
    r10->next[EAST] = r17;

    r11->next[NORTH] = r12;
    r11->next[WEST] = r14;

    r12->next[WEST] = r13;

    r13->next[NORTH] = r28;
    r13->next[WEST] = r9;

    r14->next[NORTH] = r18;
    r14->next[EAST] = r25;
    r14->next[WEST] = r10;

    r15->next[NORTH] = r26;
    r15->next[WEST] = r25;
    r15->next[EAST] = r10;

    r16->next[WEST] = r11;
    r16->next[EAST] = r32;

    r17->next[WEST] = r20;
    r17->next[EAST] = r6;

    train.rail = r1;
    train.progress = 0.0f;
    train.pos = r1->start;
    train.moving = false;
    train.chosenDir = NORTH;
}
