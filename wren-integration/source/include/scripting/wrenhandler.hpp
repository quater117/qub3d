#include <iostream>

#include <wren.hpp>

#include "scripting/wrenBindings.hpp"

#pragma once

class WrenHandler
{
public:
	WrenHandler();
	~WrenHandler();

	WrenConfiguration m_vmConfig;
	void initialiseVM();
	void destroyVM();

	void interpretCode( std::string code );

	// Super basic IMPORTANT wren functions
	static void wrenConsoleFunction( WrenVM* vm, const char* message );
	static void wrenErrorFunction( WrenVM* vm, WrenErrorType type, const char* module, int line, const char* message );

	// test code
	static void mathsAdd( WrenVM* vm )
	{
		double a = wrenGetSlotDouble(vm, 1);
		double b = wrenGetSlotDouble(vm, 2);
		double c = wrenGetSlotDouble(vm, 3);
		wrenSetSlotDouble(vm, 0, a + b + c);
	}

	static WrenForeignMethodFn handleBinds( WrenVM* vm, const char* module, const char* className, bool isStatic, const char* signature );

private:
	WrenVM* m_vmHandle;
};
