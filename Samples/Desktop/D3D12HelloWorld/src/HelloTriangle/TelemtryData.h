#pragma once
class TelemtryData
{
public:
	TelemetryData();
	TelemetryData(double TimeStamp,
		double SteeringNormal,
		double Speed,
		double UTMEast,
		double UTMNorth,
		double UTMAltitude,
		double Heading
	);
	~TelemetryData();

private:
	double TimeStamp;
	double SteeringNormal;
	double Speed;
	double UTMEast;
	double UTMNorth;
	double UTMAltitude;
	double Heading;

};

