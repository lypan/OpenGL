// view.h: interface for the view class.
//
//////////////////////////////////////////////////////////////////////

#ifndef VIEW_H_
#define VIEW_H_

#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class view
{
	public:
		string objName;
		float ex;
		float ey;
		float ez;
		float vax;
		float vay;
		float vaz;
		float vux;
		float vuy;
		float vuz;
		float fovy;
		float dnear;
		float dfar;
		float x;
		float y;
		float w;
		float h;
		// ctor
		// default
		view(string sceneName = "");
		// dtor
		~view();
};

#endif

