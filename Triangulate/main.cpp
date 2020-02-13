#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <vector>
#include <Polygon.h>
#include <Vector2D.h>
using namespace std;
struct Triangle
{
	CVector2D p0;
	CVector2D p1;
	CVector2D p2;

	Triangle(CVector2D p0_, CVector2D p1_, CVector2D p2_)
	{
		p0 = p0_;
		p1 = p1_;
		p2 = p2_;
	}
};

vector<Triangle> triangulate(vector<CVector2D> polygon)
{
	vector<Triangle> ret;

	vector<CVector2D> polygonClone = polygon;
	int p1;
	int p2;
	int p3;
	while (polygonClone.size() >= 3)
	{
		bool isTriangulation = false;
		for (int i = 0; i < polygonClone.size(); ++i)
		{
			p1 = i;
			p2 = (i + 1) % polygonClone.size();
			p3 = (i + 2) % polygonClone.size();

			if (ccw(polygonClone[p1], polygonClone[p2], polygonClone[p3]))
			{
				bool isInsidePoint = false;
				for (int j = 0; j < polygon.size(); ++j)
				{
					if (ccw(polygonClone[p2] - polygonClone[p1], polygon[j] - polygonClone[p1]) > 0.0 &&
						ccw(polygonClone[p3] - polygonClone[p2], polygon[j] - polygonClone[p2]) > 0.0 &&
						ccw(polygonClone[p1] - polygonClone[p3], polygon[j] - polygonClone[p3]) > 0.0)
					{
						isInsidePoint = true;
						break;
					}
				}

				if (!isInsidePoint)
				{
					ret.push_back(Triangle(polygonClone[p1], polygonClone[p2], polygonClone[p3]));
					polygonClone.erase(polygonClone.begin() + p2);
					isTriangulation = true;
					break;
				}
			}
		}

		if (!isTriangulation)
		{
			printf("»ï°¢È­ ½ÇÆÐ");
			break;
		}
	}

	return ret;
}

int main()
{
	vector<CVector2D> polygon = { 
		{200, 140}, {210, 88}, {287, 115},
	{360, 63}, {382, 141}, {290, 220},
	{142, 190}, {139, 106} };


	vector<Triangle> ret = triangulate(polygon);

	for (int i = 0; i < ret.size(); ++i)
	{
		printf("»ï°¢Çü : p1(%f, %f), p2(%f, %f), p3(%f, %f)\n",
			ret[i].p0.x, ret[i].p0.y, ret[i].p1.x, ret[i].p1.y, ret[i].p2.x, ret[i].p2.y);
	}

	return 0;
}