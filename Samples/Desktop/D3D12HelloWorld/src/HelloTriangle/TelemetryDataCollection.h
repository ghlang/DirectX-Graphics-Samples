#ifndef TELEMETRY_DATA_COLLECTION_H
#define TELEMETRY_DATA_COLLECTION_H
#define NOMINMAX
#include <limits>
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <DirectXMath.h> // Include DirectXMath for XMFLOAT4
#include <cmath>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include "DXSample.h"
#include "GeomObjects.h"

// Undefine the conflicting macros
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif


struct TelemetryData {
	// data from the telemetry file
	double TimeStamp;
	double SteeringNormal;
	double Speed;
	Vertex3D UTMPosition; // UTM position x=East, y=North, z=Altitude	
	double Heading;
	// additional calculated data
	Vertex2D Direction; // normalized direction to the next point
	double Distance; // distance to the next point
};



class TelemetryDataCollection {
private:
    std::map<double, TelemetryData> data;
	Vertex3D minPoint;
	Vertex3D maxPoint;
	std::map <double, TelemetryData>::iterator itData;

public:
    // Add a telemetry entry
    void addTelemetry(const TelemetryData& entry) {
        data[entry.TimeStamp] = entry;
    }

	std::map<double, TelemetryData>::iterator begin() {
		return data.begin();
	}

	// Method to get an iterator to the end of the map
	std::map<double, TelemetryData>::iterator end() {
		return data.end();
	}

	Vertex3D getMinPoint() {
		return minPoint;
	}
	Vertex3D getMaxPoint() {
		return maxPoint;
	}

    // Retrieve a telemetry entry by timestamp
    bool getTelemetry(double timeStamp, TelemetryData& entry) const {
        auto it = data.find(timeStamp);
        if (it != data.end()) {
            entry = it->second;
            return true;
        }
        return false;
    }

	bool getTelemetry(int index, TelemetryData& entry) const {
		auto it = data.begin();
		std::advance(it, index);
		if (it != data.end()) {
			entry = it->second;
			return true;
		}
		return false;
	}


	const TelemetryData getTelemetry(int index) const {
		auto it = data.begin();
		std::advance(it, index);
		return it->second;
	}

	// Remove a telemetry entry by timestamp
	bool removeTelemetry(double timeStamp) {
		return data.erase(timeStamp) > 0;
	}

    // add a function that read telemetry data from a text file where the data is comma separated and each item is a new line
	void readTelemetry(const std::string& filename, DirectX::XMFLOAT4 color) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << '\n';
			return;
		}

		// reset the min and max points
		minPoint = Vertex3D(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), std::numeric_limits<double>::max());
		maxPoint = Vertex3D(std::numeric_limits<double>::min(), std::numeric_limits<double>::min(), std::numeric_limits<double>::min());

		std::string line;
        std::getline(file, line); // read header
        while (std::getline(file, line)) {
			std::istringstream ss(line);
			std::string token;
			TelemetryData entry;
			int i = 0; 
			while (std::getline(ss, token, ',')) {
				switch (i) {
				case 0:
					entry.TimeStamp = std::stod(token);
					break;
				case 1:
					entry.SteeringNormal = std::stod(token);
					break;
				case 2:
					entry.Speed = std::stod(token);
					break;
				case 3:
					entry.UTMPosition.x = std::stod(token);
					if (entry.UTMPosition.x < minPoint.x) minPoint.x = entry.UTMPosition.x;
					if (entry.UTMPosition.x > maxPoint.x) maxPoint.x = entry.UTMPosition.x;
					break;
				case 4:
					entry.UTMPosition.y = std::stod(token);
					if (entry.UTMPosition.y < minPoint.y) minPoint.y = entry.UTMPosition.y;
					if (entry.UTMPosition.y > maxPoint.y) maxPoint.y = entry.UTMPosition.y;
					break;
				case 5:
					entry.UTMPosition.z = std::stod(token);
					if (entry.UTMPosition.z < minPoint.z) minPoint.z = entry.UTMPosition.z;
					if (entry.UTMPosition.z > maxPoint.z) maxPoint.z = entry.UTMPosition.z;
					break;
				case 6:
					entry.Heading = std::stod(token);
					break;
				}
				i++;
			}
			addTelemetry(entry);
		}
		file.close();
		itData = data.begin();
		calculateDirection();

	}

	// add a method to calculate the direction of every point in the collection
	void calculateDirection() {
		auto last = data[0];
		auto curr = data[0];
		bool first = true;
		Vertex2D dir;
		for (auto& pair : data) {
			curr = pair.second;
			if (first) {
				last = curr;
				first = false;
				continue;
			}
			dir.x = last.UTMPosition.x - curr.UTMPosition.x;
			dir.y = last.UTMPosition.y - curr.UTMPosition.y;
			double dist = sqrt(dir.x * dir.x + dir.y * dir.y);
			dir.x /= dist;
			dir.y /= dist;
			last.Direction = dir;
			last.Distance = dist;
			last = curr;
		}
		last.Direction = dir;
		last.Distance = 0;
	}
			
			
           
            


    // Print all telemetry data
/*    void printAll() const {
        for (const auto& pair : data) {
            std::cout << "TimeStamp: " << pair.second.TimeStamp
                << ", SteeringNormal: " << pair.second.SteeringNormal
                << ", Speed: " << pair.second.Speed
                << ", UTMEast: " << pair.second.UTMEast
                << ", UTMNorth: " << pair.second.UTMNorth
                << ", UTMAltitude: " << pair.second.UTMAltitude
                << ", Heading: " << pair.second.Heading << '\n';
        }
    }
*/
	int getSize() const {
		return data.size();
	}

    void clear() {
        data.clear(); // Removes all elements from the map
    }

	void checkBoundaries();
	void checkForIntersections(TelemetryDataCollection& compareCollection);
	// add a method to return the index of the data element closest to a given time stamp
	int getClosestIndex(double timeStamp) const {
		auto closest = data.lower_bound(timeStamp);
		if (closest == data.end() ||
			(closest != data.begin() && timeStamp - std::prev(closest)->first < closest->first - timeStamp)) {
			--closest;
		}
		return std::distance(data.begin(), closest);
	}

	// add a method to return the a data element of a given index assuming the map is a closed loop
	// allow for indices less than zero and larger the size of the map
	TelemetryData getClosedLoop(int index) const {
		int size = data.size();
		if (size == 0) {
			return TelemetryData();
		}
		index = (index + size) % size;
		auto it = data.begin();
		std::advance(it, index);
		return it->second;
	}





};

#endif // TELEMETRY_DATA_COLLECTION_H
