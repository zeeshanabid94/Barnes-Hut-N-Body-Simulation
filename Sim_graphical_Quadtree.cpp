#include <vector>
#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <fstream>
#include <string.h>
#include <cstddef>
#include <thread>

#include "threadedquadtree.cpp"
using namespace std;

//Util functions


class Universe {
	double _delta_t;
	vector<Body> _oldBodies;
	ThreadedQuadTree* _tree;
	BoundingBox _box;
	vector<Body> _newBodies;

public:
	Universe(double delta_t, string filename) {
		cout << "Parsing file" << endl;
		_delta_t = delta_t;
		_oldBodies = vector<Body>();
		_newBodies = vector<Body>();
		fstream file(filename.c_str());
		double size = 0;
		file.ignore(256, '\n');
		file >> size;
		_box = BoundingBox(0, 0, 2*size);
		_tree = new ThreadedQuadTree(_box);

		 while (!file.eof()) {
			string mass, x_vel, y_vel, x_pos, y_pos, dummy1, dummy2, dummy3;

			file >> x_pos >> y_pos >> x_vel >> y_vel >> mass >> dummy1 >> dummy2 >> dummy3;
			if (mass == "")
				break;
			cout << mass << " " << x_pos << " " << y_pos<< " "  << x_vel<< " "  << y_vel << endl;
		 	Body newbody(stod(mass), stod(x_vel), stod(y_vel), stod(x_pos), stod(y_pos));
		 	_oldBodies.push_back(newbody);
		 	cout << "Inserting into Tree" << endl;
		 	_tree->insertBody(newbody);
		 	cout << "Waiting for insert" << endl;
		 	while (!_tree->isIdle());
		 }
	}

	void nextIteration() {
	cout << "calculating next iteration" << endl;
	time_t start = time(0);
	_newBodies = _tree->calculateForcesOnBodies(_oldBodies, 0.4);
	for (int i = 0; i < _newBodies.size(); i++) {
		Body body;
		body = _newBodies[i];
		body.calculateNewPosition(_delta_t);
		body.clearForceVector();
		_newBodies[i] = body;
	}
	time_t end = time(0);
	double timediff = difftime(end,start) * 1000;

	cout << "Time Taken: " << timediff << endl;
	_oldBodies.clear();
	_oldBodies = _newBodies;

	_newBodies.clear();
	BoundingBox box(0, 0,_box.getSize());
	ThreadedQuadTree* newtree =  new ThreadedQuadTree(_box);
	for (int i = 0; i < _oldBodies.size(); i++) {
		newtree->insertBody(_oldBodies[i]);
		while (!_tree->isIdle());
	}
	cout << "EQUATING QUAD TREES" << endl;
	delete _tree;
	_tree = newtree;
	}

	vector<Body> getBodies() {
		return _oldBodies;
	}

	void addBody(Body B) {
		_oldBodies.push_back(B);
	}
};

sf::RenderWindow* window;
Universe* universe;



void initGraphicWindow() {
	// create the window
    window = new sf::RenderWindow (sf::VideoMode(1024, 768), "My window");
}

void drawBodies(vector<Body> totalBodies) {
	for (int i = 0; i < totalBodies.size(); i++) {
		// draw everything here...
        // window.draw(...);
        sf::CircleShape shape(1);
		shape.setFillColor(sf::Color(150, 50, 250));
		shape.setPosition(totalBodies[i].getPos().getX()/20000 + 512, totalBodies[i].getPos().getY()/20000 + 384);
		//cout << totalBodies[i].getXPos() << " " << totalBodies[i].getYPos() << endl;
		window->draw(shape);
	}
}



void renderingThread(sf::RenderWindow* window) {

    }

int main()
{
	initGraphicWindow();
	universe =  new Universe(0.1, "input.txt");
	// totalBodies = vector<Body>();
	// Body body(5.972*pow(10,24), 0, -30*pow(10,3), 149.6*pow(10,9), 0);
	// totalBodies.push_back(body);
	// Body body2(1.98*pow(10,30), 0, 0, 0, 0);
	// totalBodies.push_back(body2);
	// int iterations = 0;
 //    // run the program as long as the window is open


  while (window->isOpen())
      {
      	 // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window->pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window->close();
        }

        // clear the window with black color
        window->clear(sf::Color::Black);


	
        drawBodies(universe->getBodies());

        // end the current frame
        window->display();

		universe->nextIteration();
        }


    return 0;
}