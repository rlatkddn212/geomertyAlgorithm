#pragma once
#include <cstring>
#include <iostream>
#include "Point3D.h"

class CMatrix4x4{
private:
	double m_Elem[4][4];
public:
							CMatrix4x4();
							CMatrix4x4(CMatrix4x4& m);

	void					identity();

	void					setxRotationDegrees(double angle); 
	void					setyRotationDegrees(double angle); 
	void					setzRotationDegrees(double angle); 

	void					setScaleDegrees(const CPoint3D& scale);
	void					setMoveDegrees(const CPoint3D& move);

	CMatrix4x4				operator*(const CMatrix4x4& m2) const;
	CPoint3D				operator*(const CPoint3D& p) const;

	double& operator[] (int idx)
	{
		int column = idx / 4;
		int row = idx % 4;
		return m_Elem[column][row];
	}

	friend std::ostream& operator<<(std::ostream& os, const CMatrix4x4& m);
};