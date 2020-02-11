#pragma comment(lib, "GeoLib.lib")
#include <iostream>
#include <vector>
#include "Vector2D.h"
#include <list>
#include <memory>
#include <math.h>
#include <stdio.h>
#include <algorithm>
using namespace std;

struct Node;
struct Triangle
{
	CVector2D p0;
	CVector2D p1;
	CVector2D p2;

	weak_ptr<Node> thisNode;

	vector<shared_ptr<Triangle>> adj;

	Triangle(CVector2D _p0, CVector2D _p1, CVector2D _p2)
		: adj(3, nullptr)
	{
		p0 = _p0;
		p1 = _p1;
		p2 = _p2;
	}

	bool IsPointInTriangle(CVector2D p)
	{
		if ((ccw(p - p0, p1 - p0) < 0.0) &&
			(ccw(p - p1, p2 - p1) < 0.0) &&
			(ccw(p - p2, p0 - p2) < 0.0))
		{
			return true;
		}

		return false;
	}

	bool IsOnLine(CVector2D p)
	{
		if ((ccw(p - p0, p1 - p0) == 0.0) ||
			(ccw(p - p1, p2 - p1) == 0.0) ||
			(ccw(p - p2, p0 - p2) == 0.0))
		{
			return true;
		}

		return false;
	}

	int GetEdgeWithAdj(shared_ptr<Triangle> triangle)
	{
		for (size_t i = 0; i < adj.size(); ++i)
		{
			if (triangle == adj[i])
			{
				return i;
			}
		}

		return -1;
	}

	int GetEdge(CVector2D p)
	{
		if (ccw(p - p0, p1 - p0) == 0.0)
		{
			return 0;
		}
		else if (ccw(p - p1, p2 - p1) == 0.0)
		{
			return 1;
		}
		else if (ccw(p - p2, p0 - p2) == 0.0)
		{
			return 2;
		}

		return -1;
	}

	int GetSideEdge(CVector2D p)
	{
		if (p.Equal(p0))
		{
			return 1;
		}
		else if (p.Equal(p1))
		{
			return 2;
		}
		else if (p.Equal(p2))
		{
			return 0;
		}

		return -1;
	}

	CVector2D GetSidePoint(int edge)
	{
		switch (edge)
		{
		case 0:
			return p2;
			break;
		case 1:
			return p0;
			break;
		case 2:
			return p1;
			break;

		default:
			break;
		}
	}

	vector<shared_ptr<Triangle>> SplitTriangle(CVector2D p)
	{
		vector<shared_ptr<Triangle>> ret;

		int edge = GetEdge(p);
		
		switch (edge)
		{
		case 0:
			ret.push_back(make_shared<Triangle>(p0, p, p2));
			ret.push_back(make_shared<Triangle>(p, p1, p2));
			ret[0]->adj[1] = ret[1]; ret[0]->adj[2] = adj[2];
			ret[1]->adj[1] = adj[1]; ret[1]->adj[2] = ret[0];
			break;
		case 1:
			ret.push_back(make_shared<Triangle>(p1, p, p0));
			ret.push_back(make_shared<Triangle>(p, p2, p0));
			ret[0]->adj[1] = ret[1]; ret[0]->adj[2] = adj[0];
			ret[1]->adj[1] = adj[2]; ret[1]->adj[2] = ret[0];
			break;
		case 2:
			ret.push_back(make_shared<Triangle>(p2, p, p1));
			ret.push_back(make_shared<Triangle>(p, p0, p1));
			ret[0]->adj[1] = ret[1]; ret[0]->adj[2] = adj[1];
			ret[1]->adj[1] = adj[0]; ret[1]->adj[2] = ret[0]; 
			break;
		default:
			break;
		}

		return ret;
	}
};

/**
 * 들로네 삼각형을 생성하기 위해 커다란 삼각형을 만든다.
 *
 * @param points 점들의 집함
 * @return The large triangle
 */
shared_ptr<Triangle> GetLargeTriangle(const vector<CVector2D>& points)
{
	double minX = points[0].x;
	double maxX = points[0].x;
	
	double minY = points[0].y;
	double maxY = points[0].y;
	
	for (size_t i = 0; i < points.size(); ++i)
	{
		minX = min(points[i].x, minX);
		maxX = max(points[i].x, maxX);
		minY = min(points[i].y, minY);
		maxY = max(points[i].y, maxY);
	}

	return make_shared<Triangle>(CVector2D((minX + maxX) / 2.0, maxY + (maxY - minY) * 100.0),
					CVector2D(minX - (maxX - minX) * 100.0, minY - (maxY - minY) * 100.0),
					CVector2D(maxX + (maxX - minX) * 100.0, minY - (maxY - minY) * 100.0));
}

struct Node
{
	Node()
	{
		triangle = nullptr;
		p0 = nullptr;
		p1 = nullptr;
	}

	shared_ptr<Triangle> triangle;

	shared_ptr<Node> p0;
	shared_ptr<Node> p1;

	vector<shared_ptr<Node>> childs;


	void AddChlid(shared_ptr<Node> newNode)
	{
		childs.push_back(newNode);
	}
};

class TriangleDAG
{
public:
	TriangleDAG(shared_ptr<Triangle> tri)
	{
		root = make_shared<Node>();
		root->triangle = tri;
		tri->thisNode = root;
	}

	~TriangleDAG()
	{
		// Tree를 탐색하면서 동적할당 제거
	}

	shared_ptr<Node> GetNode(shared_ptr<Node> n, const CVector2D& p)
	{
		if (n->childs.empty()) return n;

		for (size_t i = 0; n->childs.size(); ++i)
		{
			if (n->childs[i]->triangle->IsPointInTriangle(p))
			{
				return GetNode(n->childs[i], p);
			}
		}
		
		return nullptr;
	}

	shared_ptr<Node> GetNode(const CVector2D& p)
	{
		return GetNode(root, p);
	}

	void GenTriangle(shared_ptr<Node> n)
	{
		if (n->childs.empty()) 
		{
			ret.push_back(n->triangle);

			return;
		}

		for (size_t i = 0; n->childs.size(); ++i)
		{
			GenTriangle(n->childs[i]);
		}
	}

	vector<shared_ptr<Triangle>> GenTriangle()
	{
		ret.clear();
		GenTriangle(root);

		return ret;
	}

private:
	shared_ptr<Node> root;
	vector<shared_ptr<Triangle>> ret;
};

bool IsLegal(CVector2D point, shared_ptr<Triangle> triangle)
{
	CVector2D a = triangle->p0 - triangle->p2;
	CVector2D b = triangle->p1 - triangle->p2;

	double asq = a.x * a.x + a.y * a.y;
	double bsq = b.x * b.x + b.y * b.y;
	double v = ccw(a, b);

	if (v != 0)
	{
		CVector2D c = triangle->p2;
		CVector2D center(c.x + (b.y * asq - a.y * bsq) / (2.0 * v), 
						c.y + ( -b.x * asq + a.x * bsq) / (2.0 * v));
		double radius = (a - center).Magnitude();
		double dist = (point - center).Magnitude();
		
		return dist >= radius;
	}

	return false;
}

vector<shared_ptr<Triangle>> Flip(shared_ptr<Triangle> t0, int e0, shared_ptr<Triangle> t1, int e1)
{
	// link
	// DAG 추가

	return vector<shared_ptr<Triangle>>();
}

void LegalizeEdge(CVector2D point, shared_ptr<Triangle> triangle)
{
	int edgeNum = triangle->GetSideEdge(point);
	if (edgeNum == -1)
	{
		return;
	}

	// 인접한 삼각형에서 겹쳐진 edge를 가져온다.
	shared_ptr<Triangle> adjTriangle = triangle->adj[edgeNum];
	if (adjTriangle == nullptr)
	{
		return;
	}

	int adjNum = adjTriangle->GetEdgeWithAdj(triangle);
	if (adjNum == -1)
	{
		return;
	}

	CVector2D p = adjTriangle->GetSidePoint(adjNum);
	if (! IsLegal(p, triangle))
	{
		// 플립
		vector<shared_ptr<Triangle>> newTriangle = Flip(triangle, edgeNum, adjTriangle, adjNum);
		
		LegalizeEdge(point, newTriangle[0]);
		LegalizeEdge(point, newTriangle[1]);
	}
}

vector<shared_ptr<Triangle>> DelaunayTriangulate(const vector<CVector2D>& points)
{
	// 점들을 분석하여 커다란 삼각형을 만든다.
	shared_ptr<Triangle> largeTriangle = GetLargeTriangle(points);
	TriangleDAG dag(largeTriangle);

	for (size_t i = 0; i < points.size(); ++i)
	{
		// node 위치를 가져와야함
		shared_ptr<Node> node = dag.GetNode(points[i]);
		shared_ptr<Triangle> triangle = node->triangle;

		// 삼각형 안에 있을 경우
		if (!triangle->IsOnLine(points[i]))
		{
			// 삼각형을 분할한다.
			shared_ptr<Triangle> t0 = make_shared<Triangle>(points[0], triangle->p0, triangle->p1);
			shared_ptr<Triangle> t1 = make_shared<Triangle>(points[0], triangle->p1, triangle->p2);
			shared_ptr<Triangle> t2 = make_shared<Triangle>(points[0], triangle->p2, triangle->p0);

			// 인접 삼각형 설정 (넣는 순서에 주의)
			t0->adj.push_back(t1); t0->adj.push_back(triangle->adj[0]); t0->adj.push_back(t2);
			t1->adj.push_back(t0); t1->adj.push_back(triangle->adj[1]); t1->adj.push_back(t2);
			t2->adj.push_back(t1); t2->adj.push_back(triangle->adj[2]); t2->adj.push_back(t0);

			// 삼각형을 DAG에 연결
			shared_ptr<Node> parentNode = triangle->thisNode.lock();

			shared_ptr<Node> newNode0 = make_shared<Node>();
			shared_ptr<Node> newNode1 = make_shared<Node>();
			shared_ptr<Node> newNode2 = make_shared<Node>();

			newNode0->triangle = t0;
			newNode1->triangle = t1;
			newNode2->triangle = t2;

			// 분리된 삼각형을 DAG에 연결한다.
			parentNode->AddChlid(newNode0);
			parentNode->AddChlid(newNode1);
			parentNode->AddChlid(newNode2);

			// 모든 Edge를 legal하게 만든다.
			LegalizeEdge(points[i], t0);
			LegalizeEdge(points[i], t1);
			LegalizeEdge(points[i], t2);
		}
		// edge에 겹칠 경우
		else
		{
			// 겹쳐진 edge 가져온다.
			int edgeNum = triangle->GetEdge(points[i]);
			// 인접한 삼각형에서 겹쳐진 edge를 가져온다.
			shared_ptr<Triangle> adjTriangle = triangle->adj[edgeNum];

			// 삼각형 생성
			vector<shared_ptr<Triangle>> t0 = triangle->SplitTriangle(points[i]);
			vector<shared_ptr<Triangle>> t1 = adjTriangle->SplitTriangle(points[i]);
			
			if ((t0[0]->p0.Equal(t1[0]->p0) && t0[0]->p1.Equal(t1[0]->p1)) ||
				(t0[0]->p0.Equal(t1[0]->p1) && t0[0]->p1.Equal(t1[0]->p0)))
			{
				t0[0]->adj[0] = t1[0];
				t0[1]->adj[0] = t1[1];

				t1[0]->adj[0] = t0[0];
				t1[1]->adj[0] = t0[1];
			}
			else
			{
				t0[0]->adj[0] = t1[1];
				t0[1]->adj[0] = t1[0];

				t1[0]->adj[0] = t0[1];
				t1[1]->adj[0] = t0[0];
			}
			
			LegalizeEdge(points[i], t0[0]);
			LegalizeEdge(points[i], t0[1]);
			LegalizeEdge(points[i], t1[0]);
			LegalizeEdge(points[i], t1[1]);
		}
	}

	return dag.GenTriangle();
}

int main()
{
	vector<CVector2D> points = 
	{
		{200, 140}, {210, 88}, {287, 115},
		{360, 63}, {382, 141}, {290, 220},
		{142, 190}, {139, 106}
	};

	vector<shared_ptr<Triangle>> ret = DelaunayTriangulate(points);

	for (int i = 0; i < ret.size(); ++i)
	{
		printf("삼각형 : p1(%f, %f), p2(%f, %f), p3(%f, %f)\n",
			ret[i]->p0.x, ret[i]->p0.y, ret[i]->p1.x, ret[i]->p1.y, ret[i]->p2.x, ret[i]->p2.y);
	}

	return 0;
}