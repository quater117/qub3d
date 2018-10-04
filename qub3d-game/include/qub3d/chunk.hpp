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

#pragma once

#include <GL/glew.h>
#include <glm/vec3.hpp>

#include <vector>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <unordered_map>

namespace qub3d
{
	enum class BlockFace
	{
		FRONT = 0,
		BACK = 1,
		RIGHT = 2,
		LEFT = 3,
		BOTTOM = 4,
		TOP = 5
	};

	enum class BlockType
	{
		AIR,
		STONE
	};

	class Chunk
	{
	public:
		typedef std::vector<std::vector<std::vector<BlockType>>> CPUBlockArray;

		Chunk();

		void fill(int size);
		
		inline GLuint getVAO() const { return m_vao; }
		inline GLuint getVBO() const { return m_vbo; }
		inline GLuint getEBO() const { return m_ebo; }

		void draw();

		void placeBlockAt(int x, int y, int z);
		void destroyBlockAt(int x, int y, int z);
		
		BlockType getBlockAtPos(float x, float y, float z);

	private:
		void destroyOpenGLData();
		void setChunkSize(int size);
		void build();

		void addBlockFace(glm::vec3* vertices, int x, int y, int z, BlockFace face);
		void removeBlockFace(glm::vec3 *vertices, int x, int y, int z, BlockFace face);

	private:
		GLuint m_vao, m_vbo, m_ebo, m_tbo;
		
		bool m_filled;

		int m_totalVerticesInChunk, m_totalIndicesInChunk, m_totalUvsInChunk;
		int m_chunkSize;

		CPUBlockArray m_blocks;
	};
}
