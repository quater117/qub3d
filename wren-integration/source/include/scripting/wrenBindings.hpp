#include <iostream>
#include <map>

#include <wren.hpp>

#pragma once

class WrenBindings
{
public:
	struct wrenBindingData
	{
		WrenVM* vm;
		const char* module;
		const char* className;
		bool isStatic;
		const char* signature;

		bool operator<(const wrenBindingData other) const
		{
			return	vm									< other.vm									&&
					std::hash<std::string>{}(module)	< std::hash<std::string>{}(other.module)	&&
					std::hash<std::string>{}(className) < std::hash<std::string>{}(other.className) &&
					isStatic							< other.isStatic							&&
					std::hash<std::string>{}(signature) < std::hash<std::string>{}(other.signature);
		}
	};

	WrenBindings();
	~WrenBindings();

	void initialiseBasicBindings();

	void addBinding( wrenBindingData bindInfo, void* callback );

	void* returnBinding( wrenBindingData );

private:
	std::map<wrenBindingData, void*> m_wrenBindings;
};
