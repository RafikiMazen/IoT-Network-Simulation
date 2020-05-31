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

package myalgorithm;

//
// TODO auto-generated module
//
simple PublicSensing
{
}
network Grid
{
    parameters:
        int numOfPhones = 10;
        @display("i=block/network2");
        int width @prompt("Number of columns") = default(4);
        int height @prompt("Number of rows") = default(5);
        int numOfSensors = height*width;
        int pktsGenerated = 0;
        int pktsSent = 0;
        double PDR = 0; // packets dilvery ratio
        double timeOf1Death = 0;
        int aliveSensors = numOfSensors;
        double timeof40Death = 0;

    submodules:
        cloud: Cloud;
        sensors[numOfSensors]: Sensor {
            parameters:
                @display("p=,,m,$width");
        }
        smartphones[numOfPhones]: Smartphone;
}
