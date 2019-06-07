#include "StdAfx.h"
#include "Matrix4x4.h"
#include <algorithm>
#include <math.h>
using namespace std;

ostream& operator<<(ostream& os, const CMatrix4x4& m)
{
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			os << m.m_Elem[i][j]<< " ";
		}

		os << endl;
	}
	return os;
}
CMatrix4x4::CMatrix4x4()
{
	memset(m_Elem, 0 , sizeof(m_Elem));
	m_Elem[0][0] = 1;
	m_Elem[1][1] = 1;
	m_Elem[2][2] = 1;
	m_Elem[3][3] = 1;
}

CMatrix4x4::CMatrix4x4(CMatrix4x4& m)
{
	int idx = 0;
	for(int column = 0; column < 4; column++)
	{
		for(int row = 0; row < 4; row++)
		{
			m_Elem[column][row] = m[idx];
			idx++;
		}
	}
}

void CMatrix4x4::identity()
{
	memset(m_Elem, 0 , sizeof(m_Elem));
	m_Elem[0][0] = 1;
	m_Elem[1][1] = 1;
	m_Elem[2][2] = 1;
	m_Elem[3][3] = 1;
}

void CMatrix4x4::setxRotationDegrees(double angle)
{
	identity();
	m_Elem[1][1] = cos(angle);
	m_Elem[1][2] = -sin(angle);
	m_Elem[2][1] = sin(angle);
	m_Elem[2][2] = cos(angle);

}

void CMatrix4x4::setyRotationDegrees(double angle)
{
	identity();
	m_Elem[0][0] = cos(angle);
	m_Elem[0][2] = sin(angle);
	m_Elem[2][0] = -sin(angle);
	m_Elem[2][2] = cos(angle);

}

void CMatrix4x4::setzRotationDegrees(double angle)
{
	identity();
	m_Elem[0][0] = cos(angle);
	m_Elem[0][1] = -sin(angle);
	m_Elem[1][0] = sin(angle);
	m_Elem[1][1] = cos(angle);

}

void CMatrix4x4::setScaleDegrees(const CPoint3D& scale)
{
	memset(m_Elem, 0 , sizeof(m_Elem));
	m_Elem[0][0] = scale.x;
	m_Elem[1][1] = scale.y;
	m_Elem[2][2] = scale.z;
}

void CMatrix4x4::setMoveDegrees(const CPoint3D& move)
{
	identity();
	m_Elem[0][3] = move.x;
	m_Elem[1][3] = move.y;
	m_Elem[2][3] = move.z;
}

CMatrix4x4 CMatrix4x4::operator*(const CMatrix4x4& m2) const
{
	CMatrix4x4 ret;
	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{ 
			double sum = 0.0; 
			for (int k = 0; k < 4; ++k)
				sum += m_Elem[i][k]*m2.m_Elem[k][j]; 
			ret.m_Elem[i][j] = sum; 
		} 
	}

	return ret;
}

CPoint3D CMatrix4x4::operator*( const CPoint3D& p) const
{
	double	c[4] = { p.x , p.y , p.z, 1.0 };
	double ret[4];
	for (int i=0; i < 4; i++)
	{ 
		double sum = 0.0; 
		for (int j = 0; j < 4; ++j)
			sum += (m_Elem[i][j] * c[j]); 
		ret[i] = sum; 
	}

	return CPoint3D(ret[0],ret[1],ret[2]);
}
