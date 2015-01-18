#include <windows.h>
#include <time.h>
#include <sstream>
#include <string>
#include <vector>
#include <math.h>

#define PI 3.14159265
using namespace std;

const wchar_t g_szClassName[] = L"myWindowClass";

class Domino
{
public:
	Domino(int red1, int red2, int red3, int green1, int green2, int green3, int blue1, int blue2, int blue3);
	void Circle(HDC hDC, double cx, double cy, double radius);
	void Square(HDC hDC, double cx, double cy, double size, int degrees);
	void DominoBase(HDC hDC, double cx, double cy, double size, int degrees);
	void DrawDomino(HDC hDC, double cx, double cy, double size, int degrees, int randint, int randint2);

	double toRadians(double angle);
	double getPolarRadius(double xr, double yr);
	void SingleDot(HDC hDC, double x, double y, double dotsize);
	void DoubleDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void TripleDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void QuadDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void PentaDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void HexaDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void RollDice(HDC hDC, double x, double y, double dotsize, int randint, double degrees);
	void InitializeBrushes();
	void InitializeColors(int red1, int red2, int red3, int green1, int green2, int green3, int blue1, int blue2, int blue3);
	void line(HDC hdc, int x1, int y1, int x2, int y2);
	void centerOrientedHorizontalLine(HDC hDC, double cx, double cy, double size);
	~Domino();
private:
	HPEN hPen;
	HBRUSH hBrush1;
	HBRUSH hBrush2;
	HBRUSH hBrush3;
	static int randint;
	static int randint2;
	double cx;
	double xy;
	int red1; int green1; int blue1;
	int red2; int green2; int blue2;
	int red3; int green3; int blue3;
};

//void Circle(HDC hDC, int cx, int cy, int radius, int size);
//void Square(HDC hDC, int cx, int cy, int area);

/*<---------------------Constructor--------------------------*/
Domino::Domino(int red1, int red2, int red3, int green1, int green2, int green3, int blue1, int blue2, int blue3)
{
	InitializeColors(red1, red2, red3, green1, green2, green3, blue1, blue2, blue3);
	InitializeBrushes();
}
/*<!--------------------Constructor--------------------------*/
/*<!--------------------shapes-------------------------------*/
void Domino::Circle(HDC hDC, double cx, double cy, double radius)
{
	Ellipse(hDC, (int)cx - (int)radius, (int)cy - (int)radius, (int)cx + (int)radius, (int)cy + (int)radius);
}

void Domino::Square(HDC hDC, double cx, double cy, double size, int degrees)
{

	POINT ptArray[5];
	/*Procedure to rotate dice:*/
	/*Convert from square coordinates to polar, update degrees, then back to square coordinates*/
	//Setting up:
	/* Origin set at 0,0
	origin (0) + pre-determined offset for points to create a square (size) = x location of point (x)
	origin (0) + pre-determined offset for points to create a square (size) = y location of point (y)*/
	/*double xorigin = 0;
	  double yorigin = 0;
	  double xoffset = xorigin + size;;; ... redundant */
	double xoffset = size;
	double yoffset = size;
	double sidelength = size*2;
	double r = getPolarRadius(xoffset, yoffset); //get radius from origin to point
	double radians = (3.14) - (degrees * PI / 180);

	//add cx or cy to track with static int x & static int y from main
	//ptArray[0].x = (long)(r*cos(radians)) + cx;				ptArray[0].y = (long)(r*sin(radians)) + cy; //needs no rotation
	//ptArray[1].x = (long)(r*cos(radians - 1.57)) + cx;		ptArray[1].y = (long)(r*sin(radians - 1.57)) + cy; //-1.57 rad = 90 deg rotate about origin
	//ptArray[2].x = (long)(r*cos(radians - 3.14)) + cx;		ptArray[2].y = (long)(r*sin(radians - 3.14)) + cy; //-3.14 rad = 180 deg rotate about origin
	//ptArray[3].x = (long)(r*cos(radians - 4.71)) + cx;		ptArray[3].y = (long)(r*sin(radians - 4.71)) + cy; //-4.71 = 270 deg rotate about origin
	//ptArray[4].x = (long)(r*cos(radians) + cx);				ptArray[4].y = (long)(r*sin(radians) + cy); //close polygon 
	ptArray[0].x = (long)(cx - sidelength / 2);		ptArray[0].y = (long)(cy - sidelength / 2); //needs no rotation
	ptArray[1].x = (long)(cx + sidelength / 2);		ptArray[1].y = (long)(cy - sidelength / 2); //-1.57 rad = 90 deg rotate about origin
	ptArray[2].x = (long)(cx + sidelength / 2);		ptArray[2].y = (long)(cy + sidelength / 2); //-3.14 rad = 180 deg rotate about origin
	ptArray[3].x = (long)(cx - sidelength / 2);		ptArray[3].y = (long)(cy + sidelength / 2); //-4.71 = 270 deg rotate about origin
	ptArray[4].x = (long)(cx - sidelength / 2);		ptArray[4].y = (long)(cy - sidelength / 2); //close polygon 
	Polygon(hDC, ptArray, 5); //create square
}

void Domino::DominoBase(HDC hDC, double cx, double cy, double size, int degrees)
{
	//Draws a rectangle to hold the two squares, then draws the squares.
	POINT ptArray[5];
	/*Procedure to rotate dice:*/
	/*Convert from square coordinates to polar, update degrees, then back to square coordinates*/
	//Setting up:
	/* Origin set at 0,0
	origin (0) + pre-determined offset for points to create a square (size) = x location of point (x)
	origin (0) + pre-determined offset for points to create a square (size) = y location of point (y)*/
	/*double xorigin = 0;
	double yorigin = 0;
	double xoffset = xorigin + size;;; ... redundant */
	double polyOriginx = cx;
	double polyOriginy = cy;
	double xoffset = size;
	double yoffset = size;
	double sidelength = size*2;
	double lsidelength = size*4;
	double degreesInRadians = toRadians(degrees);
	double radius = getPolarRadius(sidelength,lsidelength); //get radius from origin to point
	/*
	double radiusX2 = getPolarRadius();

	double radiusY1 = getPolarRadius();
	double radiusY2 = getPolarRadius();
	*/
	double radius0 = getPolarRadius((sidelength / 2), (lsidelength / 2)); //get radius from origin to point
	double pAngle0 = atan((cy - lsidelength / 2) / (cx - sidelength / 2)) + degreesInRadians;
	//pAngle0 = toRadians(pAngle0);
	double radius1 = getPolarRadius((sidelength / 2), (lsidelength / 2)); //get radius from origin to point
	double pAngle1 = atan((cy - lsidelength / 2) / (cx + sidelength / 2)) + degreesInRadians;
	//pAngle1 = toRadians(pAngle1);
	double radius2 = getPolarRadius((sidelength / 2), (lsidelength / 2)); //get radius from origin to point
	double pAngle2 = atan((cy + lsidelength / 2) / (cx + sidelength / 2)) + degreesInRadians;
	//pAngle2 = toRadians(pAngle2);
	double radius3 = getPolarRadius((sidelength / 2), (lsidelength / 2)); //get radius from origin to point
	double pAngle3 = atan((cy + lsidelength / 2) / (cy + lsidelength / 2)) + degreesInRadians;
	//pAngle3 = toRadians(pAngle3);
	double radius4 = getPolarRadius((sidelength / 2), (lsidelength / 2)); //get radius from origin to point
	double pAngle4 = atan((cy - lsidelength / 2) / (cx - sidelength / 2)) + degreesInRadians;
	//pAngle4 = toRadians(pAngle4);
	//add cx or cy to track with static int x & static int y from main

	ptArray[0].x = (long)(cx - sidelength / 2);		ptArray[0].y = (long)(cy - lsidelength / 2); //needs no rotation
	ptArray[1].x = (long)(cx + sidelength / 2);		ptArray[1].y = (long)(cy - lsidelength / 2); //-1.57 rad = 90 deg rotate about origin
	ptArray[2].x = (long)(cx + sidelength / 2);		ptArray[2].y = (long)(cy + lsidelength / 2); //-3.14 rad = 180 deg rotate about origin
	ptArray[3].x = (long)(cx - sidelength / 2);		ptArray[3].y = (long)(cy + lsidelength / 2); //-4.71 = 270 deg rotate about origin
	ptArray[4].x = (long)(cx - sidelength / 2);		ptArray[4].y = (long)(cy - lsidelength / 2); //close polygon 

	//ptArray[0].x = cx + radius0*cos(pAngle0);					ptArray[0].y = cy + radius0*sin(pAngle0); //needs no rotation
	//ptArray[1].x = cx + radius1*cos(pAngle1 - 1.57);			ptArray[1].y = cy + radius1*sin(pAngle1 - 1.57); //-1.57 rad = 90 deg rotate about origin
	//ptArray[2].x = cx + radius2*cos(pAngle2 - 3.14);			ptArray[2].y = cy + radius2*sin(pAngle2 - 3.14); //-3.14 rad = 180 deg rotate about origin
	//ptArray[3].x = cx + radius3*cos(pAngle3 - 4.71);			ptArray[3].y = cy + radius3*sin(pAngle3 - 4.71); //-4.71 = 270 deg rotate about origin
	//ptArray[4].x = cx + radius4*cos(pAngle4);					ptArray[4].y = cy + radius4*sin(pAngle4); //close polygon 
	
	//ORIGINAL 
	//ptArray[0].x = cx + radius*cos(radians);					ptArray[0].y = cy + radius*sin(radians); //needs no rotation
	//ptArray[1].x = cx + radius*cos(radians - 1.57);			ptArray[1].y = cy + radius*sin(radians - 1.57); //-1.57 rad = 90 deg rotate about origin
	//ptArray[2].x = cx + radius*cos(radians - 3.14);			ptArray[2].y = cy + radius*sin(radians - 3.14); //-3.14 rad = 180 deg rotate about origin
	//ptArray[3].x = cx + radius*cos(radians - 4.71);			ptArray[3].y = cy + radius*sin(radians - 4.71); //-4.71 = 270 deg rotate about origin
	//ptArray[4].x = cx + radius*cos(radians);					ptArray[4].y = cy + radius*sin(radians); //close polygon 
	
	
	Polygon(hDC, ptArray, 5); //create square
}

void Domino::DrawDomino(HDC hDC, double cx, double cy, double size, int degrees, int randint, int randint2)
{
	SelectObject(hDC, hBrush1);
	DominoBase(hDC, cx, cy, size, degrees);
	double sizeMinusPadding = 0.80*size;
	double dotsize = sizeMinusPadding / 6;
	SelectObject(hDC, hBrush3);
	Square(hDC, cx, cy - size, sizeMinusPadding, degrees);
	Square(hDC, cx, cy + size, sizeMinusPadding, degrees);
	centerOrientedHorizontalLine(hDC, cx, cy, sizeMinusPadding);
	SelectObject(hDC, hBrush2);
	RollDice(hDC, cx, cy - size, dotsize, randint, degrees-136);
	RollDice(hDC, cx, cy + size, dotsize, randint2, degrees-136);
}
void Domino::line(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}
void Domino::centerOrientedHorizontalLine(HDC hDC, double cx, double cy, double size)
{
	line(hDC, cx - size, cy, cx + size, cy);
}
/*<!/-------------------shapes------------------------------>*/

/*<!--------------------Dice functions-----------------------*/
double Domino::toRadians(double angle)
{
	return (3.14) - (angle * PI / 180);
}
double Domino::getPolarRadius(double xr, double yr)
{
	double xrsquared = pow(xr, 2);
	double yrsquared = pow(yr, 2);
	return sqrt(xrsquared + yrsquared);
}
void Domino::SingleDot(HDC hDC, double x, double y, double dotsize)
{
	Circle(hDC, x, y, dotsize);
}
void Domino::DoubleDot(HDC hDC, double x, double y, double dotsize, double degrees)
{
	double radians = (3.14) - (degrees * PI / 180);
	/* Origin set at 0,0 
		origin(0) + pre-determined offset for dot (3*dotsize) = location of dot (x)
		origin(0) + pre-determined offset for dot (3*dotsize) = location of dot (y)*/
	double xcenterdot = 3 * dotsize;
	double ycenterdot = 3 * dotsize;
	double r = getPolarRadius(xcenterdot, ycenterdot); //find radius between these new points

	Circle(hDC, r*cos(radians) + x, r*sin(radians) + y, dotsize); //create dot at new points, add x and y values from main
	Circle(hDC, -(r*cos(radians)) + x, -(r*sin(radians)) + y, dotsize); //so that they track according to center of square
}
void Domino::TripleDot(HDC hDC, double x, double y, double dotsize, double degrees)
{
	/*Rest of functions simple copy/paste of DoubleDot, except HexaDot*/
	double radians = (3.14) - (degrees * PI / 180);
	double xcenterdot = 3 * dotsize;
	double ycenterdot = 3 * dotsize;
	double r = getPolarRadius(xcenterdot, ycenterdot);

	Circle(hDC, x, y, dotsize);
	Circle(hDC, r*cos(radians) + x, r*sin(radians) + y, dotsize);
	Circle(hDC, -(r*cos(radians)) + x, -(r*sin(radians)) + y, dotsize); //inverted x and y values for mirrored dot
}
void Domino::QuadDot(HDC hDC, double x, double y, double dotsize, double degrees)
{
	double radians = (3.14) - (degrees * PI / 180);
	double xcenterdot = 3 * dotsize;
	double ycenterdot = 3 * dotsize;
	double r = getPolarRadius(xcenterdot, ycenterdot);

	Circle(hDC, r*cos(radians) + x, r*sin(radians) + y, dotsize);
	Circle(hDC, -(r*cos(radians)) + x, -(r*sin(radians)) + y, dotsize);
	Circle(hDC, r*cos(radians +  1.57) + x, r*sin(radians + 1.57) + y, dotsize); //rotate 90 degrees
	Circle(hDC, -(r*cos(radians + 1.57)) + x, -(r*sin(radians + 1.57)) + y, dotsize); //rotate 90 degrees

}
void Domino::PentaDot(HDC hDC, double x, double y, double dotsize, double degrees)
{
	/*copy/paste of QuadDot*/
	double radians = (3.14) - (degrees * PI / 180);
	double xcenterdot = 3 * dotsize;
	double ycenterdot = 3 * dotsize;
	double r = getPolarRadius(xcenterdot, ycenterdot);

	Circle(hDC, x, y, dotsize);
	Circle(hDC, r*cos(radians) + x, r*sin(radians) + y, dotsize);
	Circle(hDC, -(r*cos(radians)) + x, -(r*sin(radians)) + y, dotsize);
	Circle(hDC, r*cos(radians + 1.57) + x, r*sin(radians + 1.57) + y, dotsize);
	Circle(hDC, -(r*cos(radians + 1.57)) + x, -(r*sin(radians + 1.57)) + y, dotsize);
}
void Domino::HexaDot(HDC hDC, double x, double y, double dotsize, double degrees)
{
	double radians = (3.14) - (degrees * PI / 180);
	double xcenterdot = 3 * dotsize;
	double ycenterdot = 3 * dotsize;
	double r = getPolarRadius(xcenterdot, ycenterdot);

	Circle(hDC, r*cos(radians) + x, r*sin(radians) + y, dotsize);
	Circle(hDC, -(r*cos(radians)) + x, -(r*sin(radians)) + y, dotsize);
	Circle(hDC, r*cos(radians + 1.57) + x, r*sin(radians + 1.57) + y, dotsize);
	Circle(hDC, -(r*cos(radians + 1.57)) + x, -(r*sin(radians + 1.57)) + y, dotsize);

	xcenterdot = 2.14 * dotsize; /*2.14 trial/error ratio to create small circle between outter circle */
	ycenterdot = 2.14 * dotsize; /*to create 6-dot look*/
	r = getPolarRadius(xcenterdot, ycenterdot);//get inner circle radius

	Circle(hDC, (r*cos(radians + 0.785)) + x, (r*sin(radians + 0.785)) + y, dotsize); //rotate 45 degrees
	Circle(hDC, -(r*cos(radians + 0.785)) + x, -(r*sin(radians + 0.785)) + y, dotsize); //rotate 45 degrees
}
void Domino::RollDice(HDC hDC, double x, double y, double dotsize, int randint, double degrees)
{
	switch (randint)
	{
		case 0:
			//Draw nothing
			break;
		case 1:
			SingleDot(hDC, x, y, dotsize);
			break;
		case 2:
			DoubleDot(hDC, x, y, dotsize, degrees);
			break;
		case 3:
			TripleDot(hDC, x, y, dotsize, degrees);
			break;
		case 4:
			QuadDot(hDC, x, y, dotsize, degrees);
			break;
		case 5:
			PentaDot(hDC, x, y, dotsize, degrees);
			break;
		case 6:
			HexaDot(hDC, x, y, dotsize, degrees);
			break;
		default:
			break;
	}
}
void Domino::InitializeBrushes()
{
	hBrush1 = CreateSolidBrush(RGB(red1, green1, blue1));
	hBrush2 = CreateSolidBrush(RGB(red2, green2, blue2));
	hBrush3 = CreateSolidBrush(RGB(red3, green3, blue3));
	hPen = CreatePen(PS_SOLID, 2, RGB(red2, green2, blue2));
}
void Domino::InitializeColors(int reda, int redb, int redc, int greena, int greenb, int greenc, int bluea, int blueb, int bluec)
{
	red1 = reda; red2 = redb; red3 = redc; 
	green1 = greena; green2 = greenb; green3 = greenc;
	blue1 = bluea; blue2 = blueb; blue3 = bluec;
}
Domino::~Domino()
{
	DeleteObject(hPen);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
}
unsigned int CompareRands(int randinta, int randintb)
{
	int count = 0;
	if ((randinta % 2) == 0) { count++; }
	if ((randintb % 2) == 0) { count++; }
	return count;
}
/*<!/-------------------Dice functions-----------------------*/

//step 4: window prcoedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC; //handle to device context
	PAINTSTRUCT Ps;
	HPEN hPen;

	HBRUSH hBrush4;
	HFONT hFont;
	RECT rektangle; //rectangle object, defines x and y values
	RECT rektangle2;

	
	int fontHeight = 50;
	static int x = 380;
	static int y = 380;
	static int x2 = x + 300;
	static int y2 = y;
	static int size = 90; //squure size
	static int dotsize = size / 6;
	static int degrees = 0;
	srand((unsigned int)time(NULL));
	static int randint = (rand() % 7);
	static int randint2 = (rand() % 7);

	static int red = 255;
	static int green = 253;
	static int blue = 230;

	static int red3 = 255;
	static int green3 = 253;
	static int blue3 = 230;

	static int red4 = 0;
	static int green4 = 0;
	static int blue4 = 0;

	static int red2 = 0;
	static int blue2 = 0;
	static int green2 = 0;

	static int numeven = 0;

	static vector<Domino> DominoSet;

	Domino o1(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o1);
	Domino o2(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o2);
	Domino o3(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o3);
	Domino o4(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o4);
	Domino o5(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o5);
	Domino o6(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o6);
	Domino o7(red, red2, red3, green, green2, green3, blue, blue2, blue3);
	DominoSet.push_back(o7);

	switch (msg)
	{
		case WM_PAINT:
			hDC = BeginPaint(hwnd, &Ps);
			
			
			hBrush4 = CreateSolidBrush(RGB(red4, green4, blue4));
			hFont = CreateFont(fontHeight, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");

//			SelectObject(hDC, hBrush);
			//DrawDomino(hDC, x, y, size, degrees, randint, randint2, hBrush, hBrush2, hBrush3);
			//DominoDeck[0].DrawDomino(hDC, x, y, size, degrees, randint, randint2, hBrush, hBrush2, hBrush3);
			
			for (unsigned int i = 0; i++; i < DominoSet.size())
			{
				DominoSet[i].DrawDomino(hDC, x, y, size, degrees, randint, randint2);

			}
			
			
			//SelectObject(hDC, hBrush3);
			//Square(hDC, x2, y2, size, degrees);

			//SelectObject(hDC, hBrush2);
			//RollDice(hDC, x, y, dotsize, randint, degrees);
			//SelectObject(hDC, hBrush4);
			//RollDice(hDC, x2, y2, dotsize, randint2, degrees);
			//numeven = CompareRands(randint, randint2);

			rektangle.top = 100;
			rektangle.bottom = 150;
			rektangle.right = 1000;
			rektangle.left = 320;

			SelectObject(hDC, hFont);
			
			switch (numeven)
			{
				case 0:
					DrawText(hDC, L"There are 0 even dice", -1, &rektangle, DT_SINGLELINE);
					break;
				case 1:
					DrawText(hDC, L"There are 1 even dice", -1, &rektangle, DT_SINGLELINE);
					break;
				case 2:
					DrawText(hDC, L"There are 2 even dice", -1, &rektangle, DT_SINGLELINE);
					break;
			}

			rektangle2.top = 650;
			rektangle2.bottom = 700;
			rektangle2.right = 680;
			rektangle2.left = 380;
			DrawText(hDC, L"Press R to rotate", -1, &rektangle2, DT_SINGLELINE | DT_CENTER);

			
			DeleteObject(hBrush4);
			DeleteObject(hFont);
			EndPaint(hwnd, &Ps);
			break;
		case WM_CHAR:
			if (wParam == 'c' || wParam == 'C')
			{
				red = (rand() % 256);
				green = (rand() % 256);
				blue = (rand() % 256);

				red3 = (rand() % 256);
				green3 = (rand() % 256);
				blue3 = (rand() % 256);
				
				try //check if the background is dark
				{
					if ((red < 128) && (green < 128) && (blue < 128))
					{
						red2 = 0;
						blue2 = 0;
						green2 = 0;
						red2 = (rand() % 127) + 128; //create light shaded dot 
						blue2 = (rand() % 127) + 128;
						green2 = (rand() % 127) + 128;
						if ((red2 > 256) || (blue2 > 256) || (green2 > 256)) throw (1);
						InvalidateRect(hwnd, NULL, true);
						break;
					}
					if ((red3 < 128) && (green3 < 128) && (blue3 < 128))
					{
						red4 = 0;
						blue4 = 0;
						green4 = 0;
						red4 = (rand() % 127) + 128; //create light shaded dot 
						blue4 = (rand() % 127) + 128;
						green4 = (rand() % 127) + 128;
						if ((red4 > 256) || (blue4 > 256) || (green4 > 256)) throw (2);
						InvalidateRect(hwnd, NULL, true);
						break;
					}
				}
				catch (int a)
				{
					if (a == 1)
					{
						red2 = 0;
						blue2 = 0;
						green2 = 0;
						break;
					}
					if (a == 2)
					{
						red4 = 0;
						blue4 = 0;
						green4 = 0;
						break;
					}
				}
				//otherwise just create dark dots
				red2 = (rand() % 128);
				blue2 = (rand() % 128);
				green2 = (rand() % 128);
				InvalidateRect(hwnd, NULL, true);
			}
		/*	else if (wParam == 'r' || wParam == 'R')
			{
				degrees += 1;
				InvalidateRect(hwnd, NULL, true);
			}*/
			break;
		case WM_KEYDOWN:
			if (wParam == VK_DOWN)
			{
				y += 10;
				y2 += 10;
				InvalidateRect(hwnd, NULL, true); //invalidate window
			}
			else if (wParam == VK_UP)
			{
				y -= 10;
				y2 -= 10;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == VK_LEFT)
			{
				x -= 10;
				x2 -= 10;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == VK_RIGHT)
			{
				x += 10;
				x2 += 10;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == 33)
			{
				size += 6;
				dotsize += 1;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == 34)
			{
				if (size >= 10)
				{
					size -= 6;
					dotsize -= 1;
					InvalidateRect(hwnd, NULL, true);
				}
			}
			else if (wParam == 32)
			{
				randint = (rand() % 6) + 1;
				randint2 = (rand() % 6) + 1;
				InvalidateRect(hwnd, NULL, true);
			}
			break;
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc; //windows class
	HWND hwnd; //handle to the window (pointer to a pointer)
	MSG Msg; //message

	//Step 1: registering the windows class okay
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc; //tell os to call wndproc for procedure
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); //handle to brush
	wc.lpszMenuName = NULL;
	wc.lpszClassName = g_szClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	//step 2: create window
	hwnd = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		g_szClassName,
		L"Washington Garcia",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, 1100, 800, //default size in pixels
		NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, L"Window Creation failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}
	//window should be created
	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	//Step 3: message loop
	while (GetMessage(&Msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&Msg);
		DispatchMessage(&Msg);
	}
	return Msg.wParam;
}