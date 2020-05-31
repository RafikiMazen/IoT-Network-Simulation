//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Smartphone.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Define_Module(Smartphone);

void Smartphone::initialize() {
    WATCH(x);
    WATCH(y);
    WATCH(timenow);
    WATCH(direction);
    xLimit = 100; //boarders
    yLimit = 100;
    /* x = (((double) rand() * (xLimit) / RAND_MAX) - xLimit) * (-1);
     y = (((double) rand() * (yLimit) / RAND_MAX) - yLimit) * (-1);*/
    x = uniform(0, xLimit);
    y = uniform(0, yLimit);
    srand(time(NULL));
    dx = 0;
    dy = 0;
    speed = 10; // m/s
    timenow = simTime().dbl(); // seconds
    direction = 0; // radians
    move = new cMessage("move");
    know = new cMessage("know");
    scheduleAt(simTime() + 1, move);
    //scheduleAt(simTime() + 1.5, know);
}

void Smartphone::handleMessage(cMessage *msg) {

    if (msg == move) {
        y = y + dy;
        x = x + dx;
        // e.g. invoke a computePosition() method
        /*getDisplayString().setTagArg("p", 0, 4 *x);
        getDisplayString().setTagArg("p", 1, 4 *y);*/
        direction = (((double) rand() * (2 * pi) / RAND_MAX) - (2 * pi)) * (-1);
        dx = cos(direction) * speed;
        dy = sin(direction) * speed;
        double ynewPosition = y + dy;
        double xnewPosition = x + dx;
        if (ynewPosition < 0 || ynewPosition > yLimit || xnewPosition < 0
                || xnewPosition > xLimit) {
            direction = reverseAngle(direction);
            dx = cos(direction) * speed;
            dy = sin(direction) * speed;
            ynewPosition = y + dy;
            xnewPosition = x + dx;
        }
        bool w = ynewPosition < 0 || ynewPosition > yLimit || xnewPosition < 0
                || xnewPosition > xLimit;
        while (w == true) {
            direction = (((double) rand() * (2 * pi) / RAND_MAX) - (2 * pi))
                    * (-1);
            dx = cos(direction) * speed;
            dy = sin(direction) * speed;
            ynewPosition = y + dy;
            xnewPosition = x + dx;
            w = ynewPosition < 0 || ynewPosition > yLimit || xnewPosition < 0
                    || xnewPosition > xLimit;
        }

        timenow = simTime().dbl();
        scheduleAt(simTime() + 1, move);
    }
    if (msg == know) {
        //  EV << "x " << getXPosition(simTime().dbl());
        // EV << "y " << getYPosition(simTime().dbl());
    }
    if (strcmp(msg->getName(), "data") == 0) {

        sendDirect(msg->dup(), getModuleByPath("Grid.cloud")->gate("in"));
    }
}
double Smartphone::reverseAngle(double r) {
    r = r + pi;
    if (r > 2 * pi) {
        r = r - 2 * pi;
    }
    return r;
}
double Smartphone::getXPosition(double t) {
    double XPosition;
    double dt = t - timenow;
    double distance = dt * speed;
    XPosition = cos(direction) * distance;
    return x + XPosition;
}
double Smartphone::getYPosition(double t) {
    double YPosition;
    double dt = t - timenow;
    double distance = dt * speed;
    YPosition = sin(direction) * distance;
    return y + YPosition;
}
