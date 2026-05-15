#include "../../main.h"
#include "../../game/game.h" // for MATRIX/PMATRIX structs
#include "quaternion.h"
#include <algorithm>
#include <cmath>

void CQuaternion::SetFromMatrix(MATRIX4X4 mat)
{
    _w = sqrt(std::max((float)0, 1.0f + mat.right.X + mat.up.Y + mat.at.Z)) * 0.5f;
    _x = sqrt(std::max((float)0, 1.0f + mat.right.X - mat.up.Y - mat.at.Z)) * 0.5f;
    _y = sqrt(std::max((float)0, 1.0f - mat.right.X + mat.up.Y - mat.at.Z)) * 0.5f;
    _z = sqrt(std::max((float)0, 1.0f - mat.right.X - mat.up.Y + mat.at.Z)) * 0.5f;

    _x = static_cast <float> (copysign(_x, mat.at.Y - mat.up.Z));
    _y = static_cast <float> (copysign(_y, mat.right.Z - mat.at.X));
    _z = static_cast <float> (copysign(_z, mat.up.X - mat.right.Y));
}

void CQuaternion::GetAsMatrix(PMATRIX4X4 mat)
{
    if (!mat) return;

    float sqw = _w * _w;					// v13 = a1 * a1;
    float sqx = _x * _x;					// v14 = a2 * a2;
    float sqy = _y * _y;					// v15 = a3 * a3;
    float sqz = _z * _z;					// v16 = a4 * a4;

    mat->right.X = (sqx - sqy - sqz + sqw); // a5 = v14 - v15 - v16 + v13;
    mat->up.Y = (-sqx + sqy - sqz + sqw);	// a9 = v15 - v14 - v16 + v13;
    mat->at.Z = (-sqx - sqy + sqz + sqw);	// a13 = v16 - (v15 + v14) + v13;

    float tmp1 = _x * _y;					// v17 = a2 * a3;
    float tmp2 = _z * _w;					// v18 = a1 * a4;
    mat->up.X = 2.0 * (tmp1 + tmp2);		// a8 = v18 + v17 + v18 + v17;
    mat->right.Y = 2.0 * (tmp1 - tmp2);		// a6 = v17 - v18 + v17 - v18;

    tmp1 = _x * _z;							// v20 = a2 * a4;
    tmp2 = _y * _w;							// v21 = a1 * a3;
    mat->at.X = 2.0 * (tmp1 - tmp2);		// a11 = v20 - v21 + v20 - v21;
    mat->right.Z = 2.0 * (tmp1 + tmp2);		// a7 = v21 + v20 + v21 + v20;
    tmp1 = _y * _z;							// v22 = a3 * a4;
    tmp2 = _x * _w;							// v23 = a1 * a2;
    mat->at.Y = 2.0 * (tmp1 + tmp2);		// a12 = v23 + v22 + v23 + v22;
    mat->up.Z = 2.0 * (tmp1 - tmp2);		// a10 = v22 - v23 + v22 - v23;
}

void CQuaternion::Normalize()
{
    double n = sqrt(_x * _x + _y * _y + _z * _z + _w * _w);

    _w /= n;
    _x /= n;
    _y /= n;
    _z /= n;
}

#define SLERP_DELTA 0.01
void CQuaternion::Slerp(CQuaternion *pQ1, CQuaternion *pQ2, float t)
{
    CQuaternion *pOut = this;

    if (!pQ1 || !pQ2) return;
    if (t > 1) return;

    float p1[4];
    double omega, cosom, sinom, scale0, scale1;
    cosom = pQ1->_x*pQ2->_x + pQ1->_y*pQ2->_y + pQ1->_z*pQ2->_z + pQ1->_w*pQ2->_w;

    if (cosom < 0.0)
    {
        cosom = -cosom;
        p1[0] = -pQ2->_x;  p1[1] = -pQ2->_y;
        p1[2] = -pQ2->_z;  p1[3] = -pQ2->_w;
    }
    else
    {
        p1[0] = pQ2->_x;    p1[1] = pQ2->_y;
        p1[2] = pQ2->_z;    p1[3] = pQ2->_w;
    }

    if ((1.0 - cosom) > SLERP_DELTA)
    {
        // ñòàíäàðòíûé ñëó÷àé - SLERP
        omega = acos(cosom);
        sinom = sin(omega);
        scale0 = sin((1.0 - t) * omega) / sinom;
        scale1 = sin(t * omega) / sinom;
    }
    else
    {
        // ìàëåëüíêèé óãîë - ëèíåéíàÿ èíòåðïîëÿöèÿ
        scale0 = 1.0 - t;
        scale1 = t;
    }

    Set(scale0 * pQ1->_x + scale1 * p1[0],
        scale0 * pQ1->_y + scale1 * p1[1],
        scale0 * pQ1->_z + scale1 * p1[2],
        scale0 * pQ1->_w + scale1 * p1[3]);
}