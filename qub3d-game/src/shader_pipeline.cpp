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

#include <qub3d/imgui.hpp>

#include <glm/gtc/type_ptr.hpp>

using namespace qub3d;

GLint ShaderPipeline::getUniformLocation(const std::string& uniformName)
{
	auto it = m_uniformLocationMap.find(uniformName);
	
	// We've already cached this uniform location
	if (it != m_uniformLocationMap.end())
	{
		return (*it).second;
	}
	else
	{
		GLint location = glGetUniformLocation(m_program, uniformName.c_str());
		if (location >= 0)
		{
			m_uniformLocationMap[uniformName] = location;
			return location;
		}
		else
		{
			printf("Cannot find uniform: %s", uniformName.c_str());
			return location;
		}
	}
}

void ShaderPipeline::setUniform(const std::string& uniformName, glm::mat4& matrix)
{
	glUniformMatrix4fv(this->getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(matrix));
	updateUniformValue(uniformName, matrix);
}

void ShaderPipeline::setUniform(const std::string& uniformName, float& value)
{
	glUniform1f(this->getUniformLocation(uniformName), value);
	updateUniformValue(uniformName, value);
}

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

void ShaderPipeline::debugGUI()
{
	ImGui::Begin("Shader pipeline debugging tool");
	if (ImGui::CollapsingHeader("Uniform Editor")) 
	{
		int count;
		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &count);

		char name[512];
		GLint length, size;
		GLenum type;
		for (int i = 0; i < count; i++)
		{
			glGetActiveUniform(m_program, (GLuint)i, 512, &length, &size, &type, name);
			
			switch (type)
			{
			case GL_FLOAT_MAT4:
				if (ImGui::InputMatrix4x4(name, getUniformValuePtr<glm::mat4>(name)))
				{
					setUniform(name, *getUniformValuePtr<glm::mat4>(name));
				}
				break;
			case GL_FLOAT:
				if (ImGui::InputFloat(name, getUniformValuePtr<float>(name)))
				{
					setUniform(name, *getUniformValuePtr<float>(name));
				}
				break;
			}
			ImGui::NewLine();
		}
	}

	ImGui::End();
}