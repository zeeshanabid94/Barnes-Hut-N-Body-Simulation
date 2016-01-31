#include <iostream>

using namespace std;

int main() {

	cout << "Hello....";

	#pragma omp parallel
	{
		cout << "World";
	}

	cout << endl;
	return 0;
}