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

#ifndef __MYALGORITHM_SENSOR_H_
#define __MYALGORITHM_SENSOR_H_

#include <omnetpp.h>

/**
 * TODO - Generated class
 */
class Sensor: public cSimpleModule {
public:
    double x;
    double y;
    double p; // probability of finding a smartphone in range
    double b; // percentage of battery remaining
    bool isSleep;
    bool isDead;
protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void updateBattery(bool willSend);
    double spd; //seperation distance in meters
    cModule *Grid = getModuleByPath("Grid");
    int height = Grid->par("height");
    int width = Grid->par("width");
    int numOfSensors = Grid->par("numOfSensors");
    int numOfPhones = Grid->par("numOfPhones");
    double range = 10;
    double battery;
    double obattery = 792000;
    double txDuration = 0.00016; //s
    double sleepDuration = 1; //s
    double idleDuration; //s
    double txConsumption = 40; //mA
    double idleConsumption = 16; //mA
    double sleepConsumption = 0.004; //mA
    cMessage *cycle;
    cMessage *frequent;
    cMessage *end;
    int no[10];
    int counter;
    int found;
    int sum;
    int packetCounter;
    int s; // woke up and tried to sent
    int re; //woke up and actually sent
    double ra; // percentage of successful attempts to send
    bool willSend;


};

#endif
