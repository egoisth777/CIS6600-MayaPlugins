import sys

# Imports to use the Maya Python API
import maya.OpenMaya as OpenMaya
import maya.OpenMayaMPx as OpenMayaMPx

# Import the Python wrappers for MEL commands
import maya.cmds as cmds

# The name of the command. 
kPluginCmdName = "pyHelloMaya"

class helloMayaCommand(OpenMayaMPx.MPxCommand):
    def __init__(self):
        OpenMayaMPx.MPxCommand.__init__(self)
    
    def doIt(self, argList):
        # TODO fill in this code to implement the command.
        print("Hello Maya!") # Print "Hello Maya!"
        
        
        # Set the default name and id
        name = "Default Name"
        id_num = "Default ID"
        
        index = argList.flagIndex("n", "name")
        print(f"index: {index} \n")
        if(index != OpenMaya.MArgList.kInvalidArgIndex):
            name = argList.asString(index + 1)
        index = argList.flagIndex("id")
        if(index != OpenMaya.MArgList.kInvalidArgIndex):
            id_num = argList.asString(index + 1)
        print(f"index: {index} \n")

        # Create the dialog command string
        message = (f"Name: {name}\n ID: {id_num}")
        
        # You need to type maya.cmds.pyHelloMaya() to run the command
        # Execute the dialog command
        try:
            cmds.confirmDialog(title = "Hello Maya Python!", message = message, button = ['OK'])
        except:
            sys.stderr.write("Failed to execute the dialog command")

        self.setResult("The Custom command is Executed")

# Create an instance of the command.
def cmdCreator():
    return OpenMayaMPx.asMPxPtr(helloMayaCommand())

# Initialize the plugin
def initializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject, "cg@penn", "1.0", "2012")
    try:
        mplugin.registerCommand(kPluginCmdName, cmdCreator)
    except:
        sys.stderr.write("Failed to register command: %s\n" % kPluginCmdName)
        raise

# Uninitialize the plugin
def uninitializePlugin(mobject):
    mplugin = OpenMayaMPx.MFnPlugin(mobject)
    try:
        mplugin.deregisterCommand(kPluginCmdName)
    except:
        sys.stderr.write("Failed to unregister command: %s\n" % kPluginCmdName)
        raise
