#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "glew.h"
#include "glut.h"
#include "glaux.h"

#include "mesh.h"
#include "light.h"
#include "scene.h"
#include "view.h"
#include "ShaderLoader.h"

#define		TEX_NUM 3	  //the number of textures you use.
#define		IMG_NUM 3	  //the number of images you use.

std::ifstream testFile;	

std::vector<mesh*> Object;
scene Scene;
light Light;
view View;
GLhandleARB	MyShader;

std::vector<std::string> sceneFile;
std::vector<std::string> lightFile;
std::vector<std::string> viewFile;
std::vector<std::string> objectFile;

std::vector<AUX_RGBImageRec*> img;
std::vector<GLuint> texObject;

float deltaAngleX = 0.0;
float deltaAngleY = 0.0;
int xOrigin = -1;
int yOrigin = -1;
int lastX = -1;
int lastY = -1;
int windowSize[2];
bool mipMap = false;


void lightF();
void displayF();
void reshapeF(GLsizei , GLsizei);
void normalKeyF(unsigned char, int ,int);
void mouseMoveF(int x, int y);
void mouseButtonF(int button, int state, int x, int y);
void LoadTexture(std::vector<AUX_RGBImageRec*> img, int textureIndex,char* filename);
void LoadShaders();

using namespace std;

int main(int argc, char** argv)
{
	// initialize
	Object.clear();

	sceneFile.clear();
	lightFile.clear();
	viewFile.clear();
	objectFile.clear();

	img.clear();
	texObject.clear();

	img.resize(IMG_NUM);
	texObject.resize(TEX_NUM);	

	// scene 1 or scene2
	int testNumber = 0;
	// std::cout << "Test Scene1 or Scene2?\nInput the number." << std::endl;
	// std::cin >> testNumber;
	testNumber = 1;

	// change int to string
	std::stringstream ss;
	ss.str("");
	ss.clear();
	ss << testNumber;
	std::string number = "";
	number = ss.str();
	std::string testScene = "Scene" + number + ".txt";

	// open test case
	testFile.open(testScene);
	if (testFile.is_open())
	{
		// store fileName
		std::string line = "";
		while(std::getline(testFile, line))
		{

			if(line.substr(line.find_last_of(".") + 1) == "scene")
			{
				sceneFile.push_back(line);
			}
			else if (line.substr(line.find_last_of(".") + 1) == "view")
			{
				viewFile.push_back(line);
			}
			else if (line.substr(line.find_last_of(".") + 1) == "light")
			{
				lightFile.push_back(line);
			}			
		}
	}
	testFile.close();
	Scene = scene(sceneFile[0], objectFile);
	Light = light(lightFile[0]);
	View = view(viewFile[0]);

	// file read finish
	for (size_t i = 0; i < objectFile.size(); i ++)
	{
		Object.push_back(new mesh(objectFile[i].c_str()));
	}

	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("OpenGL_Hw3_9931031");

	glewInit();

	LoadShaders();
	// read image
	LoadTexture(img, 0, "normal.bmp");
	LoadTexture(img, 1, "diffuse.bmp");
	LoadTexture(img, 2, "specular.bmp");
	// call back
	// rendering
	glutDisplayFunc(displayF);
	glutReshapeFunc(reshapeF);
	// I/O
	glutKeyboardFunc(normalKeyF);
	glutMouseFunc(mouseButtonF);
	glutMotionFunc(mouseMoveF);

	glutMainLoop();

	system("pause");
	return 0;
}

void lightF()
{

	GLfloat light_specular[4] = {0};
	GLfloat light_diffuse[4] = {0};
	GLfloat light_ambient[4] = {0};
	GLfloat light_position[4] = {0};
	GLfloat light_pure_ambient[4] = {0};

	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);

	int aIndex = 0;
	int lightNumber = GL_LIGHT0;
	for (int i = 0; i < Light.size; i ++)
	{
		if(Light.type[i] == "light")
		{
			light_position[0] = Light.x[i];
			light_position[1] = Light.y[i];
			light_position[2] = Light.z[i];
			// point light is 1
			light_position[3] = 1;

			light_diffuse[0] = Light.dr[i];
			light_diffuse[1] = Light.dg[i];
			light_diffuse[2] = Light.db[i];
			light_diffuse[3] = 1;

			light_specular[0] = Light.sr[i];
			light_specular[1] = Light.sg[i];
			light_specular[2] = Light.sb[i];
			light_specular[3] = 1;

			light_ambient[0] = Light.ar[i];
			light_ambient[1] = Light.ag[i];
			light_ambient[2] = Light.ab[i];
			light_ambient[3] = 1;

			glEnable(lightNumber);
			glLightfv(lightNumber, GL_POSITION, light_position);
			glLightfv(lightNumber, GL_DIFFUSE, light_diffuse);
			glLightfv(lightNumber, GL_SPECULAR, light_specular);
			glLightfv(lightNumber, GL_AMBIENT, light_ambient);
			cout << "lightNumber:" << lightNumber << endl;
			lightNumber ++;
		}
		else if(Light.type[i] == "ambient")
		{
			light_pure_ambient[0] = Light.aar[aIndex];
			light_pure_ambient[1] = Light.aag[aIndex];
			light_pure_ambient[2] = Light.aab[aIndex];
			light_pure_ambient[3] = 1;

			glEnable(lightNumber);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_pure_ambient);
			cout << "lightNumber:" << lightNumber << endl;
			lightNumber ++;
			aIndex ++;
		}

	}


}

void displayF()
{
	std::cout << "display:" << std::endl;
	// clear the buffer
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);      //清除用color
	glClearDepth(1.0f);                        // Depth Buffer (就是z buffer) Setup
	glEnable(GL_DEPTH_TEST);                   // Enables Depth Testing
	glDepthFunc(GL_LEQUAL);                    // The Type Of Depth Test To Do
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//這行把畫面清成黑色並且清除z buffer

	// viewport transformation
	// glViewport(0, 0, windowSize[0], windowSize[1]);
	// glViewport(0, 0, 800, 800);
	glViewport(View.x, View.y, View.w, View.h);

	// projection transformation
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// gluPerspective(60.0, (GLfloat)windowSize[0]/(GLfloat)windowSize[1], 1.0, 1000.0);
	// gluPerspective(45.0, 1.0, 10.0, 5000.0);
	gluPerspective(View.fovy, (GLfloat)View.w / (GLfloat)View.h, View.dnear, View.dfar);

	// viewing and modeling transformation
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	gluLookAt(View.ex, View.ey, View.ez,// eye
		View.vax, View.vay, View.vaz,// center
		View.vux, View.vuy, View.vuz );// up

	// glPushMatrix();
	glTranslatef(View.ex, View.ey, View.ez);

	glRotated(deltaAngleX, View.vux, View.vuy, View.vuz );

	float Vx = View.vax - View.ex;
	float Vy = View.vay - View.ey;
	float Vz = View.vaz - View.ez;
	float Nx = Vy * View.vuz - Vz * View.vuy;
	float Ny = Vz * View.vux - Vx * View.vuz;
	float Nz = Vx * View.vuy - Vy * View.vux;
	glRotated(deltaAngleY, Nx, Ny, Nz );

	glTranslatef(-View.ex, -View.ey, -View.ez);
	// glPopMatrix();

	//注意light位置的設定，要在gluLookAt之後
	lightF();

	glUseProgram(MyShader);

	int lastMaterial = -1;
	for (size_t k = 0; k < Object.size(); k ++)
	{
		// cout << "k:" << k << ":size:" << Object.size();
		// std::cout << "object name: " << objectFile[k] << std::endl;


		glActiveTexture(GL_TEXTURE0);
		// glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texObject[0]);

		// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		// glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glActiveTexture(GL_TEXTURE1);
		// glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texObject[1]);

		// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		// glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		glActiveTexture(GL_TEXTURE2);
		// glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texObject[2]);

		// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
		// glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_MODULATE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

		// assign uniform variable for shader
		GLint location = 0;

		location = glGetUniformLocation(MyShader, "colorTexture0");
		if(location == -1)
			printf("Cant find texture name: colorTexture0\n");
		else
			glUniform1i(location, 0);

		location = glGetUniformLocation(MyShader, "colorTexture1");
		if(location == -1)
			printf("Cant find texture name: colorTexture1\n");
		else
			glUniform1i(location, 1);

		location = glGetUniformLocation(MyShader, "colorTexture2");
		if(location == -1)
			printf("Cant find texture name: colorTexture2\n");
		else
			glUniform1i(location, 2);


		glPushMatrix();

		glTranslatef(Scene.Tx[k], Scene.Ty[k], Scene.Tz[k]);
		glRotatef(Scene.Angle[k], Scene.Rx[k], Scene.Ry[k], Scene.Rz[k]);
		glScalef(Scene.Sx[k], Scene.Sy[k], Scene.Sz[k]);


		for(size_t i = 0; i < Object[k]->fTotal; i ++)
		{
			// set material property if this face used different material
			if(lastMaterial != Object[k]->faceList[i].m)
			{
				lastMaterial = (int)Object[k]->faceList[i].m;
				glMaterialfv(GL_FRONT, GL_AMBIENT  , Object[k]->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE  , Object[k]->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR , Object[k]->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &Object[k]->mList[lastMaterial].Ns);
			}
			else
			{
				glMaterialfv(GL_FRONT, GL_AMBIENT  , Object[k]->mList[lastMaterial].Ka);
				glMaterialfv(GL_FRONT, GL_DIFFUSE  , Object[k]->mList[lastMaterial].Kd);
				glMaterialfv(GL_FRONT, GL_SPECULAR , Object[k]->mList[lastMaterial].Ks);
				glMaterialfv(GL_FRONT, GL_SHININESS, &Object[k]->mList[lastMaterial].Ns);
			}

			glBegin(GL_TRIANGLES);
			for (size_t j = 0; j < 3; j ++)
			{
				glMultiTexCoord3fv(GL_TEXTURE0, Object[k]->tList[Object[k]->faceList[i][j].t].ptr);
				glMultiTexCoord3fv(GL_TEXTURE1, Object[k]->tList[Object[k]->faceList[i][j].t].ptr);
				glMultiTexCoord3fv(GL_TEXTURE2, Object[k]->tList[Object[k]->faceList[i][j].t].ptr);
				glNormal3fv(Object[k]->nList[Object[k]->faceList[i][j].n].ptr);
				glVertex3fv(Object[k]->vList[Object[k]->faceList[i][j].v].ptr);
			}
			glEnd();
		}

		glActiveTexture(GL_TEXTURE2);
		// glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glActiveTexture(GL_TEXTURE1);
		// glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		//unbind texture 0
		glActiveTexture(GL_TEXTURE0);
		// glDisable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		glFlush();
		glPopMatrix();
		// std::cout << "texture name: " << Object[k]->mList[lastMaterial].map_Kd << std::endl;
	}

	glutSwapBuffers();
	// glutPostRedisplay();

}

void reshapeF(GLsizei w, GLsizei h)
{
	std::cout << "reshape: " << std::endl;
	View.w = w;
	View.h = h;
}

void normalKeyF(unsigned char key, int x, int y){

	// std::cout << "key is :" << key << std::endl;

	float Vx = View.vax - View.ex;
	float Vy = View.vay - View.ey;
	float Vz = View.vaz - View.ez;
	float Vl = sqrt(Vx * Vx + Vy * Vy + Vz * Vz);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	if(key == 'w' || key == 'W')		
	{
		// std::cout << "1" << std::endl;

		View.ex += Vx / Vl;
		View.ey += Vy / Vl;
		View.ez += Vz / Vl;

		View.vax += Vx / Vl;
		View.vay += Vy / Vl;
		View.vaz += Vz / Vl;

		glutPostRedisplay();

	}
	else if(key == 'a' || key == 'A')		
	{
		// std::cout << "2" << std::endl;

		float Nx = Vy * View.vuz - Vz * View.vuy;
		float Ny = Vz * View.vux - Vx * View.vuz;
		float Nz = Vx * View.vuy - Vy * View.vux;
		float Nl = sqrt(Nx * Nx + Ny * Ny + Nz * Nz);

		View.ex -= Nx / Nl;
		View.ey -= Ny / Nl;
		View.ez -= Nz / Nl;

		View.vax -= Nx / Nl;
		View.vay -= Ny / Nl;
		View.vaz -= Nz / Nl;

		glutPostRedisplay();
	}
	else if(key == 's' || key == 'S')		
	{
		// std::cout << "3" << std::endl;

		View.ex -= Vx / Vl;
		View.ey -= Vy / Vl;
		View.ez -= Vz / Vl;

		View.vax -= Vx / Vl;
		View.vay -= Vy / Vl;
		View.vaz -= Vz / Vl;

		glutPostRedisplay();
	}
	else if(key == 'd' || key == 'D')		
	{
		// std::cout << "4" << std::endl;

		float Nx = Vy * View.vuz - Vz * View.vuy;
		float Ny = Vz * View.vux - Vx * View.vuz;
		float Nz = Vx * View.vuy - Vy * View.vux;
		float Nl = sqrt(Nx * Nx + Ny * Ny + Nz * Nz);

		View.ex += Nx / Nl;
		View.ey += Ny / Nl;
		View.ez += Nz / Nl;

		View.vax += Nx / Nl;
		View.vay += Ny / Nl;
		View.vaz += Nz / Nl;

		glutPostRedisplay();
	}
	else if(key == 'm' || key == 'M')		
	{

		mipMap = !mipMap;
		// std::cout << "M" << std::endl;
		if(mipMap == true)
		{
			std::cout << "Using mipMap ......" << std::endl;

		}
		else if(mipMap == false)
		{
			std::cout << "Not using mipMap ......" << std::endl;
		}

		glutPostRedisplay();
	}

}


void mouseMoveF(int x, int y) { 	

	// this will only be true when the left button is down
	if (xOrigin >= 0 || yOrigin >= 0) 
	{
		// update deltaAngle
		deltaAngleX += (x - lastX);
		deltaAngleY += (y - lastY);
		lastX = x;
		lastY = y;
	}
}

void mouseButtonF(int button, int state, int x, int y) {

	// only start motion if the left button is pressed
	if (button == GLUT_LEFT_BUTTON) 
	{
		// when the button is released
		if (state == GLUT_UP) {

			deltaAngleX /= glutGet(GLUT_WINDOW_WIDTH);
			deltaAngleY /= glutGet(GLUT_WINDOW_HEIGHT);

			deltaAngleX *= 10;
			deltaAngleY *= 10;

			glutPostRedisplay();
			xOrigin = -1;
			yOrigin = -1;
			lastX = -1;
			lastY = -1;
		}
		else  {// state = GLUT_DOWN
			xOrigin = x;
			yOrigin = y;
			lastX = x;
			lastY = y;
		}
	}
}

void LoadTexture(std::vector<AUX_RGBImageRec*> img, int textureIndex,char* filename)
{
	img[textureIndex] = auxDIBImageLoadA(filename);
	glGenTextures(1, &texObject[textureIndex]);
	glBindTexture(GL_TEXTURE_2D, texObject[textureIndex]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 
		img[textureIndex]->sizeX, img[textureIndex]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, img[textureIndex]->data);
	// mip map
	// glBindTexture(GL_TEXTURE_2D, texObject[textureIndex]);
	// gluBuild2DMipmaps(GL_TEXTURE_2D, 4, 
	// 	img[textureIndex]->sizeX, img[textureIndex]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, img[textureIndex]->data);
}

void LoadShaders()
{
	MyShader = glCreateProgram();
	if(MyShader != 0)
	{
		ShaderLoad(MyShader, "../openGLHw3/vertexShader.vs", GL_VERTEX_SHADER);
		ShaderLoad(MyShader, "../openGLHw3/fragmentShader.fs", GL_FRAGMENT_SHADER);
	}
}
