#include <vector>
#include <cmath>
#include <iostream>

using namespace std;

//Util functions


const double GRAVITYCONST = 0.000000000066742;
double delta_t;
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


class Body {
	double _mass;
	Vec2D _vel;
	vector<Force> _forces;
	Vec2D _pos;

	void addForce(Force f) {
		_forces.push_back(f);
	}
	public:

	Body(double mass, double x_vel, double y_vel, double x_pos, double y_pos) {
		_mass = mass;
		_vel = Vec2D(x_vel, y_vel);
		_pos = Vec2D(x_pos, y_pos);

	}

	double getMass() {
		return _mass;
	}

	double getXVel() {
		return _vel.getX();
	}

	double getYVel() {
		return _vel.getY();
	}

	double getXPos() {
		return _pos.getX();
	}

	double getYPos() {
		return _pos.getY();
	}

	Vec2D getPos() {
		return _pos;
	}

	Vec2D getVel() {
		return _vel;
	}

	void setPos(Vec2D pos) {
		_pos = pos;
	}

	void setVel(Vec2D vel) {
		_vel = vel;
	}

	void calculateNewPosition() {
		Force resultant = getResultantForce();
		Vec2D accel(resultant.getX() / _mass, resultant.getY() / _mass);
		Vec2D newPos = _pos + _vel * delta_t + (accel * 0.5 * pow(delta_t, 2));
		setPos(newPos);
	}

	Force getResultantForce() {
		Force resultant;
		for (int i = 0; i < _forces.size(); i++) {
			resultant = resultant + _forces[i];
		}

		return resultant;
	}

	double calculateForces(vector<Body> &totalBodies) {
		if (!_forces.empty()) {
			_forces.clear();
		}
		
		for (int i = 0; i < totalBodies.size(); i++) {
			Force f = calculateForceFromBody(totalBodies[i]);
			addForce(f);
		}
	}

	Force calculateForceFromBody(Body otherBody) {
		Vec2D distance = calculateDistanceFromBody(otherBody);
		Vec2D unit_distance = distance.getUnitVector();

		double forceMagnitude= (GRAVITYCONST * otherBody.getMass() * getMass()) / distance.getMagnitude();

		double forceMagnitude_x = forceMagnitude * unit_distance.getX();
		double forceMagnitude_y = forceMagnitude * unit_distance.getY();

		return Force(forceMagnitude_x, forceMagnitude_y);
	}

	Vec2D calculateDistanceFromBody(Body otherBody) {
		return otherBody.getPos() - getPos();
	}
};

int main() {
	Body body(1.989, 5, 5, 5, 5);
	delta_t = 1;
	cout << "Velocity " << body.getVel().getX() << " " << body.getVel().getY() << endl;
	body.calculateNewPosition();
	cout << body.getPos().getX() << " " << body.getPos().getY() << endl;
}