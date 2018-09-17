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

#include <qub3d/shader_pipeline.hpp>
#include <qub3d/io.hpp>

using namespace qub3d;

void ShaderPipeline::addStage(ShaderPipelineStage stage, const std::string& shaderFilepath)
{
	std::string fileText = File::readAllText(shaderFilepath);
	const char* txt = fileText.c_str();

	GLuint shader = glCreateShader(QTO_GLENUM(stage));
	glShaderSource(shader, 1, &txt, nullptr);
	glCompileShader(shader);

	const int INFO_LOG_SIZE = 512;

	int success;
	char infoLog[INFO_LOG_SIZE];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(shader, INFO_LOG_SIZE, NULL, infoLog);
		printf("Shader compile error! %s\n", infoLog);
	}

	m_shaderIDs.push_back(shader);
}

void ShaderPipeline::destroy()
{
	glDeleteProgram(m_program);
}

void ShaderPipeline::build()
{
	m_program = glCreateProgram();

	for (GLuint shader : m_shaderIDs)
	{
		glAttachShader(m_program, shader);
	}

	glLinkProgram(m_program);
	
	for (GLuint shader : m_shaderIDs)
	{
		glDetachShader(m_program, shader);
		glDeleteShader(shader);
	}
	m_shaderIDs.clear();
}

void ShaderPipeline::bind()
{
	glUseProgram(m_program);
}

void ShaderPipeline::unbind()
{
	glUseProgram(0);
}