#include <Vector2D.h>
#include <iostream>
using namespace std;
bool pointByIntersectSegment(CVector2D& p1, CVector2D& p2, CVector2D& p3, CVector2D& p4, CVector2D* cp)
{
	double parent = (p4.y - p3.y) * (p2.x- p1.x) - (p4.x - p3.x) * (p2.y - p1.y);

	if (parent == 0) return false;

	double c1 = (p4.x - p3.x) * (p1.y - p3.y) - (p4.y - p3.y) * (p1.x - p3.x);
	double c2 = (p2.x - p1.x) * (p1.y - p3.y) - (p2.y - p1.y) * (p1.x - p3.x);
	double t = (double)c1 / parent;
	double s = (double)c2 / parent;

	//선분에 포함되지 않는 경우
	if (t <= 0.0 || t >= 1.0 || s <= 0.0 || s >= 1.0) return false;
	if (c1 == 0 && c2 == 0) return false;

	cp->x = p1.x + t * (double)(p2.x - p1.x);
	cp->y = p1.y + t * (double)(p2.y - p1.y);

	return true;
}

int main()
{
	CVector2D lineA_Pos01(156,259);
	CVector2D lineA_Pos02(536,244);

	CVector2D lineB_Pos01(517,137);
	CVector2D lineB_Pos02(150, 362);

	CVector2D point;
	if (pointByIntersectSegment(lineA_Pos01, lineA_Pos02, lineB_Pos01, lineB_Pos02, &point))
	{
		cout << "intersect : x : " << point.x << " y : " << point.y;
	}
	else
	{
		cout << "not intersect";
	}


	return 0;
}