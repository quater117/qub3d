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
#include <qub3d/chunk.hpp>

#include <glm/gtc/matrix_transform.hpp>

int main(int argc, char** argv)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	
	qub3d::Window window("Qub3d Game", 1280, 720);

	qub3d::ShaderPipeline pipeline;
	pipeline.addStage(qub3d::ShaderPipelineStage::VERTEX, "assets/shaders/basic_vertex.glsl");
	pipeline.addStage(qub3d::ShaderPipelineStage::FRAGMENT, "assets/shaders/basic_fragment.glsl");
	pipeline.build();
	
	pipeline.bind();

	qub3d::FlyCamera camera(window);
	
	qub3d::Chunk chunk;
	chunk.fill();
	
	for (int x = 1; x < qub3d::Chunk::SIZE - 1; x++) 
	{
		chunk.destroyBlockAt(x, 0, 0);
	}

	glm::mat4 projection = glm::perspective(45.f, 1280.f / 720.f, 0.1f, 100.f);

	glEnable(GL_DEPTH_TEST);

	unsigned int lastTicks = SDL_GetTicks();
	while (window.isRunning())
	{
		window.pollEvents();
		
		unsigned int currentTicks = SDL_GetTicks();
		float dt = (currentTicks - lastTicks) / 1000.f;	// the "/ 1000" here converts the delta time from milliseconds to seconds.
		camera.tick(dt);

		pipeline.setUniform("mvp", projection * camera.calculateViewMatrix());

		glClearColor(198 / 255.f, 220/255.f, 255/255.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		chunk.draw();

		window.swapBuffers();

		lastTicks = currentTicks;
		SDL_Delay(1000 / 60);
	}

	SDL_Quit();

	return 0;
}