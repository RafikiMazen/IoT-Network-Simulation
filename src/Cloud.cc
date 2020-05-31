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

#include "Cloud.h"
#include "Sensor.h"
#include "string"
#include "strings.h"
#include <cstdlib>
#include <ctime>
#include <iostream>

Define_Module(Cloud);

void Cloud::initialize() {
    /*double xLimit = 100;
    srand(time(NULL));
    for (int i = 0; i < numOfSensors; i++) {
        xs[i] = (((double) rand() * (xLimit) / RAND_MAX) - xLimit) * (-1);
        ys[i] = (((double) rand() * (xLimit) / RAND_MAX) - xLimit) * (-1);
        EV << xs[i] << "  ";
    }*/
    determine = new cMessage("determine");
    scheduleAt(0.91, determine);
}

void Cloud::handleMessage(cMessage *msg) {
    double z[numOfSensors];
    double m;
    if (msg == determine) {

        for (int i = 0; i < numOfSensors; i++) {
            std::string s = std::to_string(i);
            std::string path = "Grid.sensors[" + s + "]";
            const char *cstr = path.c_str();
            Sensor *sensor = check_and_cast<Sensor *>(getModuleByPath(cstr));
            if (sensor->isDead == true)
                z[i] = -1;
            else
                z[i] = (sensor->b) * (sensor->p);
            EV << z[i] << "  ";
        }
        EV << " \n";
        //finding the top of the list
        int position;
        for (int j = 0; j < top; j++) {
            m = -1;
            for (int i = 0; i < numOfSensors; i++) {
                if (z[i] > m) {
                    m = z[i];
                    position = i;
                }
            }
            z[position] = -1;
            max[j] = position;
            EV << max[j] << "  ";
        }
        //setting all to sleep
        for (int i = 0; i < numOfSensors; i++) {
            std::string s = std::to_string(i);
            std::string path = "Grid.sensors[" + s + "]";
            const char *cstr = path.c_str();
            Sensor *sensor = check_and_cast<Sensor *>(getModuleByPath(cstr));
            sensor->isSleep = true;
        }
        //setting the top to awake
        for (int i = 0; i < top; i++) {
            std::string s = std::to_string(max[i]);
            std::string path = "Grid.sensors[" + s + "]";
            const char *cstr = path.c_str();
            Sensor *sensor = check_and_cast<Sensor *>(getModuleByPath(cstr));
            sensor->isSleep = false;
        }
        scheduleAt(simTime() + 1, determine);
    }

}
