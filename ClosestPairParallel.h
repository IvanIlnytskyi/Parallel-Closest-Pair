#pragma once
#include <math.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <thread>
#include <iostream>
#include <random>
#include <string>
#include <sstream>

struct Point
{
	float x;
	float y;
	bool operator<(const Point& p) const 
	{
		return x < p.x;
	}
};

struct PointPair
{
	Point a;
	Point b;
};

std::string to_string(const PointPair & pp)
{
	std::stringstream ss;
	ss<< "( "<<pp.a.x<<" , "<<pp.a.y<<" ),( "<<pp.b.x << " , " << pp.b.y << " )";
	return ss.str();
}

float points_distance(const Point & a, const Point & b)
{
	return sqrt((a.x - b.x)*(a.x - b.x) + (a.y - b.y)*(a.y - b.y));
}

float boundary_merge(const std::vector<Point> & points, const float left_distance, const float right_distance, 
					const PointPair left_pair,const PointPair right_pair,const float median, PointPair & min_dist_pair)
{
	float min_dist;
	if (left_distance < right_distance)
	{
		min_dist_pair = left_pair;
		min_dist = left_distance;
	}
	else
	{
		min_dist_pair = right_pair;
		min_dist = right_distance;
	}

	std::vector<Point> M(points.size());
	std::vector<Point>::iterator iter = std::copy_if(points.begin(), points.end(), M.begin(), [&](Point p) {return p.x >= median - min_dist && p.x <= median + min_dist; });
	M.resize(std::distance(M.begin(), iter));
	int size = M.size();
	for (int i = 0; i < size; ++i)
	{
		for (int j = 1; j < std::min(8,size-i); ++j)
		{
			if (min_dist > points_distance(M[i], M[i + j]))
			{
				min_dist = points_distance(M[i], M[i + j]);
				min_dist_pair = { M[i], M[i + j] };
			}
		}
	}
	return min_dist;
}
	
std::vector<Point> parallel_closest_pair(const std::vector<Point> &, float &,PointPair &);

void closest_pair_invoker(const std::vector<Point> & P, float & dist, PointPair & pair, std::vector<Point> & P1)
{
	P1 = parallel_closest_pair(P, dist, pair);
}


std::vector<Point> parallel_closest_pair(const std::vector<Point> & P, float & dist, PointPair & result_pair)
{
	if (P.size() < 2)
	{
		dist = FLT_MAX/10;
		return P;
	}
	else
	{

		float median = P[P.size() / 2].x;

		std::vector<Point> L(P.begin(), P.begin() + P.size() / 2 );
		std::vector<Point> R(P.begin() + P.size() / 2, P.end());

		std::vector<Point> L1;
		std::vector<Point> R1;

		float L_dist, R_dist;
		PointPair L_pair, R_pair;

		//closest_pair_invoker(std::ref(L), std::ref(L_dist), std::ref(L1));
		//closest_pair_invoker(std::ref(R), std::ref(R_dist), std::ref(R1));

		
		std::thread t1(closest_pair_invoker, std::ref(L), std::ref(L_dist),std::ref(L_pair), std::ref(L1));
		std::thread t2(closest_pair_invoker, std::ref(R), std::ref(R_dist),std::ref(R_pair), std::ref(R1));
		t1.join();
		t2.join();
	
		std::vector<Point> mergedPoints(L1.size() + R1.size());
		std::merge(L1.begin(), L1.end(), R1.begin(), R1.end(), mergedPoints.begin(), [](Point left, Point right) {return left.y < right.y; });
		dist = boundary_merge(mergedPoints, L_dist, R_dist,L_pair, R_pair,median, result_pair);
		
		return mergedPoints;
	}
}


float sequential_closest_pair(const std::vector<Point> & points,PointPair & pair_points)
{
	float min = FLT_MAX;
	pair_points = { {FLT_MAX,FLT_MAX },{-FLT_MAX,-FLT_MAX } };
	for (int i = 0; i < points.size(); ++i)
		for (int j = i + 1; j < points.size(); ++j)
		{
			float temp_dist = points_distance(points[i], points[j]);
			if (temp_dist < min)
			{
				min = temp_dist;
				pair_points = { points[i],points[j] };
			}
		}
	return min;
}

std::vector<Point> create_test_vector(const size_t size)
{
	std::vector<Point> result_vec(size);
	srand(static_cast <unsigned> (time(0)));
	for (int i = 0; i < size; ++i)
	{
		float x = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) * 1000.0f;
		float y = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX) - 0.5f) * 1000.0f;
		result_vec[i] = { x,y };
	}
	std::sort(result_vec.begin(), result_vec.end());
	return result_vec;
}
