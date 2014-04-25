// light.h: interface for the light class.
//
//////////////////////////////////////////////////////////////////////
#ifndef LIGHT_H_
#define LIGHT_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class light
{
	public:
		// light
		vector<float> x;
		vector<float> y;
		vector<float> z;
		vector<float> ar;
		vector<float> ag;
		vector<float> ab;
		vector<float> dr;
		vector<float> dg;
		vector<float> db;
		vector<float> sr;
		vector<float> sg;
		vector<float> sb;

		// ambient r g b
		vector<float> aar;
		vector<float> aag;
		vector<float> aab;		
		int size;
		vector<string> type;
		// ctor
		light(string lightName = "");
		// dtor
		~light();
};

#endif
