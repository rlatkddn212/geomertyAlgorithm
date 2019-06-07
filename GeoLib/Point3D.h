#pragma once

class CPoint3D
{
public:
	double					x;
	double					y;
	double					z;
	
							CPoint3D(double x_ , double y_ ,double z_):x(x_),y(y_),z(z_){}
	
							CPoint3D();
							~CPoint3D();
};
