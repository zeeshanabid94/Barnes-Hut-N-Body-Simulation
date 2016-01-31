

//Util functions



class Point {
	double _x;
	double _y;

	public:
	Point(int x, int y) {
		_x = x;
		_y = y;
	}

	double getX() {
		return _x;
	}

	double setX(int x) {
		_x = x;
	}

	double getY() {
		return _y;
	}

	double setY(int _y) {
		_y = y;
	}
};


class Force {
	Point _dir;

	public:
	
	Force(double x, double y) {
		_dir = Point(x,y);
	}

	friend Force operator+(const Force &force1, const Force &force2);

	double getX() {
		return x;
	}

	double getY() {
		return y;
	}
};

class Body {
	double _mass;
	double _x_vel;
	double _y_vel;
	vector<Force> _forces;
	double _x_pos;
	double _y_pos;

	public:

	Body(double mass, double x_vel, double y_vel, double x_pos, double y_pos) {
		_mass = mass;
		_x_vel = x_vel;
		_y_vel = y_vel;
		_x_pos = x_pos;
		_y_pos = y_pos

	}

	double getMass() {
		return _mass;
	}

	double getXVel() {
		return _x_vel;
	}

	double getYVel() {
		return _y_vel;
	}

	double getXPos() {
		return _x_pos;
	}

	double getYPos() {
		return _y_pos;
	}

	Force getResultantForce() {
		Force resultant;
		for (int i = 0; i < _forces.size(); i++) {
			resultant = resultant + _forces[i];
		}

		return resultant;
	}

	double calculateForces() {
		if (!_forces.empty()) {
			_forces.clear();
		}
		
		for (int i = 0; i < totalBodies.size(); i++) {
			Force 
		}
	}

	Force calculateForceFromBody(Body otherBody) {
		double x = otherBody.getXPos() - otherBody.getYPos();
		double y = 
	}
};

