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
	Domino(int red1, int red2, int red3, int green1, int green2, int green3, int blue1, int blue2, int blue3, double x, double y, int getsize, int getrand1, int getrand2, int degrees);
	//Domino constructor, this initializes all the variables the domino needs with the static variables declared in WindProc
	//From here the values are distributed to the member functions as they are called. 

	void Circle(HDC hDC, double cx, double cy, double radius);
	void Square(HDC hDC, double cx, double cy, double size, int degrees, bool position, POINT pptArray[]);
	void DominoBase(HDC hDC, double cx, double cy, double size, int degrees); //Draws the basic rectangle shape
	void DrawDomino(HDC hDC, int degrees); //Puts together all the shapes and handles rotation of the shapes using DominoBase's values

	void Rotate(POINT& point, POINT origin, double angle); //rotate point struct around given origin, angle accepts only radians
	double toRadians(double angle); //converts degrees to radians
	double getPolarRadius(double xr, double yr); //returns polar radius given two values, used primarily for dot functions
	void SingleDot(HDC hDC, double x, double y, double dotsize); //draw single dot
	void DoubleDot(HDC hDC, double x, double y, double dotsize, double degrees); //draw two dots..
	void TripleDot(HDC hDC, double x, double y, double dotsize, double degrees); //...
	void QuadDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void PentaDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void HexaDot(HDC hDC, double x, double y, double dotsize, double degrees);
	void RollDice(HDC hDC, double x, double y, double dotsize, int randint, double degrees, POINT pptArray[]); //draw the dice given randum number, handles rotation by using Square's values
	void InitializeBrushes(); //Create the brushes
	void InitializeColors(int red1, int red2, int red3, int green1, int green2, int green3, int blue1, int blue2, int blue3);
	void line(HDC hDC, double x1, double y1, double x2, double y2); //draws a line from point 1 to point 2
	void centerOrientedHorizontalLine(HDC hDC, double cx, double cy, double size, int degrees); //draws a line given only center point and degrees, handles rotation
	void UpdateRandint(int getrand1, int getrand2); //change the values of randint, unused because this is done automatically everytime WindProc is called
	unsigned int CompareRands(); //add the domino's random numbers and return 1 if its even. 

	~Domino(); //Delete the brushes
private:
	HPEN hPen;
	HBRUSH hBrush1;
	HBRUSH hBrush2;
	HBRUSH hBrush3;
	int squareint1; //random numb for first square
	int squareint2; //random numb for second square
	double cx; //origin x value
	double cy; //origin y value
	int red1; int green1; int blue1; //colors for the brushes
	int red2; int green2; int blue2;
	int red3; int green3; int blue3;
	int size; //main size of the domino, given by WindProc
};


/*<---------------------Constructor--------------------------*/
Domino::Domino(int red1, int red2, int red3, int green1, int green2, int green3, int blue1, int blue2, int blue3, double x, double y, int getsize, int getrand1, int getrand2, int degrees)
{
	squareint1 = getrand1;
	squareint2 = getrand2;
	size = getsize;
	cx = x;
	cy = y;
	InitializeColors(red1, red2, red3, green1, green2, green3, blue1, blue2, blue3);
	InitializeBrushes();
}
/*<!--------------------Constructor--------------------------*/
/*<!--------------------shapes-------------------------------*/
void Domino::Rotate(POINT& point, POINT origin, double angle)
{
	int X = origin.x + (int)((point.x - origin.x) * cos(angle) - (point.y - origin.y) * sin(angle));
	int Y = origin.y + (int)((point.x - origin.x) * sin(angle) + (point.y - origin.y) * cos(angle));
	point = { X, Y };
}
void Domino::Circle(HDC hDC, double cx, double cy, double radius)
{
	Ellipse(hDC, (int)cx - (int)radius, (int)cy - (int)radius, (int)cx + (int)radius, (int)cy + (int)radius);
}

void Domino::Square(HDC hDC, double cx, double cy, double size, int degrees, bool position, POINT pptArray[])
{

	POINT ptArray[5];
	/*Procedure to rotate dice:*/
	/*Convert from square coordinates to polar, update degrees, then back to square coordinates*/
	//Setting up:
	/* Origin set at 0,0
	origin (0) + pre-determined offset for points to create a square (size) = x location of point (x)
	origin (0) + pre-determined offset for points to create a square (size) = y location of point (y)*/

	POINT origin = { (long)cx, (long)cy };
	double sidelength = size * 2;
	double sizePlusPadding = size*1.20;
	double degreesInRadians = toRadians(degrees - 18); //convert to radians with some adjustment added in


	switch (position)
	{
		case 1:
			ptArray[0].x = (long)(cx - sidelength / 2);		ptArray[0].y = (long)(sizePlusPadding + cy - sidelength / 2); //
			ptArray[1].x = (long)(cx + sidelength / 2);		ptArray[1].y = (long)(sizePlusPadding + cy - sidelength / 2); //add the size with padding included into the y values
			ptArray[2].x = (long)(cx + sidelength / 2);		ptArray[2].y = (long)(sizePlusPadding + cy + sidelength / 2); //because this is the upper square
			ptArray[3].x = (long)(cx - sidelength / 2);		ptArray[3].y = (long)(sizePlusPadding + cy + sidelength / 2); //
			ptArray[4].x = (long)(cx - sidelength / 2);		ptArray[4].y = (long)(sizePlusPadding + cy - sidelength / 2); //close polygon 
			pptArray[0].x = (long)(cx);
			pptArray[0].y = (long)(sizePlusPadding + cy); // shift dots up like the square
			break;
		case 0:
			ptArray[0].x = (long)(cx - sidelength / 2);		ptArray[0].y = (long)((cy - sidelength / 2) - sizePlusPadding); //subtract the size with padding included into the y values
			ptArray[1].x = (long)(cx + sidelength / 2);		ptArray[1].y = (long)((cy - sidelength / 2) - sizePlusPadding); //because this is the bottom square
			ptArray[2].x = (long)(cx + sidelength / 2);		ptArray[2].y = (long)((cy + sidelength / 2) - sizePlusPadding); //
			ptArray[3].x = (long)(cx - sidelength / 2);		ptArray[3].y = (long)((cy + sidelength / 2) - sizePlusPadding); //
			ptArray[4].x = (long)(cx - sidelength / 2);		ptArray[4].y = (long)((cy - sidelength / 2) - sizePlusPadding); //close polygon 
			pptArray[0].x = (long)(cx);
			pptArray[0].y = (long)(cy - sizePlusPadding); // shift the dots down like the square
			break;
	}

	for (int i = 0; i < 5; i++)
	{

		Rotate(ptArray[i], origin, degreesInRadians);
		Rotate(pptArray[i], origin, degreesInRadians);

	}
	Polygon(hDC, ptArray, 5); //create square
}

void Domino::DominoBase(HDC hDC, double cx, double cy, double size, int degrees)
{
	//Draws a rectangle to hold the two squares, then draws the squares.
	POINT ptArray[5]; //array of points (x,y struct) to create a shape

	/*Procedure to rotate dice:*/
	/*Convert from square coordinates to polar, update degrees, then back to square coordinates*/
	//Setting up:
	/* Origin set at 0,0
	origin (0) + pre-determined offset for points to create a square (size) = x location of point (x)
	origin (0) + pre-determined offset for points to create a square (size) = y location of point (y)*/


	double polyOriginx = cx;
	double polyOriginy = cy;
	POINT origin = { (long)polyOriginx, (long)polyOriginy };
	double xoffset = size;
	double yoffset = size;
	double sidelength = size * 2;
	double lsidelength = size * 4;
	double degreesInRadians = toRadians(degrees - 18);
	//double radius = getPolarRadius(sidelength,lsidelength); //get radius from origin to point


	//add cx or cy to track with static int x & static int y from main

	ptArray[0].x = (long)(cx - sidelength / 2);		ptArray[0].y = (long)(cy - lsidelength / 2); //needs no rotation
	ptArray[1].x = (long)(cx + sidelength / 2);		ptArray[1].y = (long)(cy - lsidelength / 2); //-1.57 rad = 90 deg rotate about origin
	ptArray[2].x = (long)(cx + sidelength / 2);		ptArray[2].y = (long)(cy + lsidelength / 2); //-3.14 rad = 180 deg rotate about origin
	ptArray[3].x = (long)(cx - sidelength / 2);		ptArray[3].y = (long)(cy + lsidelength / 2); //-4.71 = 270 deg rotate about origin
	ptArray[4].x = (long)(cx - sidelength / 2);		ptArray[4].y = (long)(cy - lsidelength / 2); //close polygon 

	for (int i = 0; i < 5; i++)
	{
		Rotate(ptArray[i], origin, degreesInRadians); //go through each point and rotate it w.r.t the origin
	}

	Polygon(hDC, ptArray, 5); //create shape 
}

void Domino::DrawDomino(HDC hDC, int degrees)
{
	POINT pptArrayTOP[5]; //array of points to hold the points of the top square, this will handle rotation of the upper dots
	POINT pptArrayBOT[5]; //array of points to hold the points of the lower square, this will handle rotation of the bottom dots

	int adjustment = 63; //degree adjustment to make dice circles line up with the squares, number is arbitrarily chosen until it looks right
	int reverseDegrees = -(degrees); //the dice were rotating in the opposite direction so we need this instead
	double centerOfBotSquare = cy - size;
	double centerOfTopSquare = cy + size;
	reverseDegrees += adjustment;

	SelectObject(hDC, hPen);
	SelectObject(hDC, hBrush1);
	DominoBase(hDC, cx, cy, size, degrees);

	double sizeMinusPadding = 0.80*size;
	double dotsize = sizeMinusPadding / 6;

	SelectObject(hDC, hBrush3);
	Square(hDC, cx, cy, sizeMinusPadding, degrees, 0, pptArrayBOT); //gives me values of x and y that reflect the current rotation of the square
	Square(hDC, cx, cy, sizeMinusPadding, degrees, 1, pptArrayTOP);
	centerOrientedHorizontalLine(hDC, cx, cy, sizeMinusPadding, degrees);
	SelectObject(hDC, hBrush2);
	/*RollDice(hDC, cx, cy - size, dotsize, squareint1, degrees-136);
	RollDice(hDC, cx, cy + size, dotsize, squareint2, degrees-136);*/
	RollDice(hDC, cx, centerOfBotSquare, dotsize, squareint1, reverseDegrees, pptArrayBOT); //pass the array back into the rolldice to update the position based on rotation of the square
	RollDice(hDC, cx, centerOfTopSquare, dotsize, squareint2, reverseDegrees, pptArrayTOP);
}
void Domino::line(HDC hDC, double x1, double y1, double x2, double y2)
{
	MoveToEx(hDC, (int)x1, (int)y1, NULL);
	LineTo(hDC, (int)x2, (int)y2);
}
void Domino::centerOrientedHorizontalLine(HDC hDC, double cx, double cy, double size, int degrees)
{
	double degreesInRadians = toRadians(degrees - 18);
	double LeftPoint = cx - size;
	double RightPoint = cx + size;
	POINT origin = { (long)cx, (long)cy };
	POINT point1 = { (long)LeftPoint, (long)cy };
	POINT point2 = { (long)RightPoint, (long)cy };
	Rotate(point1, origin, degreesInRadians);
	Rotate(point2, origin, degreesInRadians);
	line(hDC, point1.x, point1.y, point2.x, point2.y);
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
	Circle(hDC, r*cos(radians + 1.57) + x, r*sin(radians + 1.57) + y, dotsize); //rotate 90 degrees
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
/*<!/-------------------shapes------------------------------>*/

/*<!--------------------Dice functions-----------------------*/
void Domino::UpdateRandint(int getrand1, int getrand2)
{
	squareint1 = getrand1;
	squareint2 = getrand2;
}
double Domino::toRadians(double angle)
{
	return (angle * (PI / 180));
}
double Domino::getPolarRadius(double xr, double yr) 
{
	double xrsquared = pow(xr, 2);
	double yrsquared = pow(yr, 2);
	return sqrt(xrsquared + yrsquared);
}

void Domino::RollDice(HDC hDC, double x, double y, double dotsize, int randint, double degrees, POINT pptArray[])
{
	switch (randint)
	{
		case 0:
			//Draw nothing
			break;
		case 1:
			SingleDot(hDC, pptArray[0].x, pptArray[0].y, dotsize);
			break;
		case 2:
			DoubleDot(hDC, pptArray[0].x, pptArray[0].y, dotsize, degrees);
			break;
		case 3:
			TripleDot(hDC, pptArray[0].x, pptArray[0].y, dotsize, degrees);
			break;
		case 4:
			QuadDot(hDC, pptArray[0].x, pptArray[0].y, dotsize, degrees);
			break;
		case 5:
			PentaDot(hDC, pptArray[0].x, pptArray[0].y, dotsize, degrees);
			break;
		case 6:
			HexaDot(hDC, pptArray[0].x, pptArray[0].y, dotsize, degrees);
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
unsigned int Domino::CompareRands()
{
	int count = 0;
	if (((squareint1 + squareint2) % 2) == 0) { count++; }
	return count;
}

Domino::~Domino()
{
	DeleteObject(hPen);
	DeleteObject(hBrush1);
	DeleteObject(hBrush2);
	DeleteObject(hBrush3);
}

/*<!/-------------------Dice functions-----------------------*/

//step 4: window prcoedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hDC; //handle to device context
	PAINTSTRUCT Ps;

	HFONT hFont;
	RECT rektangle; //rectangle object, defines x and y values "There are... even dice"
	RECT rektangle2; // "Press R to Rotate"


	int fontHeight = 50;
	static int x = 100; //center point of everything
	static int y = 380;
	static int size = 50; //domino size
	static int degrees = -162; //degrees that rotate all objects, number arbitrarily chosen until the dominoes are vertically aligned. 

	srand((unsigned int)time(NULL));
	static int randint[14] = { rand() % 7, rand() % 7, rand() % 7, rand() % 7, rand() % 7, rand() % 7, rand() % 7,
		rand() % 7, rand() % 7, rand() % 7, rand() % 7, rand() % 7, rand() % 7, rand() % 7, };
	static int randintsize = 14; //We need 2 random numbers for each domino, so this is twice the number of dominos

	static int red[7] = { 255, 255, 255, 255, 255, 255, 255 }; //domino base brush colors
	static int green[7] = { 253, 253, 253, 253, 253, 253, 253 };
	static int blue[7] = { 230, 230, 230, 230, 230, 230, 230 };

	static int red3[7] = { 255, 255, 255, 255, 255, 255, 255 }; //domino square colors
	static int green3[7] = { 253, 253, 253, 253, 253, 253, 253 };
	static int blue3[7] = { 230, 230, 230, 230, 230, 230, 230 };

	static int red4[7] = { 0, 0, 0, 0, 0, 0, 0 }; //unused
	static int green4[7] = { 0, 0, 0, 0, 0, 0, 0 };
	static int blue4[7] = { 0, 0, 0, 0, 0, 0, 0 };

	static int red2[7] = { 0, 0, 0, 0, 0, 0, 0 }; //domino dot & pen colors
	static int blue2[7] = { 0, 0, 0, 0, 0, 0, 0 };
	static int green2[7] = { 0, 0, 0, 0, 0, 0, 0 };

	static int numeven = 0;


	wstring displayString = L"";


	static int DominoSetLength = 7; //How many dominoes are in the array/set
	Domino DominoSet[] = {	Domino(red[0], red2[0], red3[0], green[0], green2[0], green3[0], blue[0], blue2[0], blue3[0], x,	   y, size, randint[0], randint[1], degrees),
							Domino(red[1], red2[1], red3[1], green[1], green2[1], green3[1], blue[1], blue2[1], blue3[1], x + 150, y, size, randint[2], randint[3], degrees),
							Domino(red[2], red2[2], red3[2], green[2], green2[2], green3[2], blue[2], blue2[2], blue3[2], x + 300, y, size, randint[4], randint[5], degrees),
							Domino(red[3], red2[3], red3[3], green[3], green2[3], green3[3], blue[3], blue2[3], blue3[3], x + 450, y, size, randint[6], randint[7], degrees),
							Domino(red[4], red2[4], red3[4], green[4], green2[4], green3[4], blue[4], blue2[4], blue3[4], x + 600, y, size, randint[8], randint[9], degrees),
							Domino(red[5], red2[5], red3[5], green[5], green2[5], green3[5], blue[5], blue2[5], blue3[5], x + 750, y, size, randint[10], randint[11], degrees),
							Domino(red[6], red2[6], red3[6], green[6], green2[6], green3[6], blue[6], blue2[6], blue3[6], x + 900, y, size, randint[12], randint[13], degrees),
						}; //		red1	red2	red3		green1	green2		green3		blue1	blue2	blue3		cx	   cy  size  squareint1   squareint2	 degrees
						// constructor variables

	switch (msg)
	{
		case WM_PAINT:
			hDC = BeginPaint(hwnd, &Ps);

			hFont = CreateFont(fontHeight, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, L"Times New Roman");


			for (unsigned int i = 0; i < (unsigned)DominoSetLength; i++)
			{
				DominoSet[i].DrawDomino(hDC, degrees); //Draw all the dominos in the set
			}


			for (unsigned int i = 0; i < (unsigned)DominoSetLength; i++)
			{
				numeven += DominoSet[i].CompareRands(); //get the number of even dominos
			}

			rektangle.top = 100;
			rektangle.bottom = 150;
			rektangle.right = 850;
			rektangle.left = 200;

			SelectObject(hDC, hFont);

			displayString = L"There are " + to_wstring(numeven) + L" even dominoes!";
			DrawText(hDC, displayString.c_str(), -1, &rektangle, DT_CENTER | DT_WORDBREAK);

			rektangle2.top = 650;
			rektangle2.bottom = 700;
			rektangle2.right = 680;
			rektangle2.left = 380;
			DrawText(hDC, L"Press R to rotate", -1, &rektangle2, DT_SINGLELINE | DT_CENTER);


			numeven = 0;
			DeleteObject(hFont);
			EndPaint(hwnd, &Ps);
			break;
		case WM_CHAR:
			if (wParam == 'c' || wParam == 'C') //color handling
			{
				for (int i = 0; i < DominoSetLength; i++)
				{
					red[i] = (rand() % 256);
					green[i] = (rand() % 256);
					blue[i] = (rand() % 256);

					red3[i] = (rand() % 128);
					green3[i] = (rand() % 128);
					blue3[i] = (rand() % 128);

					red2[i] = (rand() % 127) + 128; //create light shaded dot 
					blue2[i] = (rand() % 127) + 128;
					green2[i] = (rand() % 127) + 128;
				}

				//for (int i = 0; i < DominoSetLength; i++) //check through each domino for non contrasting colors
				//{
				//	if ((red3[i] < 128) && (green3[i] < 128) && (blue3[i] < 128))
				//	{
				//		red2[i] = 0;	blue2[i] = 0;	green2[i] = 0;

				//		red2[i] = (rand() % 127) + 128; //create light shaded dot 
				//		blue2[i] = (rand() % 127) + 128;
				//		green2[i] = (rand() % 127) + 128;
				//	}
				//	else
				//	{
				//		//otherwise just create dark dots
				//		for (int i = 0; i < DominoSetLength; i++)
				//		{
				//			red2[i] = (rand() % 128);
				//			blue2[i] = (rand() % 128);
				//			green2[i] = (rand() % 128);
				//		}
				//	}
				InvalidateRect(hwnd, NULL, true);

			}
			else if (wParam == 'r' || wParam == 'R')
			{
				degrees += 1;
				InvalidateRect(hwnd, NULL, true);
			}
			break;
		case WM_KEYDOWN:
			if (wParam == VK_DOWN)
			{
				y += 10;
				InvalidateRect(hwnd, NULL, true); //invalidate window
			}
			else if (wParam == VK_UP)
			{
				y -= 10;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == VK_LEFT)
			{
				x -= 10;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == VK_RIGHT)
			{
				x += 10;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == 33)
			{
				size += 6;
				InvalidateRect(hwnd, NULL, true);
			}
			else if (wParam == 34)
			{
				if (size >= 10)
				{
					size -= 6;
					InvalidateRect(hwnd, NULL, true);
				}
			}
			else if (wParam == 32)
			{
				for (int i = 0; i < randintsize; i++)
				{
					randint[i] = rand() % 7;
				}
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
		CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, //default size in pixels
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