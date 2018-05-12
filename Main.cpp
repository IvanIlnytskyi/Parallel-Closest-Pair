#include "ClosestPairParallel.h"
#include <iostream>

using namespace std;

const float EPSILON = 0.0000001f;

void main()
{
	vector<Point> points = create_test_vector(2048);
	float par_distance;
	auto vec = parallel_closest_pair(points, par_distance);
	float seq_distance = sequential_closest_pair(points);
	cout << "Results:\nSequential algorithm result: " << seq_distance << "." << endl;
	cout << "Parallel algorithm result: " << par_distance << "." << endl;
	cout << "Do algorithms return same results? " << (fabs(seq_distance - par_distance)<EPSILON) << endl;
	system("Pause");
}
