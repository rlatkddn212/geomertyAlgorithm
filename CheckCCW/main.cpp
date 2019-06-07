#include <iostream>
#include <Vector2D.h>
using namespace std;

int main()
{
	CVector2D p1(1.0, 1.0);
	CVector2D p2(1.0, 2.0);
	CVector2D p3(0.0, 2.0);
	
	bool ret = ccw(p1, p2, p3);

	if (ret)
	{
		cout << "CCW";
	}
	else
	{
		cout << "CW";
	}

	return 0;
}