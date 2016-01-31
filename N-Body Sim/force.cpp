#include "vector.cpp"

using namespace std;
class Force {
	Vec2D _dir;

	public:
	Force() {
		_dir = Vec2D(0,0);
	}
	Force(double x, double y) {
		_dir = Vec2D(x,y);
	}
	Force(Vec2D Vec) {
		_dir = Vec;
	}

	friend Force operator+(const Force &force1, const Force &force2);
	friend Force operator-(const Force &force1, const Force &force2);

	double getX() const {
		return _dir.getX();
	}

	double getY() const {
		return _dir.getY();
	}

	Vec2D getDir() const {
		return _dir;
	}
};

Force operator+(const Force &force1, const Force &force2) {
	return Force(force1.getDir() + force2.getDir());
}

Force operator-(const Force &force1, const Force &force2) {
	return Force(force1.getDir() - force2.getDir());
}