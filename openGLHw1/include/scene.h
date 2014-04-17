// scene.h: interface for the scene class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SCENE_H_
#define SCENE_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class scene
{
	public:
		string objName;
		vector<float> Sx;
		vector<float> Sy;
		vector<float> Sz;
		vector<float> Angle;
		vector<float> Rx;
		vector<float> Ry;
		vector<float> Rz;
		vector<float> Tx;
		vector<float> Ty;
		vector<float> Tz;
		int size;
		// ctor
		// default
		scene(string sceneName = "", vector<string>& object = vector<string>());
		// dtor
		~scene();
};

#endif
