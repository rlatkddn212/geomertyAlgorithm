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

double TRIANGLE_SIZE = 1000.0;
struct Triangle;
struct Node
{
	Node()
	{
		triangle = nullptr;
		p0 = nullptr;
		p1 = nullptr;
		visited = false;
	}

	~Node()
	{
	}

	shared_ptr<Triangle> triangle;

	shared_ptr<Node> p0;
	shared_ptr<Node> p1;

	vector<shared_ptr<Node>> childs;
	bool visited;

	void AddChlid(shared_ptr<Node> newNode)
	{
		childs.push_back(newNode);
	}
};

struct Triangle
{
	CVector2D p0;
	CVector2D p1;
	CVector2D p2;

	weak_ptr<Node> thisNode;
	weak_ptr<Triangle> adj[3];

	Triangle(CVector2D _p0, CVector2D _p1, CVector2D _p2)
	{
		p0 = _p0;
		p1 = _p1;
		p2 = _p2;
	}

	~Triangle()
	{
	}

	void print()
	{
		//printf("�ﰢ�� : p1(%f, %f), p2(%f, %f), p3(%f, %f)\n",
		//	p0.x, p0.y, p1.x, p1.y, p2.x, p2.y);
	}

	bool IsOnPoint(CVector2D p)
	{
		if (p0.Equal(p) || p1.Equal(p) || p2.Equal(p))
		{
			return true;
		}

		return false;
	}

	bool IsPointInTriangle(CVector2D p)
	{
		if ((ccw(p - p0, p1 - p0) <= 0.0) &&
			(ccw(p - p1, p2 - p1) <= 0.0) &&
			(ccw(p - p2, p0 - p2) <= 0.0))
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
		for (size_t i = 0; i < 3; ++i)
		{
			if (triangle == adj[i].lock())
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
			shared_ptr<Triangle> adjTriangle = adj[1].lock();
			if (adjTriangle &&
				(adjTriangle->p0.Equal(p) ||
					adjTriangle->p1.Equal(p) ||
					adjTriangle->p2.Equal(p)))
			{
				printf("Edge ����\n");
			}

			return 1;
		}
		else if (p.Equal(p1))
		{
			shared_ptr<Triangle> adjTriangle = adj[2].lock();
			if (adjTriangle &&
				(adjTriangle->p0.Equal(p) ||
					adjTriangle->p1.Equal(p) ||
					adjTriangle->p2.Equal(p)))
			{
				printf("Edge ����\n");
			}

			return 2;
		}
		else if (p.Equal(p2))
		{
			shared_ptr<Triangle> adjTriangle = adj[0].lock();
			if (adjTriangle &&
				(adjTriangle->p0.Equal(p) ||
					adjTriangle->p1.Equal(p) ||
					adjTriangle->p2.Equal(p)))
			{
				printf("Edge ����\n");
			}

			return 0;
		}

		return -1;
	}

	CVector2D GetSidePoint(int edge)
	{
		switch (edge)
		{
		case 0:
			if (0 != GetSideEdge(p2))
			{
				printf("Point ����\n");
			}

			return p2;
			break;
		case 1:
			if (1 != GetSideEdge(p0))
			{
				printf("Point ����\n");
			}
			return p0;
			break;
		case 2:
			if (2 != GetSideEdge(p1))
			{
				printf("Point ����\n");
			}
			return p1;
			break;

		default:
			break;
		}
	}

	pair<CVector2D, CVector2D> GetEdgePoint(int edge)
	{
		switch (edge)
		{
		case 0:
			return make_pair(p0, p1);
			break;
		case 1:
			return make_pair(p1, p2);
			break;
		case 2:
			return make_pair(p2, p0);
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
			ret[0]->adj[1] = ret[1];
			ret[0]->adj[2] = adj[2];

			for (int i = 0; i < 3; ++i)
			{
				if (adj[2].lock()->adj[i].lock().get() == this)
				{
					adj[2].lock()->adj[i] = ret[0];
					break;
				}
			}

			ret[1]->adj[1] = adj[1];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[1].lock()->adj[i].lock().get() == this)
				{
					adj[1].lock()->adj[i] = ret[1];
					break;
				}
			}
			ret[1]->adj[2] = ret[0];

			break;
		case 1:
			ret.push_back(make_shared<Triangle>(p1, p, p0));
			ret.push_back(make_shared<Triangle>(p, p2, p0));
			ret[0]->adj[1] = ret[1];
			ret[0]->adj[2] = adj[0];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[0].lock()->adj[i].lock().get() == this)
				{
					adj[0].lock()->adj[i].lock() = ret[0];
					break;
				}
			}

			ret[1]->adj[1] = adj[2];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[2].lock()->adj[i].lock().get() == this)
				{
					adj[2].lock()->adj[i] = ret[1];
					break;
				}
			}
			ret[1]->adj[2] = ret[0];

			break;
		case 2:
			ret.push_back(make_shared<Triangle>(p2, p, p1));
			ret.push_back(make_shared<Triangle>(p, p0, p1));
			ret[0]->adj[1] = ret[1];
			ret[0]->adj[2] = adj[1];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[1].lock()->adj[i].lock().get() == this)
				{
					adj[1].lock()->adj[i] = ret[0];
					break;
				}
			}
			ret[1]->adj[1] = adj[0];
			for (int i = 0; i < 3; ++i)
			{
				if (adj[0].lock()->adj[i].lock().get() == this)
				{
					adj[0].lock()->adj[i] = ret[1];
					break;
				}
			}
			ret[1]->adj[2] = ret[0];

			break;
		default:
			break;
		}

		// DAG
		shared_ptr<Node> parentNode = thisNode.lock();

		shared_ptr<Node> newNode0 = make_shared<Node>();
		shared_ptr<Node> newNode1 = make_shared<Node>();

		newNode0->triangle = ret[0];
		newNode1->triangle = ret[1];

		ret[0]->thisNode = newNode0;
		ret[1]->thisNode = newNode1;

		// �и��� �ﰢ���� DAG�� �����Ѵ�.
		parentNode->AddChlid(newNode0);
		parentNode->AddChlid(newNode1);

		return ret;
	}
};

/**
 * ��γ� �ﰢ���� �����ϱ� ���� Ŀ�ٶ� �ﰢ���� �����.
 *
 * @param points ������ ����
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

	return make_shared<Triangle>(CVector2D((minX + maxX) / 2.0, maxY + (maxY - minY) * 100.0 + TRIANGLE_SIZE),
		CVector2D(minX - (maxX - minX) * 100.0 - TRIANGLE_SIZE, minY - (maxY - minY) * 100.0 - TRIANGLE_SIZE),
		CVector2D(maxX + (maxX - minX) * 100.0 + TRIANGLE_SIZE, minY - (maxY - minY) * 100.0 - TRIANGLE_SIZE));
}

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

	}

	shared_ptr<Node> GetNode(shared_ptr<Node> n, const CVector2D& p)
	{
		if (n->childs.empty()) return n;

		for (size_t i = 0; i < n->childs.size(); ++i)
		{
			if (n->childs[i]->triangle->IsPointInTriangle(p))
			{
				shared_ptr<Node> node = GetNode(n->childs[i], p);
				if (node != nullptr)
				{
					return node;
				}
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

		n->triangle.reset();
		for (size_t i = 0; i < n->childs.size(); ++i)
		{
			if (!n->childs[i]->visited)
			{
				n->childs[i]->visited = true;
				GenTriangle(n->childs[i]);
			}
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

	double v = ccw(a, b);

	double adx = triangle->p0.x - point.x, ady = triangle->p0.y - point.y,
		bdx = triangle->p1.x - point.x, bdy = triangle->p1.y - point.y,
		cdx = triangle->p2.x - point.x, cdy = triangle->p2.y - point.y,
		bdxcdy = bdx * cdy, cdxbdy = cdx * bdy,
		cdxady = cdx * ady, adxcdy = adx * cdy,
		adxbdy = adx * bdy, bdxady = bdx * ady,
		alift = adx * adx + ady * ady,
		blift = bdx * bdx + bdy * bdy,
		clift = cdx * cdx + cdy * cdy;

	double det = alift * (bdxcdy - cdxbdy)
		+ blift * (cdxady - adxcdy)
		+ clift * (adxbdy - bdxady);

	if (v > 0) return det >= 0;
	else return det <= 0;
}

void checkAdj(shared_ptr<Triangle> t)
{
	for (int i = 0; i < 3; ++i)
	{
		if (t->adj[i].lock())
		{
			bool isFind = false;
			for (int j = 0; j < 3; ++j)
			{
				if (t->adj[i].lock()->adj[j].lock() == t)
				{
					isFind = true;
				}
			}

			if (!isFind)
			{
				printf("�ﰢ�� ���� ����\n");
				int a = 0;
			}
		}
	}

	t->GetSideEdge(t->p0);
	t->GetSideEdge(t->p1);
	t->GetSideEdge(t->p2);

	t->GetSidePoint(0);
	t->GetSidePoint(1);
	t->GetSidePoint(2);
}

vector<shared_ptr<Triangle>> Flip(shared_ptr<Triangle> t0, int e0, shared_ptr<Triangle> t1, int e1)
{
	CVector2D a = t0->GetSidePoint(e0);
	CVector2D b = t1->GetSidePoint(e1);

	pair<CVector2D, CVector2D> p = t0->GetEdgePoint(e0);

	if (ccw(b - a, p.first - a) <= 0.0)
	{
		CVector2D temp = p.first;
		p.first = p.second;
		p.second = temp;
	}

	vector<shared_ptr<Triangle>> ret;
	ret.push_back(make_shared<Triangle>(a, b, p.first));
	ret.push_back(make_shared<Triangle>(b, a, p.second));

	// ret[0] ��ũ
	ret[0]->adj[0] = ret[1];
	int t1p2Edge = t1->GetSideEdge(p.second);
	if (t1p2Edge != -1)
	{
		ret[0]->adj[1] = t1->adj[t1p2Edge];
		shared_ptr<Triangle> adj = t1->adj[t1p2Edge].lock();
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t1)] = ret[0];
		}
	}

	int t0p2Edge = t0->GetSideEdge(p.second);
	if (t0p2Edge != -1)
	{
		ret[0]->adj[2] = t0->adj[t0p2Edge];
		shared_ptr<Triangle> adj = t0->adj[t0p2Edge].lock();
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t0)] = ret[0];
		}
	}

	// ret[1] ��ũ
	ret[1]->adj[0] = ret[0];
	int t0p1Edge = t0->GetSideEdge(p.first);
	if (t0p1Edge != -1)
	{
		ret[1]->adj[1] = t0->adj[t0p1Edge];
		shared_ptr<Triangle> adj = t0->adj[t0p1Edge].lock();
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t0)] = ret[1];
		}
	}

	int t1p1Edge = t1->GetSideEdge(p.first);
	if (t1p1Edge != -1)
	{
		ret[1]->adj[2] = t1->adj[t1p1Edge];
		shared_ptr<Triangle> adj = t1->adj[t1p1Edge].lock();
		if (adj)
		{
			adj->adj[adj->GetEdgeWithAdj(t1)] = ret[1];
		}
	}

	// DAG
	shared_ptr<Node> parentNode0 = t0->thisNode.lock();
	shared_ptr<Node> parentNode1 = t1->thisNode.lock();

	shared_ptr<Node> newNode0 = make_shared<Node>();
	shared_ptr<Node> newNode1 = make_shared<Node>();

	newNode0->triangle = ret[0];
	newNode1->triangle = ret[1];

	ret[0]->thisNode = newNode0;
	ret[1]->thisNode = newNode1;

	ret[0]->print();
	ret[1]->print();

	checkAdj(ret[0]);
	checkAdj(ret[1]);

	// �и��� �ﰢ���� DAG�� �����Ѵ�.
	parentNode0->AddChlid(newNode0);
	parentNode0->AddChlid(newNode1);
	parentNode1->AddChlid(newNode0);
	parentNode1->AddChlid(newNode1);

	return ret;
}

void LegalizeEdge(CVector2D point, shared_ptr<Triangle> triangle)
{
	int edgeNum = triangle->GetSideEdge(point);
	if (edgeNum == -1)
	{
		return;
	}

	// ������ �ﰢ������ ������ edge�� �����´�.
	shared_ptr<Triangle> adjTriangle = triangle->adj[edgeNum].lock();
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
	if (IsLegal(p, triangle))
	{
		// �ø�
		vector<shared_ptr<Triangle>> newTriangle = Flip(triangle, edgeNum, adjTriangle, adjNum);
		LegalizeEdge(point, newTriangle[0]);
		LegalizeEdge(point, newTriangle[1]);
	}
}

vector<shared_ptr<Triangle>> DelaunayTriangulate(const vector<CVector2D>& points)
{
	// ������ �м��Ͽ� Ŀ�ٶ� �ﰢ���� �����.
	shared_ptr<Triangle> largeTriangle = GetLargeTriangle(points);
	TriangleDAG dag(largeTriangle);

	for (size_t i = 0; i < points.size(); ++i)
	{
		// node ��ġ�� �����;���
		shared_ptr<Node> node = dag.GetNode(points[i]);
		shared_ptr<Triangle> triangle = node->triangle;

		// �ﰢ�� �ȿ� ���� ���
		if (!triangle->IsOnLine(points[i]))
		{
			// �ﰢ���� �����Ѵ�.
			shared_ptr<Triangle> t0 = make_shared<Triangle>(points[i], triangle->p0, triangle->p1);
			shared_ptr<Triangle> t1 = make_shared<Triangle>(points[i], triangle->p1, triangle->p2);
			shared_ptr<Triangle> t2 = make_shared<Triangle>(points[i], triangle->p2, triangle->p0);

			// ���� �ﰢ�� ���� (�ִ� ������ ����)
			t0->adj[0] = t2;
			t0->adj[1] = triangle->adj[0];
			t0->adj[2] = t1;
			shared_ptr<Triangle> adj0 = triangle->adj[0].lock();
			if (adj0)
			{
				adj0->adj[adj0->GetEdgeWithAdj(triangle)] = t0;
			}

			t1->adj[0] = t0;
			t1->adj[1] = triangle->adj[1];
			t1->adj[2] = t2;
			shared_ptr<Triangle> adj1 = triangle->adj[1].lock();
			if (adj1)
			{
				adj1->adj[adj1->GetEdgeWithAdj(triangle)] = t1;
			}

			t2->adj[0] = t1;
			t2->adj[1] = triangle->adj[2];
			t2->adj[2] = t0;
			shared_ptr<Triangle> adj2 = triangle->adj[2].lock();
			if (adj2)
			{
				adj2->adj[adj2->GetEdgeWithAdj(triangle)] = t2;
			}

			// �ﰢ���� DAG�� ����
			shared_ptr<Node> parentNode = triangle->thisNode.lock();

			shared_ptr<Node> newNode0 = make_shared<Node>();
			shared_ptr<Node> newNode1 = make_shared<Node>();
			shared_ptr<Node> newNode2 = make_shared<Node>();

			newNode0->triangle = t0;
			newNode1->triangle = t1;
			newNode2->triangle = t2;

			t0->thisNode = newNode0;
			t1->thisNode = newNode1;
			t2->thisNode = newNode2;

			t0->print();
			t1->print();
			t2->print();

			checkAdj(t0);
			checkAdj(t1);
			checkAdj(t2);

			// �и��� �ﰢ���� DAG�� �����Ѵ�.
			parentNode->AddChlid(newNode0);
			parentNode->AddChlid(newNode1);
			parentNode->AddChlid(newNode2);

			// ��� Edge�� legal�ϰ� �����.
			LegalizeEdge(points[i], t0);
			LegalizeEdge(points[i], t1);
			LegalizeEdge(points[i], t2);
		}
		// edge�� ��ĥ ���
		else
		{
			// ������ edge �����´�.
			int edgeNum = triangle->GetEdge(points[i]);
			// ������ �ﰢ������ ������ edge�� �����´�.
			shared_ptr<Triangle> adjTriangle = triangle->adj[edgeNum].lock();

			// �ﰢ�� ����
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

			t0[0]->print();
			t0[1]->print();
			t1[0]->print();
			t1[1]->print();

			checkAdj(t0[0]);
			checkAdj(t0[1]);
			checkAdj(t1[0]);
			checkAdj(t1[1]);

			LegalizeEdge(points[i], t0[0]);
			LegalizeEdge(points[i], t0[1]);
			LegalizeEdge(points[i], t1[0]);
			LegalizeEdge(points[i], t1[1]);
		}
	}


	vector<shared_ptr<Triangle>> ret = dag.GenTriangle();
	vector<shared_ptr<Triangle>> res;
	for (size_t i = 0; i < ret.size(); ++i)
	{
		if (largeTriangle->IsOnPoint(ret[i]->p0) ||
			largeTriangle->IsOnPoint(ret[i]->p1) ||
			largeTriangle->IsOnPoint(ret[i]->p2))
		{
			continue;
		}

		res.push_back(make_shared<Triangle>(ret[i]->p0, ret[i]->p1, ret[i]->p2));
	}

	ret.clear();
	return res;
}

int main()
{
	vector<CVector2D> points =
	{
		{200, 140}, {210, 88}, {287, 115},
		{360, 63}, {382, 141}, {290, 220},
		{142, 190}, {139, 106}
	};
	/*
	vector<CVector2D> points =
	{
		{200, 100},{210, 100},{205, 100},{205, 150} ,{203, 100}
	};
	*/
	vector<shared_ptr<Triangle>> ret = DelaunayTriangulate(points);

	for (int i = 0; i < ret.size(); ++i)
	{
		printf("�ﰢ�� : p1(%f, %f), p2(%f, %f), p3(%f, %f)\n",
			ret[i]->p0.x, ret[i]->p0.y, ret[i]->p1.x, ret[i]->p1.y, ret[i]->p2.x, ret[i]->p2.y);
	}

	return 0;
}