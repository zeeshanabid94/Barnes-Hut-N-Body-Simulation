#include "body.cpp"
#include <iostream>
using namespace std;

struct BoundingBox {
	double _x;
	double _y;
	double _size;
	BoundingBox() {

	}

	BoundingBox(double x, double y, double size) {
		_x = x;
		_y = y;
		_size = size;
	}

	BoundingBox getQuadrant(int quad) {
		if (quad == 1) {
			return BoundingBox(_x-(_size/4), _y+(_size/4), _size/2);
		} else if (quad == 2) {
			return BoundingBox(_x+(_size/4), _y+(_size/4), _size/2);
		} else if (quad == 3) {
			return BoundingBox(_x-(_size/4), _y-(_size/4), _size/2);
		} else if (quad == 4) {
			return BoundingBox(_x+(_size/4), _y-(_size/4), _size/2);
		} else {
			cout << "Invalid Quadrant" << endl;
		}
	}

	Vec2D getCenter() {
		return Vec2D(_x,_y);
	}

	double getSize() {
		return _size;
	}
};

class TreeNode {

	double _mass;
	Vec2D _centerOfMass;
	Vec2D _position;
	BoundingBox _box;
	TreeNode* _NW;
	TreeNode* _NE;
	TreeNode* _SW;
	TreeNode* _SE;

	

	public:
	TreeNode(BoundingBox box) {
		_mass = 0;
		_centerOfMass = Vec2D();
		//_state = state;
		_NW = _SW = _NE = _SE = NULL;
		_box = box;
		//cout << "NODE CENTER " << box.getCenter().getX() << " " << box.getCenter().getY() << endl;
		_position = _box.getCenter();
	}

	~TreeNode() {
		delete _NW;
		delete _NE;
		delete _SE;
		delete _SW;
	
	}
	int getQuadrantOfBody(Body body) {
		double x = body.getXPos();
		double y = body.getYPos();
		if (x <= _position.getX()) {
			if (y >= _position.getY()) {
				return 1;
			} else {
				return 3;
			}
		} else {
			if (y >= _position.getY()) {
				return 2;
			} else {
				return 4;
			}
		}
	}

	// void updateCenterOfMass(double &mass, Vec2D &centerOfMass) {
	// 	if (!isExternal()) {
	// 	//	cout << "here" << endl;
	// 		double child_mass = 0;
	// 		Vec2D child_com;
	// 		if (_NW != NULL)
	// 			_NW->updateCenterOfMass(child_mass, child_com);
	// 		if (_SW != NULL)
	// 			_SW->updateCenterOfMass(child_mass, child_com);
	// 		if (_NE != NULL)
	// 			_NE->updateCenterOfMass(child_mass, child_com);
	// 		if (_SE != NULL)
	// 			_SE->updateCenterOfMass(child_mass, child_com);
	// 		_mass = child_mass;
	// 		_centerOfMass = child_com/_mass;
	// 		centerOfMass = centerOfMass + child_com*child_mass;
	// 		mass += child_mass;
	// 	} else {
	// 		if (_body != NULL) {
	// 			mass += _body->getMass();
	// 			centerOfMass = centerOfMass+  _body->getPos()*_body->getMass();
	// 		}
	// 	}
	// //	cout << "leaving" << endl;
	// }
	// void updateCenterOfMass(Body body) {
	// 	if (_mass == 0) {
	// 		_mass = body.getMass();
	// 		_centerOfMass = body.getPos();
	// 	} else {
	// 		double new_mass = _mass + body.getMass();
	// 		double new_cen_x = ((_mass*_centerOfMass.getX()) + (body.getMass() * body.getXPos()))/new_mass;
	// 		double new_cen_y = ((_mass*_centerOfMass.getY()) + (body.getMass() * body.getYPos()))/new_mass;
	// 		_centerOfMass = Vec2D(new_cen_x, new_cen_y);
	// 		_mass = new_mass;
	// 	}
	// }

	bool isExternal() {
		return _NW == NULL && _SW == NULL && _NE == NULL && _SE == NULL;
	}

	bool outOfRange(Body body) {
		if (body.getXPos() < _position.getX() - _box.getSize() / 2) {
			return true;
		}
		if (body.getXPos() > _position.getX() + _box.getSize() / 2) {
			return true;
		}
		if (body.getYPos() < _position.getY() - _box.getSize() / 2) {
			return true;
		}		
		if (body.getYPos() > _position.getY() + _box.getSize() / 2) {
			return true;
		}
	}

	bool insertBody(Body body) {
	//cout << "In insert" << endl;

		if (outOfRange(body)) {
			return false;
		}

		if ((_mass == 0 && isExternal())) {
//			cout << "Keeping body here" << endl;
			_mass += body.getMass();
			_centerOfMass = _centerOfMass + body.getPos();
			//_body = new Body(body);
			return true;
		}
	//			updateCenterOfMass(body);

		Body temp(_mass, 0, 0, _centerOfMass.getX(), _centerOfMass.getY());

		if (isExternal()) {
	//		cout << "is external" << endl;
			_mass = 0;
			_centerOfMass = Vec2D(0,0);

		int quad = getQuadrantOfBody(temp);


		if (quad == 1) {
			if (_NW == NULL)
				_NW = new TreeNode(_box.getQuadrant(1));
			_NW->insertBody(temp);
		}
		
		if (quad == 2) {
			if (_NE == NULL)
				_NE = new TreeNode(_box.getQuadrant(2));
			_NE->insertBody(temp);
		}

		if (quad == 3) {
			if (_SW == NULL)
				_SW = new TreeNode(_box.getQuadrant(3));
			_SW->insertBody(temp);
		}

		if (quad == 4) {
		if (_SE == NULL)
				_SE = new TreeNode(_box.getQuadrant(4));
			_SE->insertBody(temp);
		}


	//		cout << "is external 2 " << endl;
		}

		int quad = getQuadrantOfBody(body);

		if (quad == 1) {
			if (_NW == NULL)
				_NW = new TreeNode(_box.getQuadrant(1));
			_NW->insertBody(body);
		}
		
		if (quad == 2) {
			if (_NE == NULL)
				_NE = new TreeNode(_box.getQuadrant(2));
			_NE->insertBody(body);
		}

		if (quad == 3) {
			if (_SW == NULL)
				_SW = new TreeNode(_box.getQuadrant(3));
			_SW->insertBody(body);
		}

		if (quad == 4) {
		if (_SE == NULL)
				_SE = new TreeNode(_box.getQuadrant(4));
			_SE->insertBody(body);
		}



		if (_NW != NULL) {
			_mass += _NW->_mass;
			_centerOfMass = _centerOfMass + _NW->_mass * _NW->_centerOfMass;
		}

		if (_NE != NULL) {
			_mass += _NE->_mass;
			_centerOfMass = _centerOfMass + _NE->_mass * _NE->_centerOfMass;
		}

		if (_SW != NULL) {
			_mass += _SW->_mass;
			_centerOfMass = _centerOfMass + _SW->_mass * _SW->_centerOfMass;
		}

		if (_SE != NULL) {
			_mass += _SE->_mass;
			_centerOfMass = _centerOfMass + _SE->_mass * _SE->_centerOfMass;
		}
		if (_mass != 0) {
		_centerOfMass =  _centerOfMass / _mass;
	}
//		updateCenterOfMass(_mass, _centerOfMass);
	//	cout << "Leaving insert" << endl;
		return false;

	}

	void setNorthWestChild(TreeNode* node) {
		_NW = node;
	}

	void setNorthEastChild(TreeNode* node) {
		_NE = node;
	}

	void setSouthWestChild(TreeNode* node) {
		_SW = node;
	}

	void setSouthEastChild(TreeNode* node) {
		_SE = node;
	}

	TreeNode* getNorthWestChild() {
		return _NE;
	}

	TreeNode* getNorthEastChild() {
		return _NE;
	}

	TreeNode* getSouthWestChild() {
		return _NE;
	}

	TreeNode* getSouthEastChild() {
		return _NE;
	}

	BoundingBox getBoundingBox() {
		return _box;
	}

	Vec2D getCenterOfMass() {
		return _centerOfMass;
	}

	void getBodies(Body &body, double error) {

		double s = _box.getSize();
		double d = (getCenterOfMass() - body.getPos()).getMagnitude();

		double SbyD = s/d;
	//	cout << "S/D: " << SbyD << endl;
		if (!isExternal()) {
			if (SbyD < error) {
				Body overallBody(_mass, 0, 0, getCenterOfMass().getX(), getCenterOfMass().getY());
				body.addForce(body.calculateForceFromBody(overallBody));
				return;
			} else {
				if (_NW != NULL)
					_NW->getBodies(body, error, forceBodies);
				if (_NE != NULL)
					_NE->getBodies(body, error, forceBodies);
				if (_SW != NULL)
					_SW->getBodies(body, error, forceBodies);
				if (_SE != NULL)
					_SE->getBodies(body, error, forceBodies);
			}
		} else {
				Body overallBody(_mass, 0, 0, getCenterOfMass().getX(), getCenterOfMass().getY());
				body.addForce(body.calculateForceFromBody(overallBody));
			//	forceBodies.push_back(*_body);
		}
	//	cout << "Force bodies size: " << forceBodies.size() << endl;
	//	cout << "leaving" << endl;
	}

	// void print() {
	// 	cout << "Entering Print" << endl;
	// 	if (!isExternal()) {
	// 		cout << "Internal" << endl;
	// 				if (_body == NULL) {
	// 		cout << "NO Body here" << endl;
	// 		cout << "Mass of internal node " << _mass << endl;
	// 		cout << "Center of Mass " << _centerOfMass.getX() << " " << _centerOfMass.getY() << endl;
	// 	} else {
	// 		cout << _body->getMass() << endl;
	// 	}
	// 	}
	// 	else {
	// 		cout << "External" << endl;
	// 				if (_body == NULL) {
	// 		cout << "NO Body here" << endl;
	// 	} else {
	// 		cout << _body->getMass() << endl;
	// 	}
	// 	}

	// 	if (_NW != NULL) {
	// 		cout << "Printing NW" << endl;
	// 		_NW->print();
	// 	}
	// 	if (_NE != NULL) {
	// 		cout << "Printing NE" << endl;
	// 		_NE->print();
	// 	}
	// 	if (_SW != NULL) {
	// 		cout << "Printing SW" << endl;
	// 		_SW->print();
	// 	}
	// 	if (_SE != NULL) {
	// 		cout << "Printing SE" << endl;
	// 		_SE->print();
	// 	}
	// 	cout << "Leaving Print" << endl;
	// }

};

class WorkerThread {
	Thread _thread;
	bool _working;
	TreeNode _root;
	Body _body

	virtual void work();

	WorkerThread() {
		_working = false;
		_thread = Thread(work);
		_root = false;
		_body = false;
	}

	void insert(TreeNode root, TreeNode body) {
		if (_working != true) {
			_root = root;
			_body = body;
			_working = true;
		}
	}

	void calculateForce(TreeNode root, TreeNode body) {
		if (_working != true) {
			_root = root;
			_body = body;
			_working = true;
		}
	}

	bool isWorking() {
		return _working;
	}

};

class InsertThread::WorkerThread {

	InsertThread(TreeNode root, Body body)::WorkerThread(root, body) {

	}

	void work() {
		while (true) {
			while (_working) {
				root->insertBody(_body);
			}
		}
	}
};

class ForceThread::WorkerThread {

	double _error;

	ForceThread(TreeNode root, Body body, double error)::WorkerThread(root, body) {
		_error = error;
	}
	
	void work() {
		while (true) {
			while (_working) {
				root->getBodies(_body, error);
			}
		}
	}
};

class QuadTree {
	TreeNode* root;
	InsertThread *it1, *it2;
	ForceThread *ft1, *ft2;

	public:
	QuadTree() {
		root = NULL;
	}
	QuadTree(BoundingBox box) {
		root = new TreeNode(box);
		it1 = new InsertThread();
		it2 = new InsertThread();
	}

	~QuadTree() {
		delete root;
	}

	void insertBody(Body body) {
		if ()

		
	}

	// void print() {
	// 	root->print();
	// }

	TreeNode* getRoot() {
		return root;
	}


	vector<Body> calculateForcesOnBodies(vector<Body> &oldBodies, double error) {
		vector<Body> newBodies;
		// 		double dummy;
		// Vec2D dummy2;
		// root->updateCenterOfMass(dummy, dummy2);
		for (int i = 0; i < oldBodies.size(); i++) {
			Body current (oldBodies[i]);
			vector<Body> forceBodies;
			root->getBodies(current, error);
		//	current.calculateForces(forceBodies);
			newBodies.push_back(current);
		}

		return newBodies;
	}
};