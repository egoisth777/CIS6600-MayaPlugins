#include <maya/MPxCommand.h>
#include <maya/MFnPlugin.h>
#include <maya/MIOStream.h>
#include <maya/MString.h>
#include <maya/MArgList.h>
#include <maya/MGlobal.h>
#include <maya/MSimple.h>
#include <maya/MDoubleArray.h>
#include <maya/MPoint.h>
#include <maya/MPointArray.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MDGModifier.h>
#include <maya/MPlugArray.h>
#include <maya/MVector.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MStringArray.h>
#include <list>

#include "LSystemCmd.h"
#include "LSystemNode.h"

MStatus initializePlugin( MObject obj )
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj, "MyPlugin", "1.0", "Any");

    // Register Command
    status = plugin.registerCommand( "LSystemCmd", LSystemCmd::creator );
    if (!status) {
        status.perror("registerCommand: " + status.errorString());
        return status;
    }

    // Register the LSystemNode command

    status = plugin.registerNode("LSystemNode", LSystemNode::id, LSystemNode::creator, LSystemNode::initialize);
    if (!status) // check if LSystemNode Command registering is sucessful or not
    {
        status.perror("registerNode: " + status.errorString());
        return status;
    }

    // Auto register the MEL Interface (simply execute the .mel file)
    status = MGlobal::executeCommand("source \"" + plugin.loadPath() + "/plugin_dialog.mel\"");
    if (!status)
    {
        status.perror("Execute Command loading interface: " + status.errorString());
        return status;
    }

    return status;
}

MStatus uninitializePlugin( MObject obj)
{
    MStatus   status = MStatus::kSuccess;
    MFnPlugin plugin( obj );

    // Registering the Commands
    // Register the "LSystemCmd" Command
    status = plugin.deregisterCommand( "LSystemCmd" );
    if (!status) {
	    status.perror("deregisterCommand" + status.errorString());
	    return status;
    }

    // Deregister the LSystemNode MPxNode after unloading
    status = plugin.deregisterNode( LSystemNode::id);
    if (!status) {
	    status.perror("deregister Node: " + status.errorString());
	    return status;
    }

    // @TODO Deregister the UI Element when the Plugin is Unloaded
    status= MGlobal::executeCommand(
        "if (`menu -exists LSystemMenu`) deleteUI LSystemMenu;"
    );
    if (!status)
    {
        status.perror("Failed to Remove LSystem Menu!");
    }

    // Return whatever status
    return status;
}


