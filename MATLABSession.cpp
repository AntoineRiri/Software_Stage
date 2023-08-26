#include "MATLABSession.h"
#include <engine.h>
#include <string>
#include <mat.h>
#include <cmath>
#include <iostream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>


void MATengine::exe() {
	boost::filesystem::path MATPath = "Plots";
	MATPath = PATH / MATPath;
	boost::filesystem::path MATPath1 = "MATLABPlots";
	MATPath = MATPath / MATPath1;
	str MATscript = MATPath.string();
	str RUN = "run(\'";
	RUN += MATscript;
	RUN += "/CSVPlot.m\');";
	std::replace(RUN.begin(), RUN.end(), '\\', '/');
	const char* mess = &(RUN[0]);
	engEvalString(engPt, mess);
}