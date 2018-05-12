#include "ClosestPairParallel.h"
#include <iostream>

using namespace std;

const float EPSILON = 0.0000001f;

void main()
{
	vector<Point> points = create_test_vector(2048);
	float par_distance;
	PointPair par_pair;
	PointPair seq_pair;
	auto vec = parallel_closest_pair(points, par_distance,par_pair);
	float seq_distance = sequential_closest_pair(points,seq_pair);
	cout << "Results:\n1)Sequential algorithm result: \n\ta) min_distance" <<"\n\t\t" <<seq_distance << "; \n\tb) pair of points: "<<"\n\t\t" <<to_string(par_pair) << endl;
	cout << "2)Parallel algorithm result: \n\ta) min_distance" << "\n\t\t"<< par_distance << "; \n\tb) pair of points: " << "\n\t\t"<< to_string(par_pair)<< endl;
	cout << "Do algorithms return same results? " << (fabs(seq_distance - par_distance)<EPSILON) << endl;
	system("Pause");
}
