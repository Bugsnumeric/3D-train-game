#pragma once

#include "components/simple_scene.h"
#include "lab_m1/Tema2/myCamera.h"
#include "components/text_renderer.h"

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;
        enum Direction {
            NORTH = 0,
            EAST = 1,
            SOUTH = 2,
            WEST = 3,
        };

        struct Rail {
            glm::vec3 start;
            glm::vec3 end;
            Rail* next[4];
        };

        struct Train {
            Rail* rail = nullptr;
            float progress = 0.2f;
            float speed = 15;
            glm::vec3 pos;
            Direction dir = NORTH;
            Direction chosenDir;
            bool moving = true;
        };

        Train train;
        Train wagon;
        bool showTask = false;

        int ice = 0;
        int wood = 0;
        int water = 0;
        int randIce = 0;
		int randWood = 0;
		int randWater = 0;
        float speedUP = 0.0f;
		int workMaxEfficiency = 1;
        int workEfficiency = 1;
        int wave = 1;
        int coins = 0;
		int totalCoins = 0;
		bool showInstructions = false;
		float timerInstructions = 0.0f;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;
		void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, const glm::vec3& color, bool water, bool tunnel);
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void UpdateTrain(float deltaTimeSeconds);
        Rail* NextRail(Train& train);
        void Tema2::StartGame();

    protected:
        enum GameState {
            MENU,
            GAME,
            GAME_OVER,
        };

        GameState gameState = MENU;
		float timerGameOver = 0.0f;
		bool gameOverDisplayed = false;
        gfxc::TextRenderer* textRenderer;

        implemented::Camera* camera;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;
        bool perspective;
        float FoV;
        float orthoWidth;
        float orthoHeight;

		int riverLength = 70;
		int riverWidth = 5;
        
		float remainingTime = 60.0f;
        glm::mat3 modelMatrix;
    };
}
