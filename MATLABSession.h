#pragma once
#include <cmath>
#include <engine.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <vector>
#include <string>
#include <mat.h>
#include <engine.h>
#include <cmath>
#include <iostream>
#include <boost/filesystem.hpp>


#pragma comment (lib, "libmat.lib")
#pragma comment (lib, "libmx.lib")
#pragma comment (lib, "libmex.lib")
#pragma comment (lib, "libeng.lib")




typedef std::string str;

class MATengine {
private :
	Engine* engPt;
	boost::filesystem::path PATH;

public :
	MATengine(boost::filesystem::path PATH_) : PATH(PATH_) {
		engPt = engOpen("NULL");
	}
	void exe();
};