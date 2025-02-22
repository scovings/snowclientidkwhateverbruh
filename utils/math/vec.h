#pragma once
#include <algorithm>
#include <cmath> // Include cmath for sqrtf and NAN

class Vector3
{
public:
    float x{ 0.0f }; // Initialize to 0 instead of NAN
    float y{ 0.0f };
    float z{ 0.0f };

    Vector3() = default; // Default constructor
    Vector3(const float x, const float y, const float z) : x(x), y(y), z(z) {}

    Vector3 operator + (const Vector3& rhs) const { return Vector3(x + rhs.x, y + rhs.y, z + rhs.z); }
    Vector3 operator + (const float rhs) const { return Vector3(x + rhs, y + rhs, z + rhs); }
    Vector3 operator - (const Vector3& rhs) const { return Vector3(x - rhs.x, y - rhs.y, z - rhs.z); }
    Vector3 operator - (const float rhs) const { return Vector3(x - rhs, y - rhs, z - rhs); }
    Vector3 operator * (const Vector3& rhs) const { return Vector3(x * rhs.x, y * rhs.y, z * rhs.z); }
    Vector3 operator * (const float rhs) const { return Vector3(x * rhs, y * rhs, z * rhs); }
    Vector3 operator / (const float rhs) const { return Vector3(x / rhs, y / rhs, z / rhs); }

    Vector3& operator += (const Vector3& rhs) { return *this = *this + rhs; }
    Vector3& operator -= (const Vector3& rhs) { return *this = *this - rhs; }
    Vector3& operator *= (const float rhs) { return *this = *this * rhs; }
    Vector3& operator /= (const float rhs) { return *this = *this / rhs; }

    float Length() const { return sqrtf(x * x + y * y + z * z); }
    
    Vector3 Normalize() const {
        float len = Length();
        if (len == 0) return Vector3(0, 0, 0); // Handle zero length
        return *this * (1.0f / len);
    }

    Vector3 Invert() const { return Vector3{ -x, -y, -z }; }
    float Distance(const Vector3& rhs) const { return (*this - rhs).Length(); }
};

struct Vector2
{
    float x{ 0.0f }; // Initialize to 0 instead of NAN
    float y{ 0.0f };

    Vector2() = default; // Default constructor
    Vector2(float x, float y) : x(x), y(y) {}

    Vector2 operator - (const Vector2& rhs) const { return Vector2(x - rhs.x, y - rhs.y); }
    Vector2 Invert() const { return Vector2{ -x, -y }; }
};

struct Vector2i
{
    unsigned int x{ 0 };
    unsigned int y{ 0 };
};

struct Vector4
{
    float x{ 0.0f }; // Initialize to 0 instead of NAN
    float y{ 0.0f };
    float z{ 0.0f };
    float w{ 0.0f };

    Vector4() = default; // Default constructor
    Vector4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
};

// For readability & maybe security purposes.
struct Matrix
{
    float m00, m01, m02, m03;
    float m10, m11, m12, m13;
    float m20, m21, m22, m23;
    float m30, m31, m32, m33;
};

struct BoundingBox {
    double minX;
    double minY;
    double minZ;
    double maxX;
    double maxY;
    double maxZ;
};