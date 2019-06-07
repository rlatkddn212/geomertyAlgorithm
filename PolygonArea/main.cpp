#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <Polygon.h>
#include <Vector2D.h>
using namespace std;

int main()
{
	CPolygon polygon;

	double cwPolygon[8][2] = { {139, 106},{142, 190}, {290, 220},{382, 141},{360, 63},{287, 115},{210, 88},{200, 140} };
	double ccwPolygon[8][2] = { {200, 140},{210, 88},{287, 115},{360, 63},{382, 141} , {290, 220},{142, 190},{139, 106} };

	for (int i = 0; i < 8; ++i)
	{
		polygon.pushBack(CVector2D(cwPolygon[i][0], cwPolygon[i][1]));
	}

	double area = polygon.getArea();
	// 폴리곤이 cw인지 ccw인지에 따라 값이 음수가 될 수 있음
	cout << "area : " << area;

	return 0;
}