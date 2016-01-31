#include <vector>
#include "force.cpp"

using namespace std;
const double GRAVITYCONST = 6.674 * pow(10, -11);
class Body {
	double _mass;
	Vec2D _vel;
	vector<Force> _forces;
	Vec2D _pos;


	public:
	void addForce(Force f) {
		_forces.push_back(f);
	}
	Body() {
		
	}

	Body(double mass, double x_vel, double y_vel, double x_pos, double y_pos) {
		_mass = mass;
		_vel = Vec2D(x_vel, y_vel);
		_pos = Vec2D(x_pos, y_pos);

	}

	Body(const Body &otherBody) {
		_mass = otherBody.getMass();
		_vel = otherBody.getVel();
		_pos = otherBody.getPos();
		_forces = otherBody._forces;
	}

	double getMass() const {
		return _mass;
	}

	double getXVel() const {
		return _vel.getX();
	}

	double getYVel() const {
		return _vel.getY();
	}

	double getXPos() const {
		return _pos.getX();
	}

	double getYPos() const {
		return _pos.getY();
	}

	Vec2D getPos() const {
		return _pos;
	}

	Vec2D getVel() const {
		return _vel;
	}

	void setPos(Vec2D pos) {
		_pos = pos;
	}

	void setVel(Vec2D vel) {
		_vel = vel;
	}

	void calculateNewPosition(double delta_t) {
		Force resultant = getResultantForce();
//		cout << "Resultant Force " << resultant.getDir().getX() << " " << resultant.getDir().getY() << endl;
		
		Vec2D accel(resultant.getX(), resultant.getY());
//		cout << "Acceleration " << accel.getX() << " " << accel.getY() << endl;
		Vec2D newPos = _pos + _vel * delta_t;
		Vec2D newVel = _vel + accel * delta_t;
//		cout << "Position " << newPos.getX() << " " << newPos.getY() << endl;
//		cout << "Velocity " << newVel.getX() << " " << newVel.getY() << endl;
		setPos(newPos);
		setVel(newVel);
	}

	Force getResultantForce() {
		Force resultant(0,0);
		for (int i = 0; i < _forces.size(); i++) {
			resultant = resultant + _forces[i];
		}

		return resultant;
	}

	void calculateForces(vector<Body> &totalBodies) {
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
		        double EPS = 3E4;
		if (distance.getMagnitude() > 0) {
			double forceMagnitude=-1* (GRAVITYCONST * otherBody.getMass()) / (pow(distance.getMagnitude(), 2)+ EPS * EPS);

			double forceMagnitude_x = forceMagnitude * unit_distance.getX();
			double forceMagnitude_y = forceMagnitude * unit_distance.getY();
			return Force(forceMagnitude_x, forceMagnitude_y);
		} else {
			return Force(0,0);
		}
	}

	void clearForceVector() {
		_forces.clear();
	}
	Vec2D calculateDistanceFromBody(Body otherBody) {
		return  getPos()-otherBody.getPos();
	}
};