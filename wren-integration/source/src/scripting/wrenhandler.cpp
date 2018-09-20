#include "scripting/wrenhandler.hpp"

WrenHandler::WrenHandler()
{
	wrenInitConfiguration( &m_vmConfig );
}
WrenHandler::~WrenHandler()
{

}

void WrenHandler::wrenConsoleFunction(WrenVM* vm, const char* message)
{
	std::cout << message << std::endl;
}

void WrenHandler::wrenErrorFunction(WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message)
{
	std::cout << "Error Type" << type << std::endl;
	std::cout << "Module Throwing Error: " << module << std::endl;
	std::cout << "Line Throwing Error: " << line << std::endl;
	std::cout << "Error Message: " << message << std::endl;
}

WrenForeignMethodFn WrenHandler::handleBinds( WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature )
{
	WrenBindings::wrenBindingData bindData = { vm, module, className, isStatic, signature };

	void* bindReturn = (void*)mathsAdd;
	if (bindReturn != nullptr) // Make sure the function isn't a null pointer, basically checks whether function binding exists
	{
		return reinterpret_cast<void(*)(WrenVM*)>( bindReturn ); // Return the function as a void pointer with a parameter of WrenVM*
	}

	return nullptr; // not sure if this will crash stuff but whatever. ¯\_(ツ)_/¯
}

void WrenHandler::interpretCode(std::string code)
{
	wrenInterpret( m_vmHandle, "main", code.c_str() );
}

void WrenHandler::initialiseVM()
{
//	// Just adding a basic binding in a scope so the stupid bindData variable gets deleted for now.
//	{
//		WrenBindings::wrenBindingData bindData = {m_vmHandle, "main", "maths", true, "add(_,_,_)"};
//		m_binderInstance->addBinding(bindData, (void*)(mathsAdd) );
//	}

	m_vmHandle = wrenNewVM( &m_vmConfig );

}

