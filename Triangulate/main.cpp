#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <vector>
#include <Polygon.h>
#include <Vector2D.h>
using namespace std;

const int POLY_SIZE = 8;

struct Triangle
{
	CVector2D _p1;
	CVector2D _p2;
	CVector2D _p3;

	Triangle(CVector2D p1, CVector2D p2, CVector2D p3)
	{
		_p1 = p1;
		_p2 = p2;
		_p3 = p3;
	}
};

// 삼각화 알고리즘 적용, (쉽게 구현하기 위해 polygon을 삭제 시켜서 조금 느리다.)
vector<Triangle> triangulate(CVector2D* polygon, size_t polySize)
{
	vector<Triangle> ret;
	vector<bool> check(polySize, false);
	
	int p1;
	int p2;
	int p3;
	for (size_t i = 0; i < polySize; ++i)
	{
		if (polySize < 3) break;
		p1 = i;
		p2 = (i + 1) % POLY_SIZE;
		p3 = (i + 2) % POLY_SIZE;

		if (ccw(polygon[p1], polygon[p2], polygon[p3]))
		{
			ret.push_back(Triangle(polygon[p1], polygon[p2], polygon[p3]));
			check[p2] = true;
			i = 0;
			
			for (int i = p2; i < polySize - 1; ++i)
			{
				polygon[i] = polygon[i + 1];
			}

			polySize--;
		}
	}

	return ret;
}

int main()
{
	CVector2D polygon[POLY_SIZE] = { {200, 140}, {210, 88}, {287, 115}, {360, 63}, {382, 141}, {290, 220},{142, 190}, {139, 106} };
	//CVector2D polygon[POLY_SIZE] = { {1, 1},{1, 2}, {3, 2}};


	vector<Triangle>  ret = triangulate(polygon, POLY_SIZE);

	for (int i = 0; i < ret.size(); ++i)
	{
		printf("삼각형 : p1(%f, %f), p2(%f, %f), p3(%f, %f)\n", ret[i]._p1.x, ret[i]._p1.y, ret[i]._p2.x, ret[i]._p2.y, ret[i]._p3.x, ret[i]._p3.y);
	}

	return 0;
}