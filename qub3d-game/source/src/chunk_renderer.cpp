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

#include <qub3d/chunk_renderer.hpp>

#include <glm/glm.hpp>

#include <cstring>

using namespace qub3d;

Chunk::Chunk()
{}

const int NUM_VERTICES_IN_CUBE = 8;
const int NUM_INDICES_IN_CUBE = 6 * 6;

const int CHUNK_NUM_VERTICES = Chunk::SIZE * Chunk::SIZE * Chunk::SIZE * NUM_VERTICES_IN_CUBE;
const int CHUNK_NUM_INDICES = Chunk::SIZE * Chunk::SIZE * Chunk::SIZE * NUM_INDICES_IN_CUBE;

void Chunk::fill()
{
	static const glm::vec3 CUBE_VERTICES[] = {
		glm::vec3(-1.0f, -1.0f,  1.0f),
		glm::vec3(1.0, -1.0,  1.0),
		glm::vec3(1.0,  1.0,  1.0),
		glm::vec3(-1.0,  1.0,  1.0),
		
		glm::vec3(-1.0, -1.0, -1.0),
		glm::vec3(1.0, -1.0, -1.0),
		glm::vec3(1.0,  1.0, -1.0),
		glm::vec3(-1.0,  1.0, -1.0),
	};

	static const unsigned short CUBE_INDICES[] = {
		0, 1, 2,
		2, 3, 0,
		
		1, 5, 6,
		6, 2, 1,
		
		7, 6, 5,
		5, 4, 7,
		
		4, 0, 3,
		3, 7, 4,
		
		4, 5, 1,
		1, 0, 4,
		
		3, 2, 6,
		6, 7, 3,
	};

	glm::vec3 *chunkVertices = new glm::vec3[CHUNK_NUM_VERTICES];
	unsigned short *chunkIndices = new unsigned short[CHUNK_NUM_INDICES];
	
	short indexOffset = 0;
	
	for (int z = 0; z < SIZE; z++)
	{
		for (int y = 0; y < SIZE; y++)
		{
			for (int x = 0; x < SIZE; x++)
			{
				// This might look very scary, but it's really not.
				// Basically it derives from the algorithm to convert 3D array indices/coordinates into a 1D array
				// coordinate.
				//
				// ALGORITHM:
				//			x + width * (y + depth * z)
				//
				// the (x,y,z) coordinates get multiplied out by the NUM_VERTICES_IN_CUBE because the first block is represented by
				// the first NUM_VERTICES_IN_CUBE number of vertices, 
				// the second block is represented by the second NUM_VERTICES_IN_CUBE set of vertices. etc...

				int iv = (x * NUM_VERTICES_IN_CUBE) + SIZE * ((y * NUM_VERTICES_IN_CUBE) + SIZE * (z * NUM_VERTICES_IN_CUBE));
				std::memcpy(chunkVertices + iv, CUBE_VERTICES, sizeof(CUBE_VERTICES));
				
				for (int j = iv; j < iv + NUM_VERTICES_IN_CUBE; j++)
				{
					chunkVertices[j].x += x * 3;
					chunkVertices[j].y += y * 3;
					chunkVertices[j].z += z * 3;
				}

				int ii = (x * NUM_INDICES_IN_CUBE) + SIZE * ((y * NUM_INDICES_IN_CUBE) + SIZE * (z * NUM_INDICES_IN_CUBE));
				std::memcpy(chunkIndices + ii, CUBE_INDICES, sizeof(CUBE_INDICES));
				
				for (int j = ii; j < ii + NUM_INDICES_IN_CUBE; j++)
				{
					chunkIndices[j] += indexOffset;
				}
				indexOffset += 8;
			}
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, CHUNK_NUM_VERTICES * sizeof(glm::vec3), chunkVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, CHUNK_NUM_INDICES * sizeof(unsigned short), chunkIndices, GL_STATIC_DRAW);
	
	for (int z = 0; z < Chunk::SIZE; ++z)
	{
		for (int y = 0; y < Chunk::SIZE; ++y)
		{
			for (int x = 0; x < Chunk::SIZE; ++x)
			{
				m_blocks[z][x][y] = true;
			}
		}
	}

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

void Chunk::draw()
{
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	glDrawElements(GL_TRIANGLES, CHUNK_NUM_INDICES, GL_UNSIGNED_SHORT, 0);
}