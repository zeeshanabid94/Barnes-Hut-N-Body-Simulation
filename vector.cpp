#include <cmath>
#include <cstddef>

using namespace std;
class Vec2D {
	double _x;
	double _y;

	public:
	Vec2D() {
		_x = 0;
		_y = 0;
	}

	Vec2D(double x, double y) {
		_x = x;
		_y = y;
	}

	double getX() const {
		return _x;
	}

	void setX(double x) {
		_x = x;
	}

	double getY() const {
		return _y;
	}

	void setY(double y) {
		_y = y;
	}

	double getMagnitude() const {
		return sqrt(pow(_x,2) + pow(_y,2));
	}

	Vec2D getUnitVector() const {
		double x_unit = _x/getMagnitude();
		double y_unit = _y/getMagnitude();

		return Vec2D(x_unit, y_unit);
	}

	Vec2D getTriangleVec(Vec2D Vec1, Vec2D Vec2) const {
		return (Vec1 - Vec2);
	}

	friend Vec2D operator+(const Vec2D &Vec1, const Vec2D &Vec2);
	friend Vec2D operator-(const Vec2D &Vec1, const Vec2D &Vec2);
	friend Vec2D operator*(const Vec2D &Vec, const double scalar);
	friend Vec2D operator*(const double scalar, const Vec2D &Vec);
	friend Vec2D operator/(const Vec2D &Vec, const double scalar);
};

Vec2D operator+(const Vec2D &Vec1, const Vec2D &Vec2) {
	return Vec2D(Vec1.getX() + Vec2.getX(), Vec1.getY() + Vec2.getY());
}

Vec2D operator-(const Vec2D &Vec1, const Vec2D &Vec2) {
	return Vec2D(Vec1.getX() - Vec2.getX(), Vec1.getY() - Vec2.getY());
}

Vec2D operator*(const Vec2D &Vec, const double scalar) {
	return Vec2D(Vec.getX() * scalar, Vec.getY() * scalar);
}

Vec2D operator*(const double scalar, const Vec2D &Vec) {
	return Vec2D(Vec.getX() * scalar, Vec.getY() * scalar);
}

Vec2D operator/(const Vec2D &Vec, const double scalar) {
	if (scalar == 0) {
		cout << "ERROR" << endl;
	}
	return Vec2D(Vec.getX() / scalar, Vec.getY() / scalar);
}