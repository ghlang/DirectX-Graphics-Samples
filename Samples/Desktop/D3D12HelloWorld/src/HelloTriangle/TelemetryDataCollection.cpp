#include "stdafx.h" 
#define NOMINMAX
#include <limits>
#include <algorithm>
#include "TelemetryDataCollection.h"
#include "D3D12HelloTriangle.h"

// Undefine the conflicting macros
#ifdef max
#undef max
#endif

#ifdef min
#undef min
#endif

void TelemetryDataCollection::normalizeCoordinates()
{
	double minEast = std::numeric_limits<double>::max();
	double maxEast = std::numeric_limits<double>::min();
	double minNorth = std::numeric_limits<double>::max();
	double maxNorth = std::numeric_limits<double>::min();
	auto last = data[0];
	auto curr = data[0];
	for (const auto& pair : data) {
		curr = pair.second;
		minEast = std::min(minEast, pair.second.UTMEast);
		maxEast = std::max(maxEast, pair.second.UTMEast);
		minNorth = std::min(minNorth, pair.second.UTMNorth);
		maxNorth = std::max(maxNorth, pair.second.UTMNorth);
		double dEast = last.UTMEast - curr.UTMEast;
		double dNorth = last.UTMNorth - curr.UTMNorth;
		double dist = sqrt(dEast * dEast + dNorth * dNorth);
		if (dist < 0.01) {
			continue;
		}
		double vel = dist / 0.02; //curr.TimeStamp - last.TimeStamp);
		// convert meters/second to km/h
		vel *= 3.6;
		double phi = atan2(dNorth, dEast);
		// convert to degrees
		last = curr;
	}
	double diffEast = maxEast - minEast;
	double diffNorth = maxNorth - minNorth;
	for (auto& pair : data) {
		pair.second.NormalizedEast = (pair.second.UTMEast - minEast) / (maxEast - minEast)-0.5;
		pair.second.NormalizedNorth = (pair.second.UTMNorth - minNorth) / (maxNorth - minNorth)-0.5;
	}
}

void TelemetryDataCollection::copyToVertexBuffer(void* buffer, DirectX::XMFLOAT4 color)
{
	D3D12HelloTriangle::Vertex* vertexBuffer = reinterpret_cast<D3D12HelloTriangle::Vertex*>(buffer);
	for (const auto& pair : data) {
		vertexBuffer->position.x = static_cast<float>(pair.second.NormalizedEast);
		vertexBuffer->position.y = static_cast<float>(pair.second.NormalizedNorth);
		vertexBuffer->position.z = 0.0f;
		vertexBuffer->color.x = color.x;
		vertexBuffer->color.y = color.y;
		vertexBuffer->color.z = color.z;
		vertexBuffer->color.w = color.w;
		vertexBuffer++;
	}
}
