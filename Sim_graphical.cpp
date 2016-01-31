#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string.h>
#include <cstddef>
#include <ctime>
#include <getopt.h>

#include "body.cpp"
using namespace std;

//Util functions

sf::RenderWindow* window;


class Universe {
	double _delta_t;
	vector<Body> _oldBodies;
	vector<Body> _newBodies;
	int iterations;

public:
	Universe(string filename) {
		fstream file(filename.c_str());
		file >> _delta_t;
		file >> iterations;
		_oldBodies = vector<Body>();
		_newBodies = vector<Body>();
		 while (!file.eof()) {
			string mass, x_vel, y_vel, x_pos, y_pos;

			file >> mass >> x_pos >> y_pos >> x_vel >> y_vel;
			if (mass == "")
				break;
			cout << mass << " " << x_pos << " " << y_pos<< " "  << x_vel<< " "  << y_vel << endl;
		 	Body newbody(stod(mass), stod(x_vel), stod(y_vel), stod(x_pos), stod(y_pos));
		 	_oldBodies.push_back(newbody);
		 }
	}

	void nextIteration() {
	time_t start = time(0);
	for (int i = 0; i < _oldBodies.size(); i++) {
		Body newBody = _oldBodies[i];
		newBody.calculateForces(_oldBodies);
		newBody.calculateNewPosition(_delta_t);
		_newBodies.push_back(newBody);
	}
	time_t end = time(0);
	double timediff = difftime(end,start) * 1000;

	_oldBodies.clear();
	_oldBodies = _newBodies;
	_newBodies.clear();

	
	cout << "Time Taken: " << timediff << endl;
	}

	vector<Body> getBodies() {
		return _oldBodies;
	}

	void addBody(Body B) {
		_oldBodies.push_back(B);
	}

	void runSim() {
		for (int i = 0; i < iterations; i++) {
			cout << "Iterations " << i << endl;
			for (int j = 0; j < _oldBodies.size(); j++) {
				Body b = _oldBodies[i];
				cout << b.getMass() << " " << b.getXPos() << " " << b.getYPos() << " " << b.getXVel() << " " << b.getYVel() << endl;
			}
			nextIteration();
		}
	}
};


int main()
{
	Universe universe ("input.txt");

	universe.runSim();

    return 0;
}