#pragma once

#ifndef __Vector3D_H
#define __Vector3D_H

#include <cmath>
#include <float.h>
#include <sstream>

#define CONST_PI 3.142
#define FastSin sin
#define FastCos cos

class Vector3D
{
   public:
		Vector3D(double fX = 0.0f, double fY = 0.0f, double fZ = 0.0f)
		{
			m_fPoint[0] = fX;
			m_fPoint[1] = fY;
			m_fPoint[2] = fZ;
		}

		//! Constructor
		Vector3D(const Vector3D& pVector)
		{
			m_fPoint[0] = pVector.m_fPoint[0];
			m_fPoint[1] = pVector.m_fPoint[1];
			m_fPoint[2] = pVector.m_fPoint[2];
		}

        operator std::string ( void )
        {
           std::ostringstream tmp;
           tmp << "{ x: " << m_fPoint[0] << "," << " y: " << m_fPoint[1] << "," << " z: " << m_fPoint[2] << " }";
           return tmp.str();
        }

		//! Vector assignment
		inline const Vector3D& operator = (const Vector3D& pVector)
		{
			m_fPoint[0] = pVector.m_fPoint[0];
			m_fPoint[1] = pVector.m_fPoint[1];
			m_fPoint[2] = pVector.m_fPoint[2];

			// Return modified vector
			return *this;
		}

		//! Vector assignment
		inline const Vector3D& operator = (const double* pVector)
		{
			m_fPoint[0] = pVector[0];
			m_fPoint[1] = pVector[1];
			m_fPoint[2] = pVector[2];

			// Return modified vector
			return *this;
		}

		//! Vector comparison
		inline bool operator == (const Vector3D& pVector) const
		{
			if (m_fPoint[0] == pVector.m_fPoint[0] &&
				m_fPoint[1] == pVector.m_fPoint[1] &&
				m_fPoint[2] == pVector.m_fPoint[2])
				return true;

			// Vector is not equal
			return false;
		}

		//! Vector less than comparison
		inline bool operator < (const Vector3D& pVector) const
		{
			if (m_fPoint[0] < pVector.m_fPoint[0] &&
				m_fPoint[1] < pVector.m_fPoint[1] &&
				m_fPoint[2] < pVector.m_fPoint[2])
				return true;

			// Vector is greater than or equal
			return false;
		}

		//! Vector less than or equal comparison
		inline bool operator <= (const Vector3D& pVector) const
		{
			if (m_fPoint[0] <= pVector.m_fPoint[0] &&
				m_fPoint[1] <= pVector.m_fPoint[1] &&
				m_fPoint[2] <= pVector.m_fPoint[2])
				return true;

			// Vector is greater than
			return false;
		}

		//! Vector greater than comparison
		inline bool operator > (const Vector3D& pVector) const
		{
			if (m_fPoint[0] > pVector.m_fPoint[0] &&
				m_fPoint[1] > pVector.m_fPoint[1] &&
				m_fPoint[2] > pVector.m_fPoint[2])
				return true;

			// Vector is less than or equal
			return false;
		}

		//! Vector greater than or equal comparison
		inline bool operator >= (const Vector3D& pVector) const
		{
			if (m_fPoint[0] >= pVector.m_fPoint[0] &&
				m_fPoint[1] >= pVector.m_fPoint[1] &&
				m_fPoint[2] >= pVector.m_fPoint[2])
				return true;

			// Vector is less than
			return false;
		}

		//! Vector addition
		inline const Vector3D operator + (const Vector3D& pVector) const
		{
			// Return new vector result
			return Vector3D(m_fPoint[0] + pVector.m_fPoint[0],
                            m_fPoint[1] + pVector.m_fPoint[1],
                            m_fPoint[2] + pVector.m_fPoint[2]);
		}

		//! Vector addition
		inline const Vector3D operator + (double fOperand) const
		{
			// Return new vector result
			return Vector3D(m_fPoint[0] + fOperand,
                            m_fPoint[1] + fOperand,
                            m_fPoint[2] + fOperand);
		}

		//! Unary positive vector
		inline const Vector3D operator + () const
		{
			// Return modified vector
			return Vector3D(*this);
		}

		//! Vector addition
		inline const Vector3D& operator += (const Vector3D& pVector)
		{
			m_fPoint[0] += pVector.m_fPoint[0];
			m_fPoint[1] += pVector.m_fPoint[1];
			m_fPoint[2] += pVector.m_fPoint[2];

			// Return modified vector
			return *this;
		}

		//! Vector addition
		inline const Vector3D& operator += (double fOperand)
		{
			m_fPoint[0] += fOperand;
			m_fPoint[1] += fOperand;
			m_fPoint[2] += fOperand;

			// Return modified vector
			return *this;
		}

		//! Vector subtraction
		inline const Vector3D operator - (const Vector3D& pVector) const
		{
			// Return new vector
			return Vector3D(m_fPoint[0] - pVector.m_fPoint[0],
                            m_fPoint[1] - pVector.m_fPoint[1],
                            m_fPoint[2] - pVector.m_fPoint[2]);
		}

		//! Vector subtraction
		inline const Vector3D operator - (double fOperand) const
		{
			// Return new vector
			return Vector3D(m_fPoint[0] - fOperand,
                            m_fPoint[1] - fOperand,
                            m_fPoint[2] - fOperand);
		}

		//! Unary negative vector
		inline const Vector3D operator - () const
		{
			// Return new vector
			return Vector3D(-m_fPoint[0],
                            -m_fPoint[1],
                            -m_fPoint[2]);
		}

		//! Vector subtraction
		inline const Vector3D& operator -= (const Vector3D& pVector)
		{
			m_fPoint[0] -= pVector.m_fPoint[0];
			m_fPoint[1] -= pVector.m_fPoint[1];
			m_fPoint[2] -= pVector.m_fPoint[2];

			// Return modified vector
			return *this;
		}

		//! Vector subtraction
		inline const Vector3D& operator -= (double fOperand)
		{
			m_fPoint[0] -= fOperand;
			m_fPoint[1] -= fOperand;
			m_fPoint[2] -= fOperand;

			// Return modified vector
			return *this;
		}

		//! Vector multiplication
		inline const Vector3D operator * (const Vector3D& pVector) const
		{
			// Return new vector
			return Vector3D(m_fPoint[0] * pVector.m_fPoint[0],
                            m_fPoint[1] * pVector.m_fPoint[1],
                            m_fPoint[2] * pVector.m_fPoint[2]);
		}

		//! Vector multiplication
		inline const Vector3D operator * (double fOperand) const
		{
			// Return new vector
			return Vector3D(m_fPoint[0] * fOperand,
							m_fPoint[1] * fOperand,
							m_fPoint[2] * fOperand);
		}

		//! Vector multiplication
		inline const Vector3D& operator *= (const Vector3D& pVector)
		{
			m_fPoint[0] *= pVector.m_fPoint[0];
			m_fPoint[1] *= pVector.m_fPoint[1];
			m_fPoint[2] *= pVector.m_fPoint[2];

			// Return modified vector
			return *this;
		}

		//! Vector multiplication
		inline const Vector3D& operator *= (double fOperand)
		{
			m_fPoint[0] *= fOperand;
			m_fPoint[1] *= fOperand;
			m_fPoint[2] *= fOperand;

			// Return modified vector
			return *this;
		}

		//! Vector division
		inline const Vector3D operator / (const Vector3D& pVector) const
		{
			// Return new vector
			return Vector3D(m_fPoint[0] / pVector.m_fPoint[0],
                            m_fPoint[1] / pVector.m_fPoint[1],
                            m_fPoint[2] / pVector.m_fPoint[2]);
		}

		//! Vector division
		inline const Vector3D operator / (double fOperand) const
		{
			// Return new vector
			return Vector3D(m_fPoint[0] / fOperand,
							m_fPoint[1] / fOperand,
							m_fPoint[2] / fOperand);
		}

		//! Vector division
		inline const Vector3D& operator /= (const Vector3D& pVector)
		{
			m_fPoint[0] /= pVector.m_fPoint[0];
			m_fPoint[1] /= pVector.m_fPoint[1];
			m_fPoint[2] /= pVector.m_fPoint[2];

			// Return modified vector
			return *this;
		}

		//! Vector division
		inline const Vector3D& operator /= (double fOperand)
		{
			m_fPoint[0] /= fOperand;
			m_fPoint[1] /= fOperand;
			m_fPoint[2] /= fOperand;

			// Return modified vector
			return *this;
		}

		//! Dot product of vector
		inline const double operator % (const Vector3D& pVector) const
		{
			return m_fPoint[0] * pVector.m_fPoint[0] +
				   m_fPoint[1] * pVector.m_fPoint[1] +
				   m_fPoint[2] * pVector.m_fPoint[2];
		}

		//! Cross product of vector, return new result
		inline const Vector3D operator ^ (const Vector3D& pVector) const
		{
			// Return cross product of both vectors
			return Vector3D(m_fPoint[1] * pVector.m_fPoint[2] - pVector.m_fPoint[1] * m_fPoint[2],
                            m_fPoint[2] * pVector.m_fPoint[0] - pVector.m_fPoint[2] * m_fPoint[0],
                            m_fPoint[0] * pVector.m_fPoint[1] - pVector.m_fPoint[0] * m_fPoint[1]);
		}

		//! Cross product of vector
		inline const Vector3D& operator ^= (const Vector3D& pVector)
		{
			// Return modified vector
			return *this = *this ^ pVector;
		}

		//! Length of vector
		inline const double operator ! () const
		{
			// Return squared coordinate vector
			return sqrtf(m_fPoint[0] * m_fPoint[0] +
				         m_fPoint[1] * m_fPoint[1] +
						 m_fPoint[2] * m_fPoint[2]);
		}

		//! Set length of vector, returning new result
		inline const Vector3D operator | (const double fLength) const
		{
			// If fLength is equal to 1.0f, the Vector becomes normalized
			return *this * (fLength / !*this);
		}

		//! Set length of vector
		inline const Vector3D& operator |= (const double fLength)
		{
			// If fLength is equal to 1.0f, the Vector becomes normalized
			return *this = *this | fLength;
		}

		//! Return Structs::Pointer to double array
		inline operator const double* () const
		{
			// Return Structs::Pointer to double array
			return m_fPoint;
		}

		//! Return Structs::Pointer to double array
		inline operator double* ()
		{
			// Return Structs::Pointer to double array
			return m_fPoint;
		}

		//! Return value specified by element index
		inline double& operator [] (const int iIdx)
		{
			// Return value at specified index
			return m_fPoint[iIdx];
		}

		//! Return value specified by element index
		inline double operator [] (const int iIdx) const
		{
			// Return value at specified index
			return m_fPoint[iIdx];
		}

		inline friend const Vector3D operator * (const double fScaleFactor, const Vector3D& rVector)
		{
			return rVector * fScaleFactor;
		}

		//! Squared length of vector
		inline const double SquaredLength () const
		{
			// Return squared coordinate vector
			return m_fPoint[0] * m_fPoint[0] +
				   m_fPoint[1] * m_fPoint[1] +
				   m_fPoint[2] * m_fPoint[2];
		}

		//! Make all values positive (V = |V|)
		inline void Absolute()
		{
			// Make X coordinate positive
			m_fPoint[0] = fabsf(m_fPoint[0]);

			// Make Y coordinate positive
			m_fPoint[1] = fabsf(m_fPoint[1]);

			// Make Z coordinate positive
			m_fPoint[2] = fabsf(m_fPoint[2]);	
		}

		//! Reset vector
		inline void Zero()
		{
			// Zero X coordinate
			m_fPoint[0] = 0.0f;

			// Zero Y coordinate
			m_fPoint[1] = 0.0f;

			// Zero Z coordinate
			m_fPoint[2] = 0.0f;
		}

		//! Is vector valid?
		inline bool IsZero()
		{
			// Is sum of X,Y,Z zero?
			if (!(m_fPoint[0] + m_fPoint[1] + m_fPoint[2]))
				return true;

			// Vector is not zero
			return false;
		}

		//! Set XYZ components of vector
		inline void Set(const double fX, const double fY, const double fZ)
		{
			m_fPoint[0] = fX;
			m_fPoint[1] = fY;
			m_fPoint[2] = fZ;
		}

		inline void RotateX(const double fAngle)
		{
			(*this) = GetRotatedX(fAngle);
		}

		inline void RotateY(const double fAngle)
		{
			(*this) = GetRotatedY(fAngle);
		}

		inline void RotateZ(const double fAngle)
		{
			(*this) = GetRotatedZ(fAngle);
		}

		inline void RotateAxis(const double fAngle, const Vector3D& rAxis)
		{
			(*this) = GetRotatedAxis(fAngle, rAxis);
		}

		inline const Vector3D GetRotatedX(const double fAngle) const
		{
			if (fAngle == 0.0f)
				return (*this);

			double fSinAngle = FastSin(CONST_PI * fAngle / 180.0f);
			double fCosAngle = FastCos(CONST_PI * fAngle / 180.0f);

			return Vector3D(m_fPoint[0], m_fPoint[1] * fCosAngle -
				            m_fPoint[2] * fSinAngle, m_fPoint[1] *
							fSinAngle + m_fPoint[2] * fCosAngle);
		}
	
		inline const Vector3D GetRotatedY(const double fAngle) const
		{
			if (fAngle == 0.0f)
				return (*this);

			double fSinAngle = FastSin(CONST_PI * fAngle / 180.0f);
			double fCosAngle = FastCos(CONST_PI * fAngle / 180.0f);

			return Vector3D(m_fPoint[0] * fCosAngle + m_fPoint[2] *
				            fSinAngle, m_fPoint[1], -m_fPoint[0] *
							fSinAngle + m_fPoint[2] * fCosAngle);
		}
	
		inline const Vector3D GetRotatedZ(const double fAngle) const
		{
			if (fAngle == 0.0f)
				return (*this);

			double fSinAngle = FastSin(CONST_PI * fAngle / 180.0f);
			double fCosAngle = FastCos(CONST_PI * fAngle / 180.0f);
	
			return Vector3D(m_fPoint[0] * fCosAngle - m_fPoint[1] *
				            fSinAngle, m_fPoint[0] * fSinAngle +
							m_fPoint[1] * fCosAngle, m_fPoint[2]);
		}
	
		inline const Vector3D GetRotatedAxis(const double fAngle, const Vector3D& rAxis) const
		{
			if (fAngle == 0.0)
				return (*this);

			Vector3D vNormalizedAxis = rAxis;

			vNormalizedAxis |= 1;

			Vector3D vRotationMatrixRow0;
			
			Vector3D vRotationMatrixRow1;
			
			Vector3D vRotationMatrixRow2;

			double fSinAngle = FastSin(CONST_PI * fAngle / 180.0f);
			
			double fCosAngle = FastCos(CONST_PI * fAngle / 180.0f);

			double fOneMinusCosAngle = 1.0f - fCosAngle;

			vRotationMatrixRow0[0] = (vNormalizedAxis[0]) * (vNormalizedAxis[0]) + fCosAngle * (1.0f - (vNormalizedAxis[0]) * (vNormalizedAxis[0]));
			
			vRotationMatrixRow0[1] = (vNormalizedAxis[0]) * (vNormalizedAxis[1]) * (fOneMinusCosAngle) - fSinAngle * vNormalizedAxis[2];
			
			vRotationMatrixRow0[2] = (vNormalizedAxis[0]) * (vNormalizedAxis[2]) * (fOneMinusCosAngle) + fSinAngle * vNormalizedAxis[1];

			vRotationMatrixRow1[0] = (vNormalizedAxis[0]) * (vNormalizedAxis[1]) * (fOneMinusCosAngle) + fSinAngle * vNormalizedAxis[2];
			
			vRotationMatrixRow1[1] = (vNormalizedAxis[1]) * (vNormalizedAxis[1]) + fCosAngle * (1.0f - (vNormalizedAxis[1]) * (vNormalizedAxis[1]));
			
			vRotationMatrixRow1[2] = (vNormalizedAxis[1]) * (vNormalizedAxis[2]) * (fOneMinusCosAngle) - fSinAngle * vNormalizedAxis[0];
			
			vRotationMatrixRow2[0] = (vNormalizedAxis[0]) * (vNormalizedAxis[2]) * (fOneMinusCosAngle) - fSinAngle * vNormalizedAxis[1];

			vRotationMatrixRow2[1] = (vNormalizedAxis[1]) * (vNormalizedAxis[2]) * (fOneMinusCosAngle) + fSinAngle * vNormalizedAxis[0];
			
			vRotationMatrixRow2[2] = (vNormalizedAxis[2]) * (vNormalizedAxis[2]) + fCosAngle * (1.0f - (vNormalizedAxis[2]) * (vNormalizedAxis[2]));

			return Vector3D((*this) % vRotationMatrixRow0, (*this) % vRotationMatrixRow1, (*this) % vRotationMatrixRow2);
		}

		inline void PackTo01()
		{
			(*this) = GetPackedTo01();
		}

		inline const Vector3D GetPackedTo01() const
		{
			Vector3D vTemp(*this);

			vTemp |= 1;

			vTemp = vTemp * 0.5f + Vector3D(0.5f, 0.5f, 0.5f);
			
			return vTemp;
		}

		inline const Vector3D Lerp(const Vector3D& rVector, const double fFactor) const
		{
			return (*this) * (1.0f - fFactor) + rVector * fFactor;
		}

		inline const Vector3D QuadraticInterpolate(const Vector3D& rVector1, const Vector3D& rVector2, const double fFactor) const
		{
			return (*this) * (1.0f - fFactor) * (1.0f - fFactor) + 2.0f * rVector1 *
				   fFactor * (1.0f - fFactor) + rVector2 * fFactor * fFactor;
		}

//	public:
		union
		{
			//! X, Y, Z array
			double m_fPoint[3];

			struct
			{
				//! X coordinate
				double m_fX;

				//! Y coordinate
				double m_fY;

				//! Z coordinate
				double m_fZ;
			};
		};
//	};

	//! Divide Vector by scalar
//	inline const Vector3D operator / (const Vector3D& pVector, const double fDividend)
//	{
		// Return division result
//		return Vector3D(pVector[0] / fDividend, pVector[1] / fDividend, pVector[2] / fDividend);
//	}

	//! Multiply Vector by scalar
/*	inline Vector3D operator * (const double fOperand, const Vector3D& pVector)
	{
		// Return multiplication result
		return Vector3D(fOperand * pVector[0], fOperand * pVector[1], fOperand * pVector[2]);
	}*/

	//! Calculate normal from 3 Points
	inline static Vector3D CalculateNormal(Vector3D pTriangle[])
	{
		// Calculate direction from first two Points
		Vector3D v1 = pTriangle[0] - pTriangle[2];

		// Calculate direction from second two Points
		Vector3D v2 = pTriangle[0] - pTriangle[1];

		// Take the cross product
		Vector3D vResult(v1 ^ v2);

		// Normalize the vector
		vResult |= 1;

		// Return result
		return vResult;
	}

	//! Return the angle between 2 vectors
	inline static double AngleBetweenVectors(Vector3D& pVector1, Vector3D& pVector2)
	{
		// Get the dot product of the vectors
		double fDotProduct = pVector1 % pVector2;				

		// Get the product of both of the vectors magnitudes
		double fVectorMagnitude = !pVector1 * !pVector2;

		// Get the arc cosine of the (fDotProduct / fVectorMagnitude) in radians
		double fAngle = acosf(fDotProduct / fVectorMagnitude);

		// Make sure that the angle is not a -1.#IND0000000 number, which means indefinate.
		if (_isnan(fAngle))
			return 0;

		// Return the angle in radians
		return fAngle;
	}

	//! Return the vector between 2 Points
	inline static Vector3D VectorOfTwoPoints(Vector3D& pVector1, Vector3D& pVector2)
	{
		// Return result of second Structs::Point subtracted from first Structs::Point
		return Vector3D(pVector1 - pVector2);	
	}

	//! Return the distance between 2 Points
	inline static double DistanceBetweenTwoPoints(Vector3D& pVector1, Vector3D& pVector2)
	{
		// Calculate distance between two Points
		double fDistance = sqrtf((pVector2[0] - pVector1[0]) * (pVector2[0] - pVector1[0]) +
                                (pVector2[1] - pVector1[1]) * (pVector2[1] - pVector1[1]) +
                                (pVector2[2] - pVector1[2]) * (pVector2[2] - pVector1[2]));
		// Return result
		return fDistance;
	}

	//! Return distance from plane to origin
	inline static double PlaneDistance(Vector3D& pPoint, Vector3D& pNormal)
	{
		double fDistance = 0;									

		// Use the plane equation to find the distance (D = Ax + By + Cz)
		fDistance = -((pNormal[0] * pPoint[0]) + (pNormal[1] * pPoint[1]) + (pNormal[2] * pPoint[2]));

		// Return result
		return fDistance;
	}

//   double m_x, m_y, m_z;

   protected:
   private:
};

#endif
