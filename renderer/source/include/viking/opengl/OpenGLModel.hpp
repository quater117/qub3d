#pragma once

#include <viking/opengl/OpenGL.hpp>
#include <viking/IModel.hpp>

namespace viking
{
    namespace opengl
    {
        class OpenGLModel : public virtual IModel
        {
        public:
			OpenGLModel(unsigned int model_pool_index);
		private:

        };
    }
}
