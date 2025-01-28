#include "hello_maya.h"
#include <maya/MFnPlugin.h>

// define EXPORT for exporting dll functions
#define EXPORT _declspec(dllexport)


#define DISP_NAME "Aeon Li, CIS6600, Spring 2025"
#define DISP_ID "47700221"

// Maya Plugin creator function
void* helloMaya::creator()
{
	return new helloMaya;
}

// Plugin DoIt function
MStatus helloMaya::doIt(const MArgList& argList)
{
	MStatus status;
	MGlobal::displayInfo("Hello Maya Debug");

	// Display name and E-Mail
	MString name = DISP_NAME;
	MString id = DISP_ID;


	unsigned int index;

	index = argList.flagIndex("n", "name");
	if (index != MArgList::kInvalidArgIndex)
	{
		argList.get(index + 1, name);
	}

	index = argList.flagIndex("id");
	if (index != MArgList::kInvalidArgIndex)
	{
		argList.get(index + 1, id);
	}

	MString confDialog("confirmDialog -t \"Hello Maya\"");
	MString message("Name: " + name + "\\r\\n" + "ID: " + id);
	MGlobal::executeCommand(confDialog + " -m " + "\"" + message + "\"");
	return status;
}

// Initialize Maya Plugin upon loading
EXPORT MStatus initializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj, "CIS660", "1.0", "Any");
	status = plugin.registerCommand("helloMaya", helloMaya::creator);
	if (!status)
		status.perror("registerCommand failed");
	return status;
}

//Cleanup Maya Plugin upon unloading
EXPORT MStatus uninitializePlugin(MObject obj)
{
	MStatus status;
	MFnPlugin plugin(obj);
	status = plugin.deregisterCommand("helloMaya");
	if (!status)
		status.perror("deregisterCommand Failed!");
	return status;
}

