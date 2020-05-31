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

#ifndef __MYALGORITHM_SMARTPHONE_H_
#define __MYALGORITHM_SMARTPHONE_H_

#include <omnetpp.h>

/**
 * TODO - Generated class
 */
class Smartphone: public cSimpleModule {
public:
    double x;
    double y;
    double getXPosition(double t);
    double getYPosition(double t);
    const double pi = 3.1415926535897;
protected:
    double speed;
    double direction;
    double timenow;
    double xLimit;
    double yLimit;
    double dx;
    double dy;
    cMessage *move;
    cMessage *know;
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    double reverseAngle(double r);

};

#endif
