//============================================================================
// Name        : Usama Sarwar
// Roll no      : i160188
// Game        : cookie-crush.cpp
// Author      : Sibt ul Hussain
// Version     :
// Copyright   : (c) Reserved
// Description : Basic 2D game of Cookie  Crush...
//============================================================================

#ifndef COOKIE_CRUSH_CPP
#define COOKIE_CRUSH_CPP
//#include <GL/gl.h>
//#include <GL/glut.h>
#include <iostream>
#include<string>
#include<cmath>
#include<fstream>
#include "util.h"
using namespace std;
#define MAX(A,B) ((A) > (B) ? (A):(B)) // defining single line functions....
#define MIN(A,B) ((A) < (B) ? (A):(B))
#define ABS(A) ((A) < (0) ? -(A):(A))
#define FPS 10

#define KEY_ESC 27 // A

// 20,30,30
const int bradius = 30; // Cookie radius in pixels...

int width = 930, height = 660; // i have set my window size to be 800 x 600
int dball = 0; // difference between cookies center of 0 pixels
int byoffset = bradius + dball; // 2 * bradius + dball; // board yoffset

float score = 0;         //Our score
int setcolor, psetcolor;

int display_counter=0;   //counts the number of display
bool play=false;         // Start page of game
int cw=0,ch=0;           // deals with cwidth and cheight
bool multiplier;         // to add multiplier to the score
int moves=0, moveCount=0; //deals with moves
int Level=1;             // deals with level of game
int TotalMove=5;         // total moves for different levels
int TotalScore=50;       // total score for different levels
string gameStatus="Lose"; // Status of game (Win/Lose/GameOver)
int increment=0;          //deals with next level.. increment the score for next levels
//===========================CHECK FOR MATCHES==========================

void checkMatchesX(int arr[9][14]){
	                                       //check the pair of 3 or more same cookies in the X direction and deals with them
	for (int i=0;i<9;i++){
		for (int j=0;j<14;j++){
			int checkX=j+1;
			int matchesX=0;
			while ((arr[i][j]==arr[i][checkX]) && checkX<14){
					matchesX++;
					if (matchesX>=2){
							ch=0;
                           	cw=0;
						int tempj=j;
						int tempArr=arr[i][j];
						while (tempArr==arr[i][tempj] && tempj<14){
							arr[i][tempj]=rand()%6;
							arr[i][tempj]+=10;        // For animation
							tempj++;}
						    }
				checkX+=1;}
			}
		}
return;}

void checkMatchesY(int arr[9][14]){
                                           //check the pair of 3 or more same cookies in the Y direction and deals with them
		for (int j=0;j<14;j++){
		for (int i=0;i<9;i++){
			int checkY=i+1;
			int matchesY=0;
			while ((arr[i][j]==arr[checkY][j]) && checkY<9){
					matchesY++;
					if (matchesY>=2){
							ch=0;
	                        cw=0;
						int tempi=i;
						int tempArr=arr[i][j];
						while (tempArr==arr[tempi][j] && checkY<9){
							arr[tempi][j]=rand()%6;
							arr[tempi][j]+=10;       //For animation.. to differentiate group of same cookies from other cookies I've added 10 to the same cookies
							tempi++;}
						    }
				checkY+=1;}
			}
		}
	return;}

//========================ARRAY FUNCTION===============================
int arr[9][14];
                                                 //Crushes the pair of 3 or more cookies at the start of the game
void arrray(){
if (display_counter>0){
	checkMatchesX(arr);
    checkMatchesY(arr);
	return;}

	for (int i=0;i<9;i+=1){
		for (int j=0;j<14;j+=1){
			int r=rand()%6;
			int tempj=j-1;
			int tempi=i-1;
			int counterj=0;
			int counteri=0;
			int k=0;
			for (; (k<=2 );tempj-=1,tempi-=1,k+=1){
				if (arr[i][tempj]==r){
					counterj+=1;}
				if (arr[tempi][j]==r){
					counteri+=1;}
                }
			if (counterj==2 || counteri==2){
				while (arr[i][j-1]==r || arr[i-1][j]==r){
					r=rand()%6;
                    }
				}
			arr[i][j]=r;
		}
	}
	display_counter+=1;
}
// =====================ARRAY FUNCTION END=============================

// =====================SWAP FUNCTION==================================
void swap(int &x,int &y){
	int s;
	s=x;
	x=y;
	y=s;
}
// ======================SWAP FUNCTION END=============================

enum GameState { // Use to check different states of game...
	Ready, Shot, Over, RemoveCluster
};
GameState gamestate = Ready;

const int ncookies = 6;
enum Cookies {
	CK_BISCUIT, CK_BURGER, CK_CROISSONT, CK_CUPCAKE, CK_DONUT, CK_STAR
};
GLuint texture[ncookies];
GLuint tid[ncookies];
string tnames[] = { "biscuit.png", "burger.png", "croissont.png", "cupcake.png",
		"donut.png", "star.png" };
GLuint mtid[ncookies];
int cwidth = 60, cheight = 60; // 60x60 pixels cookies width & height...

void RegisterTextures()
/*Function is used to load the textures from the files and display*/
// Please do not change it...
{
	// allocate a texture name
	glGenTextures(ncookies, tid);

	vector<unsigned char> data;
	ifstream ifile("image-data.bin", ios::binary | ios::in);
	if (!ifile) {
		cout << " Couldn't Read the Image Data file ";
		exit(-1);
	}
	// now load each cookies data...
	int length;
	ifile.read((char*) &length, sizeof(int));
	data.resize(length, 0);
	for (int i = 0; i < ncookies; ++i) {

		// Read current cookie

		ifile.read((char*) &data[0], sizeof(char) * length);

		mtid[i] = tid[i];
		// select our current texture
		glBindTexture( GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps( GL_TEXTURE_2D, 3, cwidth, cheight, GL_RGB,
		GL_UNSIGNED_BYTE, &data[0]);
	}
	ifile.close();
}
void DrawCookie(const Cookies &cname, int sx, int sy, int cwidth = 60,
		int cheight = 60)
		/*Draws a specfic cookie at given position coordinate
		 * sx = position of x-axis from left-bottom
		 * sy = position of y-axis from left-bottom
		 * cwidth= width of displayed cookie in pixels
		 * cheight= height of displayed cookiei pixels.
		 * */
		{
	float fwidth = (float) cwidth / width * 2, fheight = (float) cheight
			/ height * 2;
	float fx = (float) sx / width * 2 - 1, fy = (float) sy / height * 2 - 1;

	glPushMatrix();
	glEnable( GL_TEXTURE_2D);
	glBindTexture( GL_TEXTURE_2D, mtid[cname]);
//	glTranslatef(0, 0, 0);
//	glRotatef(-M_PI / 2, 0, 0, 1);
	glBegin( GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}

float Distance(float x1, float y1, float x2, float y2) {
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}
void Pixels2Cell(int px, int py, int & cx, int &cy) {
}
void Cell2Pixels(int cx, int cy, int & px, int &py)
// converts the cell coordinates to pixel coordinates...
		{

}

void DisplayFunction() {
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim red and 1 means pure red and so on.
//#if 0
	glClearColor(1/*Red Component*/, 1.0/*Green Component*/,
			1.0/*Blue Component*/, 0 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
	glClear(GL_COLOR_BUFFER_BIT); //Update the colors

// #----------------- Write your code here ----------------------------#
	//write your drawing commands here or call your drawing functions...

	// draw cookies..;

if (play==false){
	                                               //Start page of game. Play=false means when player does'nt start the game
		DrawString(20, height - 20, width, height,
			"Cookie-Crush Version 1 ", colors[RED]);
			DrawString(20, height - 60, width, height,
			"Click anywhere to start the game... ", colors[RED]);
			DrawString(20, height - 120, width, height,
			"Instructions: ", colors[RED]);
					DrawString(20, height - 160, width, height,
			"There are different levels of the game. You've to complete the level ",colors[RED]);
			DrawString(20, height - 200, width, height,
			 "in the given range of moves.",colors[RED]);
			 DrawString(20, height - 240, width, height,
			  "When the scores are blinking the multiplier is set on.",colors[RED]);
			     DrawString(200, height - 500, width, height,
			     "PLAY FAST TO SCORE MORE.", colors[RED]);
			     }
    else{                                 //When the game starts
arrray();
Cookies random_cookie[] = {	CK_BISCUIT, CK_BURGER, CK_CROISSONT, CK_CUPCAKE, CK_DONUT, CK_STAR};
for (int y=height-120,i=0 ; y>=60 ;y-=60,i+=1){
	for (int x=60,j=0 ;x<width-60; x+=60,j+=1){
                                                           //cw and ch variables are used for different purposes
			if (arr[i][j]<10) DrawCookie(random_cookie[arr[i][j]], x, y, cwidth, cheight);
			else {
				              // For animation
				DrawCookie(random_cookie[(arr[i][j])-10], x, y, cw, ch);  //For animation
	            if (cw==0){
					score++;
					if (moveCount==0){
						moves=moves+1;}
					moveCount++;
					}
	            if (cw==cwidth){
					arr[i][j]-=10;
					moveCount=0;
					multiplier=false;
					}
				else if (cw<cwidth){multiplier=true;}
				}
    }
}
if (cw<=cwidth){

	cw++;                  //Enlarges the cookies
	ch++;
	}
if (multiplier==false){
	DrawString(width / 2 - 20, height - 20, width, height,
			"Score " + Num2Str(score), colors[BLUE_VIOLET]);}
else if (multiplier==true){
	if (cw%3!=0){          //This line blinks the score when multiplier is set on
		DrawString(width / 2 - 20, height - 20, width, height,
			"Score " + Num2Str(score), colors[RED]);}
	}
		DrawString(width / 2 - 150, height - 20, width, height,
			"Moves " + Num2Str(moves), colors[BLUE_VIOLET]);
if (gameStatus=="Lose"){
		DrawString(20, 20, width, height,
			"Level "+Num2Str(Level)+": Score "+Num2Str(TotalScore)+" in "+Num2Str(TotalMove)+" moves :) " , colors[BLUE_VIOLET]);
	}
if (score>=TotalScore){
	if (moves<=TotalMove){
		gameStatus="Win";
		DrawString(20, 20, width, height,
			"You win! Click anywhere for next level... " , colors[BLUE_VIOLET]); // Displays You Win.. when we win the game
		}
	}
else if (moves>=TotalMove){
	if (score<TotalScore){
		gameStatus="GameOver";
		DrawString(20, 20, width, height,
			"Game Over! Click anywhere to play again... " , colors[BLACK]); //Displays GameOver when we lose the game
		}
	}
}
// #----------------- Write your code till here ----------------------------#
	// do not modify below this
	glutSwapBuffers(); // do not modify this line..

}
/* Function sets canvas size (drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */
void SetCanvasSize(int width, int height) {
	glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
	 glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
}

/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	} else if (key == GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	} else if (key == GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {
	} else if (key == GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {
	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/
	/*
	 glutPostRedisplay();
	 */
}
/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */

void MouseMoved(int x, int y) {

	cout << "Current Mouse Coordinates X=" << x << " Y= " << height - y << endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */

int elemX;   //when pressed
int elemY;   //when pressed
int elemX2;  //when released
int elemY2;  //when released
void MouseClicked(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON) // dealing only with left button
	{
		play=true;           //when play becomes true Game starts
        arrray();
        if (gameStatus=="Win"){
			                        //When the player completes the level
			Level+=1;
			TotalMove+=3;
			increment+=25;
			TotalScore+=100 + increment;
			moves=0;
			score=0;
			gameStatus="Lose";
			}
        if (gameStatus=="GameOver"){
			                     //When the player fails to complete the level
			Level=1;
			TotalMove=5;
			increment=0;
			TotalScore=50;
			moves=0;
			score=0;
			gameStatus="Lose";
			}
		if (state == GLUT_DOWN && gamestate == Ready) {
			cout << "Left Mouse Button Pressed at Coordinates X=" << x << " Y= "
					<< height - y << endl;
			elemX=(x/60)-1;                       //x index of cookie that is clicked from the array
			elemY= y/60-1;                        //y index of cookie that is clicked from the array
			cout<<elemY<<endl;
			cout<<elemX<<endl;
			cout<<arr[elemY][elemX];
		}
		if (state == GLUT_UP && gamestate == Ready) {
			elemX2=(x/60)-1;                       //x index of cookie from the array at which button is released
			elemY2= y/60-1;                        //y index of cookie from the array at which button is released
			cout<<elemY2<<endl;
			cout<<elemX2<<endl;
			cout<<arr[elemY2][elemX2];
			if ( (elemY==elemY2 && (elemX2==elemX+1 || elemX2==elemX-1) ) ||  (elemX==elemX2 && (elemY2==elemY+1 || elemY2==elemY-1)) ){

//         ------------SWAP IN X DIRECTION---------------
                                                  //When we try to swap from the right cookie that have some same cookie present in the adjacent row
				if  (elemY==elemY2 && (elemX2==elemX+1 || elemX2==elemX-1) ){
					bool easily=false;
					int i=elemY-1;
						while ((arr[i][elemX2]==arr[elemY][elemX] ) && i>=0 && i<9) {
							i-=1;}
						int counter1=0;
						i=i+1;
						while (((arr[i][elemX2]==arr[elemY][elemX]  ) || i==elemY2) && i>=0 && i<9){
							counter1+=1;
							i+=1;}
					int counter2=0;
					if (elemX2==elemX+1){
						int j=elemX;
						while (arr[elemY][j]==arr[elemY][elemX] || j==elemX2 ){
							j++;
							counter2+=1;
							}
						}
					else if (elemX2==elemX-1){
						int j=elemX;
						while ( arr[elemY][j]==arr[elemY][elemX] || j==elemX2 ){
							j--;
							counter2++;
						    }
						}
						if (counter1>=3 || counter2>=4){
							swap (arr[elemY][elemX],arr[elemY2][elemX2]);
							 easily=true;
							}
	//-----------------------------------------------------------------------
					                                   //When we try to swap from the other cookie (I call it wrong ccokie)
					 if	 ( easily==false){
						int i=elemY-1;
						while (( arr[i][elemX]==arr[elemY][elemX2] ) && i>=0 && i<9) {
							i-=1;}
						int counter3=0;
						i=i+1;
						while ((( arr[i][elemX]==arr[elemY][elemX2] ) || i==elemY2) && i>=0 && i<9){
							counter3+=1;
							i+=1;}
						int counter4=0;
						if (elemX2==elemX-1){
							int j=elemX2;
							while (arr[elemY][j]==arr[elemY][elemX2] || j==elemX ){
								j++;
								counter4+=1;
								}
							}
						else if (elemX2==elemX+1){
							int j=elemX2;
							while ( arr[elemY][j]==arr[elemY][elemX2] || j==elemX ){
								j--;
								counter4++;
								}
							}
						if (counter3>=3 || counter4>=4){
							swap (arr[elemY][elemX],arr[elemY2][elemX2]);}
						}
				}
//         ------------SWAP IN Y DIRECTION---------------
													//When we try to swap from the right cookie that have some same cookie present in the adjacent column
			if  (elemX==elemX2 && (elemY2==elemY+1 || elemY2==elemY-1) ){
					bool easily=false;
					int i=elemX-1;
					while (arr[elemY2][i]==arr[elemY][elemX] && i>=0 && i<14  ){
						i-=1;}
					int counter1=0;
					i=i+1;
					while (arr[elemY2][i]==arr[elemY][elemX] && i>=0 && i<14  || i==elemX2){
						counter1+=1;
						i+=1;}
                    int counter2=0;
					if (elemY2==elemY+1){
						int j=elemY;
						while (arr[j][elemX]==arr[elemY][elemX] || j==elemY2 ){
							j++;
							counter2+=1;
							}
						}
					else if (elemY2==elemY-1){
						int j=elemY;
						while ( arr[j][elemX]==arr[elemY][elemX] || j==elemY2 ){
							j--;
							counter2++;
						    }
						}
					if (counter1>=3 || counter2>=4){
						 swap (arr[elemY][elemX],arr[elemY2][elemX2]);
						 easily=true;
						 }
	//-----------------------------------------------------------------------
											 //When we try to swap from the other cookie (I call it wrong ccokie)
					 if	 ( easily==false){
						int i=elemX-1;
						while (( arr[elemY][i]==arr[elemY2][elemX2]) && i>=0 && i<14) {
							i-=1;}
						int counter3=0;
						i=i+1;
						while ((( arr[elemY][i]==arr[elemY2][elemX2]) || i==elemX) && i>=0 && i<14){
							counter3+=1;
							i+=1;}
						int counter4=0;
						if (elemY2==elemY-1){
							int j=elemY2;
							while (arr[j][elemX]==arr[elemY2][elemX] || j==elemY ){
								j++;
								counter4+=1;
								}
							}
						else if (elemY2==elemY+1){
							int j=elemY2;
							while ( arr[j][elemX]==arr[elemY2][elemX] || j==elemY ){
								j--;
								counter4++;
								}
							}
						if (counter3>=3 || counter4>=4){
							swap (arr[elemY][elemX],arr[elemY2][elemX2]);}
						}
				}
				}
		}
	}
	 else if (button == GLUT_RIGHT_BUTTON) // dealing with right button
	{
	}
	//glutPostRedisplay();
}
/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 * */
void PrintableKeys(unsigned char key, int x, int y) {
	if (key == KEY_ESC/* Escape key ASCII*/) {
		exit(1); // exit the program when escape key is pressed.
	}
}

/*
 * This function is called after every 1000.0 milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
float dt = 0, lastframe = 0;
void Timer(int m) {
	dt = (m - lastframe) / 100.0;
	lastframe = m;
	// dt is time elapsed between two frames..
	glutPostRedisplay();
  //checkMatches(arr);
	// implement your functionality here
	glutTimerFunc(100, Timer, m + 1);
}

/*
 * our gateway main function
 * */


int main(int argc, char*argv[]) {
	InitRandomizer(); // seed the random number generator...
	//Write your code here for filling the canvas with different Cookies. You can use the GetCookie function for getting the random shaped cookies


	glutInit(&argc, argv); // initialize the graphics library...

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("ITCs Cookie Crush"); // set the title of our game window
	//SetCanvasSize(width, height); // set the number of pixels...

	// Load the textures from the file..;
	RegisterTextures();
	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.

	glutDisplayFunc(DisplayFunction); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	glutMouseFunc(MouseClicked);

//// This function tells the library to call our Timer function after 1000.0 milliseconds...
	glutTimerFunc(1000, Timer, 0);

//// now handle the control to library and it will call our registered functions when
//// it deems necessary...

	glutMainLoop();

	return 1;
}
#endif /* */
