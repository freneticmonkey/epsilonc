#pragma once

#include "math/Vector.h"

#include <boost/format.hpp>

namespace epsilon
{
	class Bounds
	{
	public:
		enum Intersection
		{
			OUTSIDE = 0,
			INSIDE	= 1,
			INTERSECTION = 2
		};

		Vector3 centre;
		Vector3 size; // Half widths

		// By default the Bounding Box will encapsulate 0 space.
		Bounds() {};

		Bounds(const Vector3& min, const Vector3& max)
		{
			centre = Vector3(min.x + ((max.x - min.x) / 2.0f),
							min.y + ((max.y - min.y) / 2.0f),
							min.z + ((max.z - min.z) / 2.0f));
			// Calc half widths
			size = max - centre;
		}

		// To String
		inline std::string ToString()
		{
			return boost::str(format("Bounds: centre: %s size: %s") % centre.ToString() % size.ToString());
		}

		void GetMinMax(Vector3& min, Vector3& max)
		{
			min = centre - size;
			max = centre + size;
		}

		Vector3 GetMin()
		{
			return centre - size;
		}

		Vector3 GetMax()
		{
			return centre + size;
		}

		void Nullify()
		{
			centre = Vector3::ZERO;
			size = Vector3::ZERO;
		}

		void Scale(const Vector3& scale)
		{
			size *= scale;
		}

		bool IsNull() const { return (centre == Vector3::ZERO) && (size == Vector3::ZERO); }

		float GetWidth()  { return size.x * 2.0f; }
		float GetHeight() { return size.y * 2.0f; }
		float GetDepth()  { return size.z * 2.0f; }

		bool Overlap(const Bounds& other) const
		{
			if (IsNull() || other.IsNull())
				return false;

			if (abs(centre.x - other.centre.x) > (size.x + other.size.x)) return false;
			if (abs(centre.y - other.centre.y) > (size.y + other.size.y)) return false;
			if (abs(centre.z - other.centre.z) > (size.z + other.size.z)) return false;

			// If all distance is smaller on all axis, then there is an overlap
			return true;
		}

		void Extend(const float& extentIncrease)
		{
			size += Vector3(extentIncrease);
		}

		void Extend(const Vector3& extentIncrease)
		{
			size += extentIncrease;
		}

		Bounds::Intersection Intersect(const Bounds& other) const
		{
			if (IsNull() && other.IsNull())
				return Intersection::OUTSIDE;
			
			Vector3 min = centre - size;
			Vector3 max = centre + size;
			Vector3 omin = other.centre - other.size;
			Vector3 omax = other.centre + other.size;

			if ( (min.x < omin.x) || (min.x > omax.x) ||
				 (min.y < omin.y) || (min.y > omax.y) ||
				 (min.z < omin.z) || (min.z > omax.z)
			   ) 
				return Intersection::OUTSIDE;

			if ((min.x <= omin.x) || (max.x >= omax.x) ||
				(min.y <= omin.y) || (max.y >= omax.y) ||
				(min.z <= omin.z) || (max.z >= omax.z)
				)
				return Intersection::INSIDE;

			return Intersection::INTERSECTION;
		}

		Bounds operator+(const Vector3& point) 
		{
			Vector3 min = centre - size;
			Vector3 max = centre + size;

			if (point.x < min.x)
				min.x = point.x;
			if (point.y < min.y)
				min.y = point.y;
			if (point.z < min.z)
				min.z = point.z;

			if (point.x > max.x)
				max.x = point.x;
			if (point.y > max.y)
				max.y = point.y;
			if (point.z > max.z)
				max.z = point.z;

			return Bounds(min, max);
		}

		Bounds operator+(const Bounds& other)
		{
			Vector3 min = centre - size;
			Vector3 max = centre + size;
			Vector3 othermin = other.centre - other.size;
			Vector3 othermax = other.centre + other.size;

			if (othermin.x < min.x)
				min.x = othermin.x;
			if (othermin.y < min.y)
				min.y = othermin.y;
			if (othermin.z < min.z)
				min.z = othermin.z;

			if (othermax.x > max.x)
				max.x = othermax.x;
			if (othermax.y > max.y)
				max.y = othermax.y;
			if (othermax.z > max.z)
				max.z = othermax.z;

			return Bounds(min, max);
		}

		Bounds& operator+=(const Bounds& other)
		{
			Vector3 min = centre - size;
			Vector3 max = centre + size;
			Vector3 othermin = other.centre - other.size;
			Vector3 othermax = other.centre + other.size;

			if (othermin.x < min.x)
				min.x = othermin.x;
			if (othermin.y < min.y)
				min.y = othermin.y;
			if (othermin.z < min.z)
				min.z = othermin.z;

			if (othermax.x > max.x)
				max.x = othermax.x;
			if (othermax.y > max.y)
				max.y = othermax.y;
			if (othermax.z > max.z)
				max.z = othermax.z;

			centre = Vector3(min.x + ((max.x - min.x) / 2.0f),
				min.y + ((max.y - min.y) / 2.0f),
				min.z + ((max.z - min.z) / 2.0f));

			// Calc half widths
			size = max - centre;

			return *this;
		}
	};
}