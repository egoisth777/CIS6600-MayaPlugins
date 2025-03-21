#include "LSystemNode.h"
MTypeId LSystemNode::id(0x99999);

MObject LSystemNode::m_outputMesh;
MObject LSystemNode::m_time;
MObject LSystemNode::m_angle;
MObject LSystemNode::m_step;
MObject LSystemNode::m_syntax;

MStatus LSystemNode::initialize()
{
	MFnNumericAttribute numeric_att;
	LSystemNode::m_angle = numeric_att.create("angle_degree", "ad", MFnNumericData::kDouble, 60.0);
	LSystemNode::m_step = numeric_att.create("step_size", "ss", MFnNumericData::kDouble, 1.0);
	MFnTypedAttribute typed_att;
	LSystemNode::m_syntax = typed_att.create("grammar", "g", MFnData::kString, MFnStringData().create("F++F++F \n" "F -> F-F++F-F"));
	MFnUnitAttribute unit_att;
	LSystemNode::m_time = unit_att.create("time", "t", MFnUnitAttribute::kTime, 0.0);
	LSystemNode::m_outputMesh = typed_att.create("outputMesh", "out", MFnData::kMesh);

	typed_att.setStorable(false);

	MStatus s;
	s = addAttribute(LSystemNode::m_time);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::time " + s.errorString());
	s = addAttribute(LSystemNode::m_outputMesh);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::outputMesh " + MString() + s.errorString());
	s = addAttribute(LSystemNode::m_angle);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::object_degree " + MString() + s.errorString());
	s = addAttribute(LSystemNode::m_step);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::object_step " + MString() + s.errorString());
	s = addAttribute(LSystemNode::m_syntax);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::object_grammar " + MString() + s.errorString());

	s = attributeAffects(LSystemNode::m_time, LSystemNode::m_outputMesh);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::time -> LSystemNode::outputMesh " + MString() + s.errorString());
	s = attributeAffects(LSystemNode::m_angle, LSystemNode::m_outputMesh);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::object_degree -> LSystemNode::outputMesh " + MString() + s.errorString());
	s = attributeAffects(LSystemNode::m_step, LSystemNode::m_outputMesh);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::object_step -> LSystemNode::outputMesh " + MString() + s.errorString());
	s = attributeAffects(LSystemNode::m_syntax, LSystemNode::m_outputMesh);
	if (s != MStatus::kSuccess)
		MGlobal::displayInfo("LSystemNode::object_grammar -> LSystemNode::outputMesh " + MString() + s.errorString());
	return MS::kSuccess;
}

MStatus LSystemNode::compute(const MPlug& plug, MDataBlock& data)
{
	MGlobal::displayInfo("LSystemNode::Compute\n");

	MStatus s;
	if (plug == m_outputMesh)
	{
		MDataHandle timeHandle = data.inputValue(m_time, &s);
		MTime time = timeHandle.asTime();
		MDataHandle degreeHandle = data.inputValue(m_angle, &s);
		double angle = degreeHandle.asDouble();
		MDataHandle stepHandle = data.inputValue(m_step, &s);
		double step = stepHandle.asDouble();
		MDataHandle grammarHandle = data.inputValue(m_syntax, &s);
		MString syntax = grammarHandle.asString();
		MDataHandle meshHandle = data.outputValue(m_outputMesh, &s);
		MFnMeshData meshCreator;

		MObject current = meshCreator.create(&s);

		m_lsys.loadProgramFromString(string(syntax.asChar()));
		m_lsys.setDefaultAngle(angle);

		int frame = (int)time.as(MTime::kFilm);
		m_lsys.setDefaultStep(step * std::pow(0.33333333333333333333333, frame-1));
		frame = frame > 0 ? frame : 1;

		std::vector<LSystem::Branch> branches;
		m_lsys.process(frame, branches);
		MPointArray pts;
		MIntArray faceCnt;
		MIntArray faceConnect;

		for (int i = 0; i < branches.size(); i++) {
			MPoint start_point = MPoint(branches[i].first[0], branches[i].first[1], branches[i].first[2]);
			MPoint end = MPoint(branches[i].second[0], branches[i].second[1], branches[i].second[2]);
			CylinderMesh cm(start_point, end);
			cm.appendToMesh(pts, faceCnt, faceConnect);
		}
		MFnMesh fnmesh;
		fnmesh.create(pts.length(), faceCnt.length(), pts, faceCnt, faceConnect, current);
		string gmr_str = syntax.asChar();
		meshHandle.set(current);
		data.setClean(plug);
	}
	return MS::kSuccess;
}
