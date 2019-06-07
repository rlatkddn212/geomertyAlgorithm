#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <Polygon.h>
#include <Vector2D.h>
using namespace std;

int main()
{
	// A, B, C
	CVector2D triangle[3] = { {314, 80},{130, 285}, {547, 360}};
	double triattr[3] = { 10, 20, 40};

	CVector2D p = {410, 280};

	double areaABC = (triangle[0] - triangle[2]).cross(triangle[1] - triangle[2]) / 2;

	double areaABP = (triangle[0] - p).cross(triangle[1] - p) / 2;
	double areaACP = (triangle[0] - p).cross(triangle[2] - p);
	double areaBCP = (triangle[1] - p).cross(triangle[2] - p);

	double pAttr = (areaABP / areaABC) * triattr[2] + (areaACP / areaABC) * triattr[1] + (areaBCP / areaABC) * triattr[0];

	cout << "점 P의 속성 값은 : " << pAttr;

	return 0;
}