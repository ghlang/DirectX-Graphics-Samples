#include "stdafx.h" 
#include "TelemetryDataCollection.h"
#include "D3D12HelloTriangle.h"





void TelemetryDataCollection::checkBoundaries()
{
	// check for each data element that is within boundaries using boost's intersection function	
	// if not, print a warning	

	for (const auto& pair : data) {
		// check if the data is within the boundaries
		namespace bg = boost::geometry;
		using Point2D = bg::model::point<float, 2, bg::cs::cartesian>;
		using Segment = bg::model::segment<Point2D>;
		Segment seg1(Point2D(0, 0), Point2D(4, 4));

	}

}

void TelemetryDataCollection::checkForIntersections(TelemetryDataCollection& compareCollection)
{
	// check for each data element that is within boundaries using boost's intersection function	
	// if not, print a warning	
	Vertex3D lastPoint;
	Vertex3D currPoint;
	double currTimeStamp;
	bool first = true;
	int ind = 0;
	const double frontTrack = 1.6f;
	for (auto& pair : data) {
		currPoint = pair.second.UTMPosition;
		currTimeStamp = pair.first;
		// skip if this is the first point
		if (first) {
			lastPoint = currPoint;
			first = false;
			ind++;
			continue;
		}

		// calculate the distance between the last and current point
		double dxs = lastPoint.x - currPoint.x;
		double dys = lastPoint.y - currPoint.y;
		double dist = sqrt(dxs * dxs + dys * dys);
		// normalize dx and dy
		double dxsn = dxs / dist;
		double dysn = dys / dist;
		// calculate the vector perpendicular to the track using frontTrack
		double dxp = -dysn * frontTrack / 2.0f;
		double dyp = dxsn * frontTrack / 2.0f;
		// calculate the points for the left and right wheel
		Vertex3D leftWheel;
		Vertex3D rightWheel;
		leftWheel.x = currPoint.x + dxp;
		leftWheel.y = currPoint.y + dyp;
		rightWheel.x = currPoint.x - dxp;
		rightWheel.y = currPoint.y - dyp;
		// define a segement between the left and right wheel
		Segment segl((Point2D) currPoint, (Point2D) leftWheel);
		Segment segr((Point2D) currPoint, (Point2D) rightWheel);

		dist = bg::distance((Point2D)lastPoint, (Point2D)currPoint);

		// check if this segment is interescting with any segment of the compareCollection. If so print a warning and exit this loop
		Vertex3D compareLastPoint;
		Vertex3D compareCurrPoint;
		bool compareFirst = true;
        
		
		int imatch = compareCollection.getClosestIndex(currTimeStamp);

		for (const auto& comparePair : compareCollection.data) {
			compareCurrPoint = comparePair.second.UTMPosition;
			// skip if this is the first point
			if (compareFirst || bg::distance((Point2D)compareCurrPoint, (Point2D)currPoint) > 10.f) {
				compareLastPoint = compareCurrPoint;
				compareFirst = false;
				continue;
			}
			// define a segment from the last point to the current point
			Segment seg2((Point2D) compareLastPoint, (Point2D) compareCurrPoint);
			// return a list of points where the segment intersects with the boundaries
			std::vector<Point2D> intersection_points;
			bg::intersection(segl, seg2,intersection_points);
			if (!intersection_points.empty()) {
				dist = bg::distance(intersection_points[0], (Point2D) currPoint);
				if (dist > 0.2f) {
//					pair.second.Color = DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
					break;
				}
			}
			bg::intersection(segr, seg2, intersection_points);
			if (!intersection_points.empty()) {
				dist = bg::distance(intersection_points[0], (Point2D)currPoint);
				if (dist > 0.2f) {
//					pair.second.Color = DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
					break;
				}
			}
			compareLastPoint = compareCurrPoint;
		}
		lastPoint = currPoint;
		ind++;
	}
}
