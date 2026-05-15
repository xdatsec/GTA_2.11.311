#pragma once

/*
	CQuaternion - êëàññ äëÿ ðàáîòû ñ êâàòåðíèîíàìè

	github.com/4x11
	axwellcm@gmail.com
*/
#pragma pack(push, 1)
class CQuaternion
{
public:
    CQuaternion() : _w{ 1.0f }, _x(0.0f), _y(0.0f), _z(0.0f) {};
    CQuaternion(float x, float y, float z, float w)
            : _w(w), _x(x), _y(y), _z(z) {};

    void Set(float x, float y, float z, float w) {
        _w = w;
        _x = x;
        _y = y;
        _z = z;
    }

    void Set(CQuaternion q) {
        _w = q._w;
        _x = q._x;
        _y = q._y;
        _z = q._z;
    }

    float x() { return _x; }
    float y() { return _y; }
    float z() { return _z; }
    float w() { return _w; }

    void SetFromMatrix(MATRIX4X4 mat);
    void GetAsMatrix(PMATRIX4X4 mat);
    void Normalize();
    void Slerp(CQuaternion *pQ1, CQuaternion *pQ2, float t);

public:
    float _w;
    float _x;
    float _y;
    float _z;
};
#pragma pack(pop)