#pragma once

#include <maya/MStringArray.h>
#include <maya/MTime.h>
#include <maya/MFnMesh.h>
#include <maya/MPoint.h>
#include <maya/MFloatPoint.h>
#include <maya/MFloatPointArray.h>
#include <maya/MIntArray.h>
#include <maya/MDoubleArray.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnTypedAttribute.h>
#include <maya/MFnNumericAttribute.h>
#include <maya/MPxNode.h>
#include <maya/MObject.h>
#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MFnMeshData.h>
#include <maya/MIOStream.h>
#include <maya/MPxCommand.h>
#include <maya/msyntax.h>
#include <maya/MArgParser.h>
#include <maya/MFnStringData.h>
#include <maya/MGlobal.h>

#include "cylinder.h"
#include "LSystem.h"


class LSystemNode : public MPxNode
{
public:
    LSystemNode() = default;
    virtual ~LSystemNode() = default;
    static void* creator() { return new LSystemNode; };
    static MStatus initialize();
    virtual MStatus compute(const MPlug& plug, MDataBlock& data);

    LSystem m_lsys;
    static MObject m_syntax;
    static MObject m_angle;
    static MObject m_step;
    static MObject m_outputMesh;
    static MTypeId id;
    static MObject m_time;
};

