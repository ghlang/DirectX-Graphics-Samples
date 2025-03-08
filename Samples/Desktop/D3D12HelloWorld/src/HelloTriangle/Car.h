#ifndef CAR_H
#define CAR_H
#include "DXSample.h"
#include <iostream>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <DirectXMath.h> 
#include <cmath>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include "GeomObjects.h"
#include "TelemetryDataCollection.h"

class Car
{
public:
	Car();
	~Car();
	void SetPosition(Vertex3D newPosition) {
		position = newPosition;
	}
	void SetHeading(double newHeading) {
		heading = newHeading;
	}
	void SetHeading(double dx, double dy) {
		dxHeading = dx;
		dyHeading = dy;
	}
	void SetSpeed(double newSpeed) {
		speed = newSpeed;
	}
	void updateWheels();
	// check if the front axle intersects with another segment return the intersection point if true
	bool checkFrontAxleIntersection(Segment seg, Vertex3D& intersectionPoint); 
	// set the track of the car
	void setTrack(const TelemetryDataCollection& newTrack) {
		track = newTrack;
	}
	void progressOnTrack()
	{
		if (itTrack != track.end()) {
			position = itTrack->second.UTMPosition;
			heading = itTrack->second.Heading;
			speed = itTrack->second.Speed;
			steeringAngle = itTrack->second.SteeringNormal;
		}
		updateWheels();
	}
	void start() {
		// start the car
		itTrack = track.begin();
	}
	void checkTrack(TelemetryDataCollection& compareCollection);


private:
	// static properties
	double wheelBase;
	double frontTrack;
	double rearTrack;
	// dynamic properties
	Vertex3D position;
	double heading;
	double speed;
	double steeringAngle;
	// calculated properties
	double dxHeading;
	double dyHeading;
	Vertex3D leftFrontWheel;
	Vertex3D rightFrontWheel;
	Vertex3D leftRearWheel;
	Vertex3D rightRearWheel;
	Segment segFrontAxle;
	Segment segRearAxle;
	TelemetryDataCollection track;
	std::map <double, TelemetryData>::iterator itTrack;
};
#endif 
