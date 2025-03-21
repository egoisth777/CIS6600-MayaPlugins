#include "LSystemCmd.h"

#include <maya/MSyntax.h>
#include <maya/MGlobal.h>
#include <maya/MArgDatabase.h>
#include <list>
#include <string>
LSystemCmd::LSystemCmd() : MPxCommand()
{
}

LSystemCmd::~LSystemCmd()
{
}

MStatus LSystemCmd::doIt( const MArgList& args )
{
	// message in Maya output window
    cout<<"Implement Me!"<<endl;
	std::cout.flush();

	// message in scriptor editor
	MGlobal::displayInfo("Implement Me!");
	MStatus s;
	auto getSyntax = []()->MSyntax {
		MSyntax syntax;
		syntax.addFlag("-ss", "-step_size", MSyntax::kDouble);
		syntax.addFlag("-a", "-angle", MSyntax::kDouble);
		syntax.addFlag("-g", "-grammar", MSyntax::kString);
		syntax.addFlag("-i", "-iterations", MSyntax::kLong);
		return syntax;
	};
	double step_size,angle;
	MString grammar;
	int iter;
	MArgDatabase argdb(getSyntax(), args, &s);
	if (argdb.isFlagSet("-i"))
	{
		argdb.getFlagArgument("-i", 0, iter);
	}
	if (argdb.isFlagSet("-g"))
	{
		argdb.getFlagArgument("-g", 0, grammar);
	}
	if (argdb.isFlagSet("-a"))
	{
		argdb.getFlagArgument("-a", 0, angle);
	}
	if (argdb.isFlagSet("-ss"))
	{
		argdb.getFlagArgument("-ss", 0, step_size);
	}

	m_lsys.loadProgramFromString(grammar.asChar());
	m_lsys.setDefaultAngle(angle);
	m_lsys.setDefaultStep(step_size);
	std::vector<std::pair<vec3, vec3>> branches;
	m_lsys.process(iter, branches);

	for (int i = 0; i < branches.size(); i++)
	{
		MString idx_str = to_string(i + 1).c_str();
		MString x0 = to_string(branches.at(i).first[0]).c_str();
		MString y0 = to_string(branches.at(i).first[1]).c_str();
		MString z0 = to_string(branches.at(i).first[2]).c_str();
		MString x1 = to_string(branches.at(i).second[0]).c_str();
		MString y1 = to_string(branches.at(i).second[1]).c_str();
		MString z1 = to_string(branches.at(i).second[2]).c_str();
		vec3 n = (branches.at(i).second - branches.at(i).first).Normalize();
		MString nx = to_string(n[0]).c_str();
		MString ny = to_string(n[1]).c_str();
		MString nz = to_string(n[2]).c_str();

		MString displacement = to_string(branches.at(i).first[1] - step_size).c_str();

		MString command = "curve -d 1 -p " + x0 + " " + y0 + " " + z0 + "-p " + x1 + " " + y1 + " " + z1 + " -k 0 -k 1 -name \" curve" + idx_str + "\";"
			"circle -radius 0.1 -nr " + nx + " " + ny + " " + nz + "-c " + x0 + " " + y0 + " " + z0 + " -name \"nurbsCircle" + idx_str + "\";"
			"select -r nurbsCircle" + idx_str + " curve" + idx_str + " ;"
			"extrude -ch true -rn false -po 1 -et 2 -rotation 0 -scale 1 -rsp 1 \"nurbsCircle " + idx_str + "\" \"curve" + idx_str + "\"";
		MGlobal::executeCommand(command);
	}
    return MStatus::kSuccess;
}

