// light.cpp: implementation of the mesh class.
//
//////////////////////////////////////////////////////////////////////
#include "light.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
light::light(string lightName):x(0),y(0),z(0),ar(0),ag(0),ab(0),dr(0),dg(0),db(0),sr(0),sg(0),sb(0),aar(0),aag(0),aab(0),size(0)
{
	ifstream myLight;
	myLight.open(lightName);
	if (myLight.is_open())
	{
		string line = "";
		stringstream ss;
		int index = 0;

		while(getline(myLight, line))
		{
			ss.str("");
			ss.clear();

			ss << line;
			string lightType;
			ss >> lightType;

			// light x y z ar ag ab dr dg db sr sg sb: define the light information.
			if (lightType == "light")
			{	
				size ++;
				type.push_back(lightType);
				index ++;
				float tmpNumber;

				ss >> tmpNumber;
				x.push_back(tmpNumber);
				ss >> tmpNumber;
				y.push_back(tmpNumber);
				ss >> tmpNumber;
				z.push_back(tmpNumber);

				ss >> tmpNumber;
				ar.push_back(tmpNumber);
				ss >> tmpNumber;
				ag.push_back(tmpNumber);
				ss >> tmpNumber;
				ab.push_back(tmpNumber);
				
				ss >> tmpNumber;
				dr.push_back(tmpNumber);
				ss >> tmpNumber;
				dg.push_back(tmpNumber);
				ss >> tmpNumber;
				db.push_back(tmpNumber);
				ss >> tmpNumber;
				sr.push_back(tmpNumber);
				ss >> tmpNumber;
				sg.push_back(tmpNumber);
				ss >> tmpNumber;
				sb.push_back(tmpNumber);
			}
			else if(lightType == "ambient")
			{
				size ++;
				type.push_back(lightType);
				index ++;
				float tmpNumber;

				ss >> tmpNumber;
				aar.push_back(tmpNumber);
				ss >> tmpNumber;
				aag.push_back(tmpNumber);
				ss >> tmpNumber;
				aab.push_back(tmpNumber);
			}
		}
		// reverse(aar.begin(), aar.end());
		// reverse(aag.begin(), aag.end());
		// reverse(aab.begin(), aab.end());
	}
	myLight.close();
}

light::~light()
{
}
