#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <Polygon.h>
#include <Vector2D.h>
using namespace std;

const int POLY_SIZE = 8;

int main()
{
	CVector2D polygon[POLY_SIZE] = { {139, 106},{142, 190}, {290, 220},{382, 141},{360, 63},{287, 115},{210, 88},{200, 140} };

	int p1;
	int p2;
	int p3;

	bool isConvex = true;
	for (int i = 0; i < POLY_SIZE; ++i)
	{
		p1 = i;
		p2 = (i + 1) % POLY_SIZE;
		p3 = (i + 2) % POLY_SIZE;

		if (ccw(polygon[p1], polygon[p2], polygon[p3]))
		{
			isConvex = false;
		}
	}

	if (isConvex)
	{
		cout << "º¼·Ï Æú¸®°ï";
	}
	else
	{
		cout << "¿À¸ñ Æú¸®°ï";
	}

	return 0;
}