#pragma once

#ifndef HELLO_MAYA_H
#define HELLO_MAYA_H

#include <Maya/MArgList.h>
#include <maya/MObject.h>
#include <maya/MGlobal.h>
#include <maya/MPxCommand.h>


class helloMaya : public MPxCommand
{
public:
	helloMaya(){};
	virtual MStatus doIt(const MArgList& args);
	static void* creator();
};

#endif
