#include "stdafx.h"
#include "Car.h"

Car::Car()
{
	wheelBase = 2.5;
	frontTrack = 1.6;
	rearTrack = 1.8;
	position = Vertex3D(0, 0, 0);
	heading = 0;
	speed = 0;
	steeringAngle = 0;
	leftFrontWheel = Vertex3D(0, 0, 0);
	rightFrontWheel = Vertex3D(0, 0, 0);
	leftRearWheel = Vertex3D(0, 0, 0);
	rightRearWheel = Vertex3D(0, 0, 0);
}
Car::~Car()
{
}
void Car::updateWheels()
{
	if (itTrack == track.end()) return;

	position = itTrack->second.UTMPosition;
	heading = itTrack->second.Heading;
	speed = itTrack->second.Speed;
	double dxn = itTrack->second.Direction.x;
	double dyn = itTrack->second.Direction.y;

	// calculate the points for the left and right front wheels
	// calculate the vector perpendicular to the track using frontTrack
	double dxp = -dyn * frontTrack / 2.0f;
	double dyp = dxn * frontTrack / 2.0f;
	leftFrontWheel.x = position.x + dxn * wheelBase / 2.0 + dxp;
	leftFrontWheel.y = position.y + dyn * wheelBase / 2.0 + dyp;
	rightFrontWheel.x = position.x + dxn * wheelBase / 2.0 - dxp;
	rightFrontWheel.y = position.y + dyn * wheelBase / 2.0 - dyp;

	// calculate the points for the left and right rear wheels
	dxp = -dyn * rearTrack / 2.0f;
	dyp = dxn * rearTrack / 2.0f;
	leftRearWheel.x = position.x - dxn * wheelBase / 2.0 + dxp;
	leftRearWheel.y = position.y - dyn * wheelBase / 2.0 + dyp;
	rightRearWheel.x = position.x - dxn * wheelBase / 2.0 - dxp;
	rightRearWheel.y = position.y - dyn * wheelBase / 2.0 - dyp;

	segFrontAxle = Segment((Point2D)leftFrontWheel, (Point2D)rightFrontWheel);
	segRearAxle = Segment((Point2D)leftRearWheel, (Point2D)rightRearWheel);

}
bool Car::checkFrontAxleIntersection(Segment seg, Vertex3D& intersectionPoint)
{
	std::vector<Point2D> intersection_points;
	bg::intersection(segFrontAxle, seg, intersection_points);
	if (!intersection_points.empty()) {
		intersectionPoint.x = bg::get<0>(intersection_points[0]);
		intersectionPoint.y = bg::get<1>(intersection_points[0]);
		return true;
	}
	return false;
}

void Car::checkTrack(TelemetryDataCollection& compareCollection)
{
	//start();
	//while (itTrack != track.end())
	//{
	//	//Vertex3D intersectionPoint;
	//	//// check if the front axle intersects with another segment
	//	//if (checkFrontAxleIntersection(compareCollection.getTelemetry(0).Direction, intersectionPoint))
	//	//{
	//	//	// if it does, print the intersection point
	//	//	std::cout << "Intersection at: " << intersectionPoint.x << ", " << intersectionPoint.y << std::endl;
	//	//}
	//	//progressOnTrack();


}