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

#include <iostream>
#include <cstring>
#include <map>

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
	m_totalUvsInChunk(0),
	m_voxelDataSize(glm::vec3(0,0,0)),
	m_voxels(nullptr)
{}

Chunk::~Chunk()
{

}

const int NUM_VERTICES_IN_CUBE = 36;
const int NUM_UVS_IN_CUBE = 36;

const int VERTS_PER_FACE = 6;
const int UVS_PER_FACE = 6;

static std::vector<int> meshID;

static std::vector<std::vector<const glm::vec3*>> meshList;
static std::vector<std::vector<const glm::vec2*>> meshUVList;

int t = 32;
int b = 16;
int l = 8;
int r = 4;
int f = 2;
int k = 1;

static const glm::vec3 FRONT[] = {
	glm::vec3(-1.f,-1.f,1.f),
	glm::vec3(1.f,-1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, -1.f,1.f)
};
static const glm::vec3 BACK[] = {
	glm::vec3(-1.f,-1.f,-1.f),
	glm::vec3(1.f,-1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(-1.f,1.f,-1.f),
	glm::vec3(-1.f,-1.f,-1.f)
};
static const glm::vec3 LEFT[] = {
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f)
};
static const glm::vec3 RIGHT[] = {
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,1.f),
	glm::vec3(1.f, 1.f,1.f)
};
static const glm::vec3 TOP[] = {
	glm::vec3(-1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f, -1.f)
};
static const glm::vec3 BOTTOM[] = {
	glm::vec3(-1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f, -1.f)
};

static const glm::vec2 FRONT_UV[] = {
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
};
static const glm::vec2 BACK_UV[] = {
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f)
};
static const glm::vec2 LEFT_UV[] = {
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f)
};
static const glm::vec2 RIGHT_UV[] = {
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f)
};
static const glm::vec2 TOP_UV[] = {
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f)
};
static const glm::vec2 BOTTOM_UV[] = {
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f)
};

static const glm::vec3 CUBE_VERTICES[] = {
	//back
	glm::vec3(-1.f,-1.f,-1.f),
	glm::vec3(1.f,-1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(1.f,1.f,-1.f),
	glm::vec3(-1.f,1.f,-1.f),
	glm::vec3(-1.f,-1.f,-1.f),

	//front
	glm::vec3(-1.f,-1.f,1.f),
	glm::vec3(1.f,-1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, -1.f,1.f),

	//left
	glm::vec3(-1.f, 1.f,1.f),
	glm::vec3(-1.f, 1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,-1.f),
	glm::vec3(-1.f, -1.f,1.f),
	glm::vec3(-1.f, 1.f,1.f),

	//right
	glm::vec3(1.f, 1.f,1.f),
	glm::vec3(1.f, 1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,-1.f),
	glm::vec3(1.f, -1.f,1.f),
	glm::vec3(1.f, 1.f,1.f),

	//bottom
	glm::vec3(-1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f, -1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f,  1.f),
	glm::vec3(-1.f, -1.f, -1.f),

	//top
	glm::vec3(-1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f, -1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f,  1.f),
	glm::vec3(-1.f,  1.f, -1.f),
};
static const glm::vec2 CUBE_UV[] = {
	//back
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),

	//front
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	
	//left
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),

	//right
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 1.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(1.f, 0.f),

	//bottom
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f),

	//top
	glm::vec2(0.f, 1.f),
	glm::vec2(1.f, 1.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(1.f, 0.f),
	glm::vec2(0.f, 0.f),
	glm::vec2(0.f, 1.f)
};

void Chunk::setChunkSize(int size) {
	int sizeCubed = size * size * size;

	m_chunkSize = size;
	m_totalVerticesInChunk = sizeCubed * NUM_VERTICES_IN_CUBE;
	m_totalUvsInChunk = NUM_UVS_IN_CUBE * sizeCubed;
}

void Chunk::initVoxelData(glm::vec3& size)
{
	// Init
	m_voxelDataSize = size;
	m_voxels = new Voxel[size.x*size.y*size.z];
	
}

void Chunk::initMeshingData()
{
	// Loop through all 64 states of a meshed cube
	for (int top = 0; top <= 1; top++)
	{
		for (int bottom = 0; bottom <= 1; bottom++)
		{
			for (int left = 0; left <= 1; left++)
			{
				for (int right = 0; right <= 1; right++)
				{
					for (int front = 0; front <= 1; front++)
					{
						for (int back = 0; back <= 1; back++)
						{
							int faceCount = top + bottom + left + right + front + back;
							meshID.push_back(faceCount);
							std::vector<const glm::vec3*> mesh;
							std::vector<const glm::vec2*> meshUV;
							if (top == 1) {
								mesh.push_back(TOP);
								meshUV.push_back(TOP_UV);
							}
							if (bottom == 1) {
								mesh.push_back(BOTTOM);
								meshUV.push_back(BOTTOM_UV);
							}
							if (left == 1) {
								mesh.push_back(LEFT);
								meshUV.push_back(LEFT_UV);
							}
							if (right == 1) {
								mesh.push_back(RIGHT);
								meshUV.push_back(RIGHT_UV);
							}
							if (front == 1) {
								meshUV.push_back(FRONT_UV);
								mesh.push_back(FRONT);
							}
							if (back == 1) {
								mesh.push_back(BACK);
								meshUV.push_back(BACK_UV);
							}
							meshList.push_back(mesh);
							meshUVList.push_back(meshUV);
						}
					}
				}
			}
		}
	}
}

std::vector<glm::vec4> Chunk::findCubesToRender()
{
	//int t = 32;
	//int b = 16;
	//int l = 8;
	//int r = 4;
	//int f = 2;
	//int k = 1;

	std::vector<glm::vec4> voxels;

	int z_max, y_max, x_max;

	z_max = m_voxelDataSize.z;
	y_max = m_voxelDataSize.y;
	x_max = m_voxelDataSize.x;

	for (int z = 0; z <= z_max; z++)
	{
		for (int y = 0; y <= y_max; y++)
		{
			for (int x = 0; x <= x_max; x++)
			{
				int cubeMeshID = 0;

				bool top, bottom, left, right, back, front;

				if (m_voxels[x+(y*x_max)+(z*x_max*y_max)].active)
				{
					//top y+1
					if (y == y_max)
					{
						// top block is not active
						top = false;
					}
					else {
						top = m_voxels[x + ((y+1)*x_max) + (z*x_max*y_max)].active;
					}
					//bottom y-1
					if (y == 0)
					{
						bottom = false;
					}
					else
					{
						bottom = m_voxels[x + ((y-1)*x_max) + (z*x_max*y_max)].active;
					}
					//left x-1
					if (x == 0)
					{
						left = false;
					}
					else
					{
						left = m_voxels[(x-1) + (y*x_max) + (z*x_max*y_max)].active;
					}
					//right x+1
					if (x == x_max)
					{
						// right block is not active
						right = false;
					}
					else {
						right = m_voxels[(x+1) + (y*x_max) + (z*x_max*y_max)].active;
					}
					//front z+1
					if (z == z_max)
					{
						// front block is not active
						front = false;
					}
					else {
						front = m_voxels[x + (y*x_max) + ((z+1)*x_max*y_max)].active;
					}
					//back z-1
					if (z == 0)
					{
						back = false;
					}
					else
					{
						back = m_voxels[x + (y*x_max) + ((z - 1)*x_max*y_max)].active;
					}
					
					if (!top)
					{
						cubeMeshID += t;
					}
					if (!bottom)
					{
						cubeMeshID += b;
					}
					if (!left)
					{
						cubeMeshID += l;
					}
					if (!right)
					{
						cubeMeshID += r;
					}
					if (!front)
					{
						cubeMeshID += f;
					}
					if (!back)
					{
						cubeMeshID += k;
					}
					
					// if the mesh is completely blocked in, don't add it
					if (cubeMeshID != 63)
					{
						voxels.push_back(glm::vec4(x, y, z, cubeMeshID));
					}
				}
			}
		}
	}
	return voxels;
}

void Chunk::fill(int size)
{
	this->setChunkSize(size);
	
	if (m_filled)
		this->destroyOpenGLData();

	m_totalVerticesInChunk = 0;
	m_totalUvsInChunk = 0;

	std::vector<glm::vec4> voxels = this->findCubesToRender();
	for (glm::vec4 voxel : voxels)
	{
		// add the faces of each voxel to the vert and UV counts
		m_totalVerticesInChunk += VERTS_PER_FACE * meshID[voxel.w];
		m_totalVerticesInChunk += UVS_PER_FACE * meshID[voxel.w];
	}

	glm::vec3* chunkVertices = new glm::vec3[m_totalVerticesInChunk];
	glm::vec2* chunkUVs = new glm::vec2[m_totalUvsInChunk];
	
	int insertVertPos = 0;
	int insertUVPos = 0;

	for (glm::vec4 voxel : voxels)
	{
		int cubeVerts = VERTS_PER_FACE * meshID[voxel.w];
		insertVertPos += cubeVerts;
		std::memcpy(chunkVertices + insertVertPos, &meshList[voxel.w], sizeof(meshList[voxel.w]));

		for (int j = insertVertPos; j < insertVertPos + cubeVerts; j++)
		{
			chunkVertices[j].x += voxel.x * 2;
			chunkVertices[j].y += voxel.y * 2;
			chunkVertices[j].z += voxel.z * 2;
		}

		insertUVPos += UVS_PER_FACE * meshID[voxel.w];
		std::memcpy(chunkUVs + insertUVPos, &meshUVList[voxel.w], sizeof(meshUVList[voxel.w]));
	}


	//for (int z = 0; z < size; z++)
	//{
	//	for (int y = 0; y < size; y++)
	//	{
	//		for (int x = 0; x < size; x++)
	//		{
	//
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

	//			int iv = (x * NUM_VERTICES_IN_CUBE) + size * ((y * NUM_VERTICES_IN_CUBE) + size * (z * NUM_VERTICES_IN_CUBE));
	//			std::memcpy(chunkVertices + iv, CUBE_VERTICES, sizeof(CUBE_VERTICES));
	//			
	//			for (int j = iv; j < iv + NUM_VERTICES_IN_CUBE; j++)
	//			{
	//				chunkVertices[j].x += x * 2;
	//				chunkVertices[j].y += y * 2;
	//				chunkVertices[j].z += z * 2;
	//			}

	//			std::memcpy(chunkUVs + iv, CUBE_UV, sizeof(CUBE_UV));
	//		}
	//	}
	//}

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
}

void Chunk::placeBlockAt(int x, int y, int z)
{
	if (x >= m_chunkSize || y >= m_chunkSize || z >= m_chunkSize)
		return;

	if (x < 0 || y < 0 || z < 0)
		return;

	int iv = ((x * NUM_VERTICES_IN_CUBE) + m_chunkSize * ((y * NUM_VERTICES_IN_CUBE) + m_chunkSize * (z * NUM_VERTICES_IN_CUBE))) * sizeof(glm::vec3);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, iv, sizeof(CUBE_VERTICES), CUBE_VERTICES);
}

void Chunk::destroyBlockAt(int x, int y, int z)
{
	if (x >= m_chunkSize || y >= m_chunkSize || z >= m_chunkSize)
		return;

	if (x < 0 || y < 0 || z < 0)
		return;

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

	int iv = ((x * NUM_VERTICES_IN_CUBE) + m_chunkSize * ((y * NUM_VERTICES_IN_CUBE) + m_chunkSize * (z * NUM_VERTICES_IN_CUBE))) * sizeof(glm::vec3);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferSubData(GL_ARRAY_BUFFER, iv, sizeof(EMPTY_BLOCK), EMPTY_BLOCK);
}

void Chunk::draw()
{
	glBindVertexArray(m_vao);
	
	glDrawArrays(GL_TRIANGLES, 0, m_totalVerticesInChunk);
}