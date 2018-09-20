#include "scripting/wrenhandler.hpp"

int main(int argc, char *argv[])
{
	WrenHandler* wrenHandler = new WrenHandler();

	wrenHandler->m_vmConfig.errorFn = wrenHandler->wrenErrorFunction;
	wrenHandler->m_vmConfig.writeFn = wrenHandler->wrenConsoleFunction;
	wrenHandler->m_vmConfig.bindForeignMethodFn = wrenHandler->handleBinds;

	wrenHandler->initialiseVM();

	wrenHandler->interpretCode("class Maths { \n foreign static add(a,b,c) \n } \n System.print( Maths.add(1,2,3) )");

	return 0;
}
