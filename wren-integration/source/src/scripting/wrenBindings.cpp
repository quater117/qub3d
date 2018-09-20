#include "scripting/wrenBindings.hpp"

WrenBindings::WrenBindings() {};
WrenBindings::~WrenBindings() {};

void WrenBindings::initialiseBasicBindings() {};

void WrenBindings::addBinding( wrenBindingData bindData, void* callback )
{
	m_wrenBindings[bindData] = callback; // Set the callback function for the specific bindInfo in the hashmap
}

void* WrenBindings::returnBinding( wrenBindingData bindData )
{
	return m_wrenBindings[bindData]; // Look for the binded function (void*) in the hashmap, using bindInfo as the key
}
