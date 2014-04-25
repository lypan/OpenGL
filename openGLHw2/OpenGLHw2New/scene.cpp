// scene.cpp: implementation of the mesh class.
//
//////////////////////////////////////////////////////////////////////
#include "scene.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// scene::scene(){}
scene::scene(string sceneName, vector<string>& object):Sx(0),Sy(0),Sz(0),Angle(0),Rx(0),Ry(0),Rz(0),Tx(0),Ty(0),Tz(0),size(0)
{
	ifstream myScene;
	myScene.open(sceneName);
	if (myScene.is_open())
	{
		string line = "";
		stringstream ss;

		while(getline(myScene, line))
		{
			size ++;
			ss.str("");
			ss.clear();

			ss << line;
			string tmpString = "model";
			int tmpNumer;
			ss >> tmpString;
			ss >> objName;
			// cout << objName << endl;
			object.push_back(objName);
			float fTemp;
			ss >> fTemp;
			Sx.push_back(fTemp);
			ss >> fTemp;
			Sy.push_back(fTemp);
			ss >> fTemp; 
			Sz.push_back(fTemp);
			ss >> fTemp;
			Angle.push_back(fTemp);
			ss >> fTemp; 
			Rx.push_back(fTemp);
			ss >> fTemp; 
			Ry.push_back(fTemp);
			ss >> fTemp;
			Rz.push_back(fTemp);
			ss >> fTemp; 
			Tx.push_back(fTemp);
			ss >> fTemp;
			Ty.push_back(fTemp);
			ss >> fTemp; 
			Tz.push_back(fTemp);
		}
	}
	myScene.close();
}

scene::~scene()
{
}
