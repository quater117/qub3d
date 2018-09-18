/*
*	 Copyright (C) 2018 Qub³d Engine Group.
*	 All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification,
*  are permitted provided that the following conditions are met:
*
*  1. Redistributions of source code must retain the above copyright notice, this
*  list of conditions and the following disclaimer.
*
*  2. Redistributions in binary form must reproduce the above copyright notice,
*  this list of conditions and the following disclaimer in the documentation and/or
*  other materials provided with the distribution.
*
*  3. Neither the name of the copyright holder nor the names of its contributors
*  may be used to endorse or promote products derived from this software without
*  specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
*  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
*  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
*  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
*  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
*  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
*  ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
*  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <qub3d/window.hpp>
#include <qub3d/shader_pipeline.hpp>
#include <SDL.h>

#include <qub3d/imgui.hpp>
#include <qub3d/fly_cam.hpp>

#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	qub3d::Window window("Qub3d Game", 1280, 720);

	qub3d::ShaderPipeline pipeline;
	pipeline.addStage(qub3d::ShaderPipelineStage::VERTEX, "assets/shaders/basic_vertex.glsl");
	pipeline.addStage(qub3d::ShaderPipelineStage::FRAGMENT, "assets/shaders/basic_fragment.glsl");
	pipeline.build();
	
	float vertices[] = {
		// front
		-1.0, -1.0,  1.0,
		1.0, -1.0,  1.0,
		1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		1.0, -1.0, -1.0,
		1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0,
	};

	unsigned int indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		1, 5, 6,
		6, 2, 1,
		// back
		7, 6, 5,
		5, 4, 7,
		// left
		4, 0, 3,
		3, 7, 4,
		// bottom
		4, 5, 1,
		1, 0, 4,
		// top
		3, 2, 6,
		6, 7, 3,
	};

	struct Material {
		glm::vec4 color;

	};

	struct Mesh {
		std::vector<glm::vec3> vertices;
	};

	Mesh mesh;
	mesh.vertices = {
		{ -0.5f, -0.5f, 0.0f },
		{ -0.5f, 0.5f,  0.0f },
		{ 0.5f,  0.5f,  0.0f },
		{ 0.5f, -0.5f,  0.0f }
	};

	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	unsigned int EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	pipeline.bind();

	qub3d::FlyCamera camera(window);

	glm::vec3 testVec3(0);
	glm::mat4 testMat4(1);
	
	glm::mat4 projection = glm::perspective(45.f, 1280.f / 720.f, 0.1f, 100.f);

	glEnable(GL_DEPTH_TEST);
	
	const int TICKS_PER_SECOND = 60;
	const int SKIP_TICKS = 1000 / TICKS_PER_SECOND;
	const int MAX_FRAMESKIP = 10;

	unsigned int nextTicks = SDL_GetTicks();
	int loops;

	unsigned int lastTicks = SDL_GetTicks();

	while (window.isRunning())
	{
		window.pollEvents();
		
		unsigned int currentTicks = SDL_GetTicks();
		float dt = (currentTicks - lastTicks) / 1000.f;	// the "/ 1000" here converts the delta time from milliseconds to seconds.
		
		loops = 0;
		while (SDL_GetTicks() > nextTicks && loops < MAX_FRAMESKIP) {
			camera.tick(dt);
			pipeline.setUniform("mvp", projection * camera.calculateViewMatrix());

			nextTicks += SKIP_TICKS;
			loops++;
		}

		ImGui::Begin("ImGui Extensions Examples");
			ImGui::InputVector3("Test Vector", &testVec3);
			ImGui::NewLine();
			ImGui::InputMatrix4x4("Test Matrix", &testMat4);
		ImGui::End();

		glClearColor(198 / 255.f, 220/255.f, 255/255.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	
		window.swapBuffers();

		lastTicks = currentTicks;
	}

	SDL_Quit();

	return 0;
}