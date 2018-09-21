/*
*	 Copyright (C) 2018 Qub�d Engine Group.
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

#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>

#include <array>
#include <vector>

namespace qub3d
{
struct Voxel
{
	bool active;
};

class Chunk
{
public:
	Chunk();
	~Chunk();

	void initVoxelData(glm::vec3& size);
	void initMeshingData();

	std::vector<glm::vec4> findCubesToRender();

	void fill(int size);

	inline GLuint getVAO() const { return m_vao; }
	inline GLuint getVBO() const { return m_vbo; }
	inline GLuint getEBO() const { return m_ebo; }

	void draw();

	void placeBlockAt(int x, int y, int z);
	void destroyBlockAt(int x, int y, int z);

private:
	void destroyOpenGLData();
	void setChunkSize(int size);

private:
	GLuint m_vao, m_vbo, m_ebo, m_tbo;

	bool m_filled;

	int m_totalVerticesInChunk, m_totalIndicesInChunk, m_totalUvsInChunk;
	int m_chunkSize;

	glm::vec3 m_voxelDataSize;
	Voxel* m_voxels;
};
}