#include "body.cpp"
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

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
	// cout << "In insert" << endl;

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
					_NW->getBodies(body, error);
				if (_NE != NULL)
					_NE->getBodies(body, error);
				if (_SW != NULL)
					_SW->getBodies(body, error);
				if (_SE != NULL)
					_SE->getBodies(body, error);
			}
		} else {
				Body overallBody(_mass, 0, 0, getCenterOfMass().getX(), getCenterOfMass().getY());
				body.addForce(body.calculateForceFromBody(overallBody));
			//	forceBodies.push_back(*_body);
		}
	}


};

class Job {

	public:
	enum Status {
		NONE,READY,DONE, FAILED
	};
	Status _status;
	Job() {
		_status = NONE;
	};

	~Job() {

	}

	void setStatus(Status newStatus) {
		_status = newStatus;
	}
	virtual void run() = 0;
	virtual Status getStatus() {
		return _status;
	}

};

class InsertJob: public Job {
	TreeNode* _root;
	Body _body;
	bool _result;

	public:
	InsertJob(TreeNode* root, Body body):Job() {
		_root = root;
		_body = body;
	}

	void run() {
		
		_result = _root->insertBody(_body);
	//	if (_result == true)
			_status = Job::DONE;
	//	else
	//		_status = Job::FAILED;
	}

};

class ForceCalculation: public Job {
	TreeNode* _root;
	Body _body;
	Force _result;
	double _error;

	public:
	ForceCalculation(TreeNode* root, Body body, double error):Job() {
		_root = root;
		_body = body;
		_error = error;
	}

	void run() {
		_root->getBodies(_body, _error);
		_result = _body.getResultantForce();
		_status = Job::DONE;
	}

	Force getResult() {
		return _result;
	}
};



class QuadTreeThread {
	Job* _job;
	thread _thread;
	mutex _mtx;
	condition_variable _noWork;
	bool _running;
public:
	QuadTreeThread() {
		_running = true;
		_thread = thread(&QuadTreeThread::running, this);
		_job = NULL;
	}

	~QuadTreeThread() {
		_running = false;
		_noWork.notify_one();
	}

	void running() {
		unique_lock<mutex> lck(_mtx);
		while (_running) {		
			while (_job == NULL && _running == true) {
				_noWork.wait(lck);
			}
			if (_running != false) {
				_job->run();
				_job = NULL;
			}
		}
	}

	void detach() {
		_thread.detach();
	}

	void WakeUpThread() {
		_noWork.notify_one();
	}

	void setJob(Job* job) {
		_mtx.lock();
		job->setStatus(Job::READY);
		_job = job;
		_mtx.unlock();
	}

	bool isIdle() {
		return (_job == NULL);
	}

	bool Error() {
		return _job->getStatus() == Job::FAILED;
	}
};

class ThreadedQuadTree {
	vector<Job> _jobQueue;
	QuadTreeThread* thread0;
	QuadTreeThread* thread1;
	QuadTreeThread* thread2;
	QuadTreeThread* thread3;
	TreeNode* _root0;
	TreeNode* _root1;
	TreeNode* _root2;
	TreeNode* _root3;
	int _nextInsert;


	public:
	ThreadedQuadTree(BoundingBox box) {
		cout << "creating threaded tree" << endl;
		thread0 = new QuadTreeThread();
		thread1 = new QuadTreeThread();
		thread2 = new QuadTreeThread();
		thread3 = new QuadTreeThread();
		_root0 = new TreeNode(box);
		_root1 = new TreeNode(box);
		_root2 = new TreeNode(box);
		_root3 = new TreeNode(box);
		_nextInsert = 0;
		cout << "Tree Created" << endl;
	}

	~ThreadedQuadTree() {
		thread0->detach();
		thread1->detach();
		thread2->detach();
		thread3->detach();
		delete _root0;
		delete _root1;
		delete _root2;
		delete _root3;
		delete thread0;
		delete thread1;
		delete thread2;
		delete thread3;
	}

	void insertBody(Body body) {
		if (_nextInsert == 0) {
			Job* job =  new InsertJob(_root0, body);
			thread0->setJob(job);
			thread0->WakeUpThread();
			_nextInsert +=1;
			while (!isIdle());
			delete job;
		} else if (_nextInsert == 1) {
			Job* job =  new InsertJob(_root1, body);
			thread1->setJob(job);
			thread1->WakeUpThread();
			_nextInsert +=1;
			while (!isIdle());
			delete job;
		} else if (_nextInsert == 2) {
			Job* job =  new InsertJob(_root2, body);
			thread2->setJob(job);
			thread2->WakeUpThread();
			_nextInsert +=1;
			while (!isIdle());
			delete job;
		} else {
			Job* job =  new InsertJob(_root3, body);
			thread3->setJob(job);
			thread3->WakeUpThread();
			_nextInsert = 0;
			while (!isIdle());
			delete job;
		}
	}

	bool isIdle() {
		return thread0->isIdle() && thread1->isIdle() &&
				thread2->isIdle() && thread3->isIdle();
	}

	bool Error() {
		return thread0->Error() && thread1->Error() &&
				thread2->Error() && thread3->Error();
	}

	// void print() {
	// 	root->print();
	// }


	vector<Body> calculateForcesOnBodies(vector<Body> &oldBodies, double error) {
		vector<Body> newBodies;
		for (int i = 0; i < oldBodies.size(); i++) {
			Body current (oldBodies[i]);
			Job* _force0 = new  ForceCalculation(_root0, current, error);
			Job* _force1 = new  ForceCalculation(_root1, current, error);
			Job* _force2 = new  ForceCalculation(_root2, current, error);
			Job* _force3 = new  ForceCalculation(_root3, current, error);
			thread0->setJob(_force0);
			thread1->setJob(_force1);
			thread2->setJob(_force2);
			thread3->setJob(_force3);
			thread0->WakeUpThread();
			thread1->WakeUpThread();
			thread2->WakeUpThread();
			thread3->WakeUpThread();
			while (!isIdle());
			current.addForce(((ForceCalculation*)_force0)->getResult() + 
							((ForceCalculation*)_force1)->getResult() + 
							((ForceCalculation*)_force2)->getResult() + 
							((ForceCalculation*)_force3)->getResult());
			newBodies.push_back(current);
			delete _force0, _force1, _force2, _force3;
		}

		return newBodies;
	}
};