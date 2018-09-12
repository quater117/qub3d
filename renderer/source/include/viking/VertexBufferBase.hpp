#pragma once

#include <viking/VertexBinding.hpp>

#include <vector>

namespace viking
{
	struct VertexBufferBase
	{
	public:
		std::vector<VertexBinding> vertex_bindings;
		unsigned int size;
	};
}