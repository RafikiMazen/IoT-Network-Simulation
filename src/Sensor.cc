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

#include "Sensor.h"
#include "Smartphone.h"
#include "string"
#include "strings.h"
#include "Cloud.h"
Define_Module(Sensor);

void Sensor::initialize() {
    WATCH(x);
    WATCH(y);
    WATCH(p);
    WATCH(b);
    WATCH(isSleep);
    WATCH(isDead);
    spd = 10;
    battery = obattery;
    isDead = false;
    int index = getIndex();
    //creating x-coordinates
    double value = 0;
    double xs[numOfSensors];
    int xcounter = 1;
    for (int i = 0; i < numOfSensors; i++) {
        xs[i] = value;
        value = value + spd;
        xcounter++;
        if (xcounter > width) {
            value = 0;
            xcounter = 1;
        }
    }
    //creating y-coordinates
    value = 0;
    int ycounter = 1;
    double ys[numOfSensors];
    for (int i = 0; i < numOfSensors; i++) {
        ys[i] = value;
        ycounter++;
        if (ycounter > width) {
            value = value + spd;
            ycounter = 1;
        }
    }
    x = xs[index];
    y = ys[index];

    /*Cloud *c = check_and_cast<Cloud *>(getModuleByPath("Grid.cloud"));
     x = c->xs[index];
     y = c->ys[index];*/

    for (int i = 0; i < 10; i++)
        no[i] = 0;
    isSleep = true;
    frequent = new cMessage("frequent");
    cycle = new cMessage("cycle");
    end = new cMessage("end");
    scheduleAt(0, cycle);

}

void Sensor::handleMessage(cMessage *msg) {
    if (msg == cycle) {
        packetCounter = 0;
        willSend = false;
        if (isSleep == false) {
            s = getModuleByPath("Grid")->par("pktsGenerated").doubleValue();
            s++;
            getModuleByPath("Grid")->par("pktsGenerated").setDoubleValue(s);

            double diffx;
            double diffy;
            double diff;
            cMessage *data = new cMessage("data");
            for (int i = 0; i < numOfPhones; i++) {
                std::string s = std::to_string(i);
                std::string path = "Grid.smartphones[" + s + "]";
                const char *cstr = path.c_str();
                Smartphone *phone = check_and_cast<Smartphone *>(
                        getModuleByPath(cstr));
                diffx = abs(x - phone->x);
                diffy = abs(y - phone->y);
                diff = sqrt(pow(diffx, 2) + pow(diffy, 2));
                if (diff < range) {
                    sendDirect(data->dup(), phone->gate("in"));
                    packetCounter++;
                    willSend = true;
                    break;
                }
            }
            if (willSend == true) {
                re = getModuleByPath("Grid")->par("pktsSent").doubleValue();
                re++;
                getModuleByPath("Grid")->par("pktsSent").setDoubleValue(re);
            }
            ra =
                    (getModuleByPath("Grid")->par("pktsSent").doubleValue()
                            * 1.0
                            / getModuleByPath("Grid")->par("pktsGenerated").doubleValue());
            getModuleByPath("Grid")->par("PDR").setDoubleValue(ra);
        }
        updateBattery(willSend);
        for (int i = 0; i < 10; i++)
            no[i] = 0;
        sum = 0;
        counter = 9;
        scheduleAt(simTime(), frequent);
        scheduleAt(simTime() + 1, cycle);
    }
    if (msg == frequent) {
        found = 0;
        double diffx;
        double diffy;
        double diff;
        for (int i = 0; i < numOfPhones; i++) {
            std::string s = std::to_string(i);
            std::string path = "Grid.smartphones[" + s + "]";
            const char *cstr = path.c_str();
            Smartphone *phone = check_and_cast<Smartphone *>(
                    getModuleByPath(cstr));
            diffx = abs(x - phone->getXPosition(simTime().dbl()));
            diffy = abs(y - phone->getYPosition(simTime().dbl()));
            diff = sqrt(pow(diffx, 2) + pow(diffy, 2));
            if (diff < range)
                found++;
        }

        no[counter] = found;
        counter--;
        if (counter > -1) {
            scheduleAt(simTime() + 0.1, frequent);
        } else {
            for (int i = 0; i < 10; i++) {
                EV << no[i] << " cell ";
                if (no[i] > 0)
                    sum++;
            }

            p = sum * 1.0 / 10;
            EV << sum << " sum " << p << " p " << "\n";
            b = battery / obattery;
        }
    }

}
void Sensor::updateBattery(bool willSend) {
    double currentConsumption;
    idleDuration = 1 - (txDuration * packetCounter);
    if (isSleep == true)
        currentConsumption = sleepConsumption * sleepDuration;
    else {
        if (willSend == true)
            currentConsumption = (idleConsumption * idleDuration)
                    + (txConsumption * txDuration * packetCounter);
        else
            currentConsumption = (idleConsumption * idleDuration);
    }
    battery = battery - currentConsumption;
    if (battery <= 0) {
         b = -1;
         isDead = true;
         getModuleByPath("Grid")->par("timeOf1Death").setDoubleValue(
         simTime().dbl());
         endSimulation();
        /*int as = getModuleByPath("Grid")->par("aliveSensors").doubleValue();
        if (isDead == false) {
            isDead = true;
            as--;
            getModuleByPath("Grid")->par("aliveSensors").setDoubleValue(as);
        }
        b = -1;
        battery = -1;
        if (as <= 15) {
            getModuleByPath("Grid")->par("timeof40Death").setDoubleValue(
                    simTime().dbl());
            endSimulation();
        }*/
    }
}
