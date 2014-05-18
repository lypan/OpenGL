// view.cpp: implementation of the mesh class.
//
//////////////////////////////////////////////////////////////////////
#include "view.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
view::view(string viewName):ex(0),ey(0),ez(0),vax(0),vay(0),vaz(0),vux(0),vuy(0),vuz(0),fovy(0),dnear(0),dfar(0),x(0),y(0),w(0),h(0)
{
	ifstream myView;
	myView.open(viewName);
	if (myView.is_open())
	{
		string line = "";
		stringstream ss;

		while(getline(myView, line))
		{
			ss.str("");
			ss.clear();

			ss << line;
			string tmpString;

			ss >> tmpString;

			if(tmpString == "eye")
			{
				ss >> ex;
				ss >> ey;
				ss >> ez;
				// cout << "eye:" << ex << ":" << ey << ":" << ez << endl; 				
			}
			else if(tmpString == "vat")
			{
				ss >> vax;
				ss >> vay;
				ss >> vaz;
				// cout << "vat:" << vax << ":" << vay << ":" <<  vaz << endl; 				
			}
			else if(tmpString == "vup")
			{
				ss >> vux;
				ss >> vuy;
				ss >> vuz;
				// cout << "vup:" << vux << ":" << vuy << ":" << vuz << endl; 				

			}
			else if(tmpString == "fovy")
			{
				ss >> fovy;
				// cout << "fovy:" << fovy << endl;
			}	
			else if(tmpString == "dnear")
			{
				ss >> dnear;
				// cout << "dnear:" << dnear << endl;
			}
			else if(tmpString == "dfar")
			{
				ss >> dfar;
				// cout << "dfar:" << dfar << endl;
			}		
			else if(tmpString == "viewport")
			{
				ss >> x;
				ss >> y;
				ss >> w;
				ss >> h;
				// cout << "viewport:" << x << ":" << y << ":" << w << ":" << h << endl;
			}

		}
	}
	myView.close();
}

view::~view()
{
}
