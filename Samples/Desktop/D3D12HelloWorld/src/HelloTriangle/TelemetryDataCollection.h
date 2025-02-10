#ifndef TELEMETRY_DATA_COLLECTION_H
#define TELEMETRY_DATA_COLLECTION_H
#include "DXSample.h"
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <DirectXMath.h> // Include DirectXMath for XMFLOAT4


struct TelemetryData {
    double TimeStamp;
    double SteeringNormal;
    double Speed;
    double UTMEast;
    double UTMNorth;
    double UTMAltitude;
    double Heading;
    double NormalizedEast;
	double NormalizedNorth;
};

class TelemetryDataCollection {
private:
    std::map<double, TelemetryData> data;

public:
    // Add a telemetry entry
    void addTelemetry(const TelemetryData& entry) {
        data[entry.TimeStamp] = entry;
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
	void readTelemetry(const std::string& filename) {
		std::ifstream file(filename);
		if (!file.is_open()) {
			std::cerr << "Failed to open file: " << filename << '\n';
			return;
		}
            
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
					entry.UTMEast = std::stod(token);
					break;
				case 4:
					entry.UTMNorth = std::stod(token);
					break;
				case 5:
					entry.UTMAltitude = std::stod(token);
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
	}

           
            


    // Print all telemetry data
    void printAll() const {
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

	int getSize() const {
		return data.size();
	}

	void copyToVertexBuffer(void* buffer, DirectX::XMFLOAT4 color);

    void normalizeCoordinates();
    void clear() {
        data.clear(); // Removes all elements from the map
    }
    

};

#endif // TELEMETRY_DATA_COLLECTION_H
