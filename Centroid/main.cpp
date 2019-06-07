#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <Polygon.h>
#include <Vector2D.h>
using namespace std;

int main()
{
	CPolygon polygon;

	double ccwPolygon[8][2] = { {139, 106},{142, 190}, {290, 220},{382, 141},{360, 63},{287, 115},{210, 88},{200, 140} };
	double cwPolygon[8][2] = { {200, 140},{210, 88},{287, 115},{360, 63},{382, 141} , {290, 220},{142, 190},{139, 106} };

	for (int i = 0; i < 8; ++i)
	{
		polygon.pushBack(CVector2D(cwPolygon[i][0], cwPolygon[i][1]));
	}

	CVector2D centroid = polygon.getCentroid();
	// 폴리곤이 cw인지 ccw인지에 따라 중심값이 달라지니 주의 필요
	cout << "x : "<< centroid.x<< " y : " << centroid.y;

	return 0;
}