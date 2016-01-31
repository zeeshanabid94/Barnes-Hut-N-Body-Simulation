#include "quadtree.cpp"

int main() {
	BoundingBox box(-50,-50,50,50);
	QuadTree tree(box);
	Body A(1.0,0.0,0.0, -45.0,35.0);
	Body B(2.0,0.0,0.0,20.0,45.0);
	Body C(3.0,0.0,0.0,5.0,20.0);
	Body D(4.0,0.0,0.0,45.0,25.0);
	Body E(5.0,0.0,0.0,35.0,-40.0);

	tree.insertBody(A);
	cout << "A inserted" << endl;
	tree.insertBody(B);
	cout << "B inserted" << endl;
	tree.insertBody(C);
	cout << "C inserted" << endl;
	tree.insertBody(D);
	cout << "D inserted" << endl;
	tree.insertBody(E);
	cout << "E inserted" << endl;
	tree.print();

}