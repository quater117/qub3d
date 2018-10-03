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

#include <qub3d/chunk.hpp>

#include <glm/glm.hpp>

#include <cstring>

using namespace qub3d;

void Chunk::destroyOpenGLData()
{
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);

	m_filled = false;
}

Chunk::Chunk()
	: m_vao(-1), m_vbo(-1), m_ebo(-1),
	m_filled(false),
	m_totalIndicesInChunk(0), m_totalVerticesInChunk(0),
	m_chunkSize(0),
	m_totalUvsInChunk(0)
{}

const int NUM_VERTICES_IN_CUBE = 36;
const int NUM_UVS_IN_CUBE = 36;

static const glm::vec3 CUBE_VERTICES[] = {
	// front
	glm::vec3(-1.f,-1.f,-1.f),
	glm::vec3(1.f,-1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(-1.f,1.f,-1.f),
	glm::vec3(-1.f,-1.f,-1.f),

	// back
	glm::vec3(-1.f,-1.f,1.f),
	glm::vec3(1.f,-1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, -1.f,1.f),

	// left
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),

	// right
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),

	// bottom
	glm::vec3(-1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f, -1.f),

	// top
	glm::vec3(-1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f, -1.f)
};

static const glm::vec2 CUBE_UV[] = {
	// front
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),

	// back
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	
	// left
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),

	// right
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),

	// bottom
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f),

	// top
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f)
};

static const glm::vec3 EMPTY_BLOCK[] = {
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f),
	glm::vec3(0.f), glm::vec3(0.f), glm::vec3(0.f)
};

void Chunk::setChunkSize(int size) {
	int sizeCubed = size * size * size;

	m_chunkSize = size;
	m_totalVerticesInChunk = sizeCubed * NUM_VERTICES_IN_CUBE;
	m_totalUvsInChunk = NUM_UVS_IN_CUBE * sizeCubed;

	m_blocks.resize(size);

	for (int x = 0; x < m_blocks.size(); x++)
	{
		m_blocks[x].resize(size);
		for (int y = 0; y < m_blocks.size(); y++)
		{
			m_blocks[x][y].resize(size);
			for (int z = 0; z < m_blocks.size(); z++)
			{
				// Don't ask about this... the C++ standard thinks that its being clever
				m_blocks[x][y][z] = BlockType::STONE;
			}
		}
	}
}

void qub3d::Chunk::build()
{
	const int size = m_chunkSize;
	if (m_filled)
		this->destroyOpenGLData();

	glm::vec3 *chunkVertices = new glm::vec3[m_totalVerticesInChunk];
	glm::vec2 *chunkUVs = new glm::vec2[m_totalUvsInChunk];

	for (int z = 0; z < size; z++)
	{
		for (int y = 0; y < size; y++)
		{
			for (int x = 0; x < size; x++)
			{
				int i = (x * NUM_VERTICES_IN_CUBE) + size * ((y * NUM_VERTICES_IN_CUBE) + size * (z * NUM_VERTICES_IN_CUBE));

				std::memcpy(chunkVertices + i, EMPTY_BLOCK, sizeof(EMPTY_BLOCK));
				std::memcpy(chunkUVs + i, CUBE_UV, sizeof(CUBE_UV));

				if (m_blocks[x][y][z] == BlockType::AIR) continue;

				if (x == 0 || m_blocks[x - 1][y][z] == BlockType::AIR) addBlockFace(chunkVertices, x, y, z, BlockFace::RIGHT);
				if (x == size - 1 || m_blocks[x + 1][y][z] == BlockType::AIR) addBlockFace(chunkVertices, x, y, z, BlockFace::LEFT);

				if (y == 0 || m_blocks[x][y - 1][z] == BlockType::AIR) addBlockFace(chunkVertices, x, y, z, BlockFace::BOTTOM);
				if (y == size - 1 || m_blocks[x][y + 1][z] == BlockType::AIR) addBlockFace(chunkVertices, x, y, z, BlockFace::TOP);

				if (z == 0 || m_blocks[x][y][z - 1] == BlockType::AIR) addBlockFace(chunkVertices, x, y, z, BlockFace::FRONT);
				if (z == size - 1 || m_blocks[x][y][z + 1] == BlockType::AIR) addBlockFace(chunkVertices, x, y, z, BlockFace::BACK);
			}
		}
	}

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_totalVerticesInChunk * sizeof(glm::vec3), chunkVertices, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &m_tbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_tbo);
	glBufferData(GL_ARRAY_BUFFER, m_totalUvsInChunk * sizeof(glm::vec2), chunkUVs, GL_STATIC_DRAW);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(1);

	m_filled = true;

	delete chunkVertices;
	delete chunkUVs;
}

void Chunk::addBlockFace(glm::vec3* vertices, int x, int y, int z, BlockFace face)
{
	static const int NUM_VERTS_IN_FACE = 6;
	static const int SIZEOF_FACE_VERTS_BYTES = NUM_VERTS_IN_FACE * sizeof(glm::vec3);

	const int size = m_chunkSize;
	
	int i = (x * NUM_VERTICES_IN_CUBE) + size * ((y * NUM_VERTICES_IN_CUBE) + size * (z * NUM_VERTICES_IN_CUBE));
	int blockMemoryOffsetIndex = static_cast<int>(face) * NUM_VERTS_IN_FACE;

	memcpy(vertices + i + blockMemoryOffsetIndex, CUBE_VERTICES + blockMemoryOffsetIndex, SIZEOF_FACE_VERTS_BYTES);
	for (int j = i + blockMemoryOffsetIndex; j < i + blockMemoryOffsetIndex + NUM_VERTS_IN_FACE; j++)
	{
		vertices[j].x += x * 2;
		vertices[j].y += y * 2;
		vertices[j].z += z * 2;
	}

}

void Chunk::fill(int size)
{
	this->setChunkSize(size);
	build();
}

void Chunk::removeBlockFace(glm::vec3* vertices, int x, int y, int z, BlockFace face)
{
	static const int NUM_VERTS_IN_FACE = 6;
	
	const int size = m_chunkSize;

	int i = (x * NUM_VERTICES_IN_CUBE) + size * ((y * NUM_VERTICES_IN_CUBE) + size * (z * NUM_VERTICES_IN_CUBE));
	int blockMemoryOffsetIndex = static_cast<int>(face) * NUM_VERTS_IN_FACE;

	for (int j = i + blockMemoryOffsetIndex; j < i + blockMemoryOffsetIndex + NUM_VERTS_IN_FACE; j++) {
		vertices[j].x = 0.f;
		vertices[j].y = 0.f;
		vertices[j].z = 0.f;
	}
}


void Chunk::placeBlockAt(int x, int y, int z)
{
	if (x >= m_chunkSize || y >= m_chunkSize || z >= m_chunkSize)
		return;

	if (x < 0 || y < 0 || z < 0)
		return;

	m_blocks[x][y][z] = BlockType::STONE;
	build();

}

void Chunk::destroyBlockAt(int x, int y, int z)
{
	if (x >= m_chunkSize || y >= m_chunkSize || z >= m_chunkSize)
		return;

	if (x < 0 || y < 0 || z < 0)
		return;

	m_blocks[x][y][z] = BlockType::AIR;

	build();
}

BlockType Chunk::getBlockAtPos(float x, float y, float z)
{
	if (x < 0 || y < 0 || z < 0) return BlockType::AIR;
	if (x > m_chunkSize || y > m_chunkSize || z > m_chunkSize) return BlockType::AIR;
	return m_blocks[(int)x][(int)y][(int)z];
}

void Chunk::draw()
{
	glBindVertexArray(m_vao);
	
	glDrawArrays(GL_TRIANGLES, 0, m_totalVerticesInChunk);
}