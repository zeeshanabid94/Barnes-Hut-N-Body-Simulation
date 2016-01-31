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
			return BoundingBox(_x-(_size/2), _y+(_size/2), _size/2);
		} else if (quad == 2) {
			return BoundingBox(_x+(_size/2), _y+(_size/2), _size/2);
		} else if (quad == 3) {
			return BoundingBox(_x-(_size/2), _y-(_size/2), _size/2);
		} else if (quad == 4) {
			return BoundingBox(_x+(_size/2), _y-(_size/2), _size/2);
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
	Body* _body;
	TreeNode* _parent;
	TreeNode* _NW;
	TreeNode* _NE;
	TreeNode* _SW;
	TreeNode* _SE;

	

	public:
	enum NodeState {
		EMPTY, INTERNAL, EXTERNAL 
	};
	bool visited;
	NodeState _state;
	TreeNode(BoundingBox box, NodeState state) {
		_mass = 0;
		_centerOfMass = Vec2D();
		_parent = NULL;
		_state = state;
		if (state == INTERNAL) {
			_NW = new TreeNode(box.getQuadrant(1), EXTERNAL);
			_NE = new TreeNode(box.getQuadrant(2), EXTERNAL);
			_SW = new TreeNode(box.getQuadrant(3), EXTERNAL);
			_SE = new TreeNode(box.getQuadrant(4), EXTERNAL);
			_SE->setParent(this);
			_NW->setParent(this);
			_NE->setParent(this);
			_SW->setParent(this);
		} else {
			_NW = _SW = _NE = _SE = NULL;
		}
		_body = NULL;
		_box = box;
		//cout << "NODE CENTER " << box.getCenter().getX() << " " << box.getCenter().getY() << endl;
		_position = box.getCenter();
		visited = false;
	}

	~TreeNode() {
		delete _body;
		delete _NW;
		delete _NE;
		delete _SE;
		delete _SW;
	
	}
	int getQuadrantOfBody(Body body) {
		double x = body.getXPos();
		double y = body.getYPos();
		if (x < _position.getX()) {
			if (y > _position.getY()) {
				return 1;
			} else {
				return 3;
			}
		} else {
			if (y > _position.getY()) {
				return 2;
			} else {
				return 4;
			}
		}
	}

	void updateCenterOfMass(double &mass, Vec2D &centerOfMass) {
		if (_state == INTERNAL) {
			double child_mass = 0;
			Vec2D child_com;
			if (_NW != NULL)
				_NW->updateCenterOfMass(child_mass, child_com);
			if (_SW != NULL)
				_SW->updateCenterOfMass(child_mass, child_com);
			if (_NE != NULL)
				_NE->updateCenterOfMass(child_mass, child_com);
			if (_SE != NULL)
				_SE->updateCenterOfMass(child_mass, child_com);
			_mass = child_mass;
			_centerOfMass = child_com/_mass;
			centerOfMass = centerOfMass + child_com*child_mass;
			mass += child_mass;
		} else {
			if (_body != NULL) {
				mass += _body->getMass();
				centerOfMass = centerOfMass+  _body->getPos()*_body->getMass();
			}
		}
	}
	void updateCenterOfMass(Body body) {
		if (_mass == 0) {
			_mass = body.getMass();
			_centerOfMass = body.getPos();
		} else {
			double new_mass = _mass + body.getMass();
			double new_cen_x = ((_mass*_centerOfMass.getX()) + (body.getMass() * body.getXPos()))/new_mass;
			double new_cen_y = ((_mass*_centerOfMass.getY()) + (body.getMass() * body.getYPos()))/new_mass;
			_centerOfMass = Vec2D(new_cen_x, new_cen_y);
			_mass = new_mass;
		}
	}

	bool insertBody(Body body) {
		//cout << "IN INSERT" << endl;
		int quad = getQuadrantOfBody(body);
		if (getState() == INTERNAL) {
			if (quad == 1) {
		//		cout << "going to quad 1" << endl;
				return _NW->insertBody(body);
			} else if (quad == 2) {
		//		cout << "going to quad 2" << endl;
				return _NE->insertBody(body);
			} else if (quad == 3) {
		//		cout << "going to quad 3" << endl;
				return _SW->insertBody(body);
			} else if (quad == 4) {
		//		cout << "going to quad 4" << endl;
				return _SE->insertBody(body);
			} else {
		//		cout << "Invalid quadrant!" << endl;
				return false;
			}
		} else {
			if (_body == NULL){
				_body = new Body(body);
			} else {
				quad = _parent->getQuadrantOfBody(body);
				if (quad == 1) {
		//			cout << "inserting in quad 1" << endl;
					TreeNode* toDelete = _parent->_NW;
					_parent->_NW = new TreeNode(_box, INTERNAL);
					_parent->_NW->setParent(_parent);
					_parent->_NW->insertBody(*_body);
					_parent->_NW->insertBody(body);
					delete toDelete;
					return true;
				} else if (quad == 2) {
		//			cout << "inserting in quad 2" << endl;
					TreeNode* toDelete = _parent->_NE;
					_parent->_NE = new TreeNode(_box, INTERNAL);
					_parent->_NE->setParent(_parent);
					_parent->_NE->insertBody(*_body);
					_parent->_NE->insertBody(body);
					delete toDelete;
					return true;
				} else if (quad == 3) {
		//			cout << "inserting in quad 3" << endl;
					TreeNode* toDelete = _parent->_SW;
					_parent->_SW = new TreeNode(_box, INTERNAL);
					_parent->_SW->setParent(_parent);
					_parent->_SW->insertBody(*_body);
					_parent->_SW->insertBody(body);
					delete toDelete;
					return true;
				} else if (quad == 4) {
		//			cout << "inserting in quad 4" << endl;
					TreeNode* toDelete = _parent->_SE;
					_parent->_SE = new TreeNode(_box, INTERNAL);
					_parent->_SE->setParent(_parent);
					_parent->_SE->insertBody(*_body);
					_parent->_SE->insertBody(body);
					delete toDelete;
					return true;
				} else {
		//			cout << "Invalid quadrant!" << endl;
					return false;
				}
			}
		}

//			updateCenterOfMass(body);
	//	cout << "LEAVING INSERT" << endl;
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

	void setState(NodeState state) {
		_state = state;
	}

	void setBody(Body body) {
		_body = new Body(body);
	}

	void setParent(TreeNode* parent) {
		_parent = parent;
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

	NodeState getState() {
		return _state;
	}

	Body* getBody() {
		return _body;
	}

	BoundingBox getBoundingBox() {
		return _box;
	}

	Vec2D getCenterOfMass() {
		return _centerOfMass;
	}

	void getBodies(Body &body, double error, vector<Body> &forceBodies) {

		if (_mass == 0 && _body == NULL)
			return;
		double s = _box.getSize();
		double d = (_centerOfMass - body.getPos()).getMagnitude();

		double SbyD = s/d;
	//	cout << "S/D: " << SbyD << endl;
		if (_state == INTERNAL) {
			if (SbyD < error) {
				Body overallBody(_mass, 0, 0, _centerOfMass.getX(), _centerOfMass.getY());
				forceBodies.push_back(overallBody);
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
			if (_body != NULL) {
				forceBodies.push_back(*_body);
			}
		}
	}

	void print() {
		cout << "Entering Print" << endl;
		if (_state == INTERNAL) {
			cout << "Internal" << endl;
					if (_body == NULL) {
			cout << "NO Body here" << endl;
			cout << "Mass of internal node " << _mass << endl;
			cout << "Center of Mass " << _centerOfMass.getX() << " " << _centerOfMass.getY() << endl;
		} else {
			cout << _body->getMass() << endl;
		}
		}
		else {
			cout << "External" << endl;
					if (_body == NULL) {
			cout << "NO Body here" << endl;
		} else {
			cout << _body->getMass() << endl;
		}
		}

		if (_NW != NULL) {
			cout << "Printing NW" << endl;
			_NW->print();
		}
		if (_NE != NULL) {
			cout << "Printing NE" << endl;
			_NE->print();
		}
		if (_SW != NULL) {
			cout << "Printing SW" << endl;
			_SW->print();
		}
		if (_SE != NULL) {
			cout << "Printing SE" << endl;
			_SE->print();
		}
		cout << "Leaving Print" << endl;
	}

};

class QuadTree {
	TreeNode* root;

	public:
	QuadTree() {
		cout << "Creating tree" << endl;
		root = NULL;
		cout << "Tree created" << endl;
	}
	QuadTree(BoundingBox box) {
		cout << "Creating tree" << endl;
		root = new TreeNode(box, TreeNode::INTERNAL);
		cout << "Tree created" << endl;
	}

	~QuadTree() {
		delete root;
	}

	void insertBody(Body body) {
		root->insertBody(body);
		double dummy;
		Vec2D dummy2;
		root->updateCenterOfMass(dummy, dummy2);
	}

	void print() {
		root->print();
	}

	TreeNode* getRoot() {
		return root;
	}


	vector<Body> calculateForcesOnBodies(vector<Body> &oldBodies, double error) {
		vector<Body> newBodies;

		for (int i = 0; i < oldBodies.size(); i++) {
			Body current = oldBodies[i];
			vector<Body> forceBodies;
			root->getBodies(current, error, forceBodies);
			current.calculateForces(forceBodies);
			newBodies.push_back(current);
		}

		return newBodies;
	}
};