#include "Quaternion.h"
#include "Matrix.h"
#include "Vector3D.h"
#include "MyMath.h"
#include <math.h>

MyMath::Quaternion MyMath::SetQuaternion(const MyMath::Vector3D& v, float angle)
{
    Vector3D vec = v;
    vec.Normalize();

    Quaternion ans;
    float rad = sinf(angle / 2);
    ans.x = v.x * rad;
    ans.y = v.y * rad;
    ans.z = v.z * rad;
    ans.w = cosf(angle / 2);

    Normalize(ans);
    return ans;
}

MyMath::Quaternion::Quaternion(float w, float x, float y, float z) :w(w), x(x), y(y), z(z) {}

MyMath::Quaternion::Quaternion(float w, const Vector3D& vec) :w(w), x(vec.x), y(vec.y), z(vec.z) {}

MyMath::Quaternion MyMath::Quaternion::SetQuaternion(const Vector3D& v)
{
    Quaternion r;
    r.x = v.x;
    r.y = v.y;
    r.z = v.z;
    r.w = 0;

    Quaternion q = *this;
    r = CalcProduct(q, r);
    r = CalcProduct(r, q.GetConjugation());

    return r;
}

MyMath::Vector3D MyMath::Quaternion::GetVector3()
{
    return Vector3D(x, y, z);
}

void MyMath::Quaternion::SetVec(const MyMath::Vector3D& vec)
{
    x = vec.x;
    y = vec.y;
    z = vec.z;
    w = 0.0f;
}

MyMath::Quaternion MyMath::Quaternion::CalcProduct(const Quaternion& q, const Quaternion& r)
{
    Quaternion ans;
    ans.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
    ans.x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    ans.y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    ans.z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;

    return ans;
}

MyMath::Quaternion MyMath::Quaternion::GetIdentity()
{
    Quaternion ans;
    ans.x = 0;
    ans.y = 0;
    ans.z = 0;
    ans.w = 1;

    return ans;
}

MyMath::Quaternion MyMath::Quaternion::GetConjugation()
{
    Quaternion ans;
    ans.x = -x;
    ans.y = -y;
    ans.z = -z;
    ans.w = w;

    return ans;
}

void MyMath::Quaternion::Normalize()
{
    float len = sqrtf(w * w + x * x + y * y + z * z);

    if (len != 0.0f)
    {
        x /= len;
        y /= len;
        z /= len;
        w /= len;
    }
}

MyMath::Matrix MyMath::Quaternion::GetRotMatrix()
{
    Matrix mat;
    mat.m[0][0] = w * w + x * x - y * y - z * z;
    //mat.m[0][0] = 1 - (2 * y * y) - (2 * z * z);
    mat.m[0][1] = (2 * x * y) + (2 * w * z);
    mat.m[0][2] = (2 * x * z) - (2 * w * y);

    mat.m[1][0] = (2 * x * y) - (2 * w * z);
    mat.m[1][1] = 1 - (2 * x * x) - (2 * z * z);
    mat.m[1][2] = (2 * y * z) + (2 * w * x);

    mat.m[2][0] = (2 * x * z) + (2 * w * y);
    mat.m[2][1] = (2 * y * z) - (2 * w * x);
    //mat.m[2][2] = 1 - (2 * x * x) - (2 * y * y);
    mat.m[2][2] = w * w - x * x - y * y + z * z;

    return mat;
}

MyMath::Quaternion MyMath::operator-(const Quaternion& q)
{
    return Quaternion(-q.w, -q.x, -q.y, -q.z);
}

MyMath::Quaternion& MyMath::Quaternion::operator/=(float s)
{
    x /= s;
    y /= s;
    z /= s;
    w /= s;
    return *this;
}

MyMath::Quaternion& MyMath::Quaternion::operator*=(float s)
{
    x *= s;
    y *= s;
    z *= s;
    w *= s;
    return *this;
}

MyMath::Quaternion& MyMath::Quaternion::operator*=(const Quaternion& q)
{
    Quaternion temp(*this);
    w = -temp.x * q.x - temp.y * q.y - temp.z * q.z + temp.w * q.w;
    x =  temp.x * q.w + temp.y * q.z - temp.z * q.y + temp.w * q.x;
    y = -temp.x * q.z + temp.y * q.w + temp.z * q.x + temp.w * q.y;
    z =  temp.x * q.y - temp.y * q.x + temp.z * q.w + temp.w * q.z;
    return *this;
}

MyMath::Quaternion& MyMath::Quaternion::operator+=(const Quaternion& q)
{
    x += q.x;
    y += q.y;
    z += q.z;
    w += q.w;
    return *this;
}

MyMath::Quaternion& MyMath::Quaternion::operator-=(const Quaternion& q)
{
    x -= q.x;
    y -= q.y;
    z -= q.z;
    w -= q.w;
    return *this;
}

float MyMath::Quaternion::Dot(const Quaternion& q)
{
    return x * q.x + y * q.y + z * q.z + w * q.w;
}

MyMath::Quaternion MyMath::operator*(const Quaternion& q, float s)
{
    Quaternion temp(q);
    return temp *= s;
}

MyMath::Quaternion MyMath::operator*(float s, const Quaternion& q)
{
    return q * s;
}

MyMath::Quaternion MyMath::operator*(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion temp(q1);
    return temp *= q2;
}

MyMath::Quaternion MyMath::operator+(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion temp(q1);
    return temp += q2;
}

MyMath::Quaternion MyMath::operator-(const Quaternion& q1, const Quaternion& q2)
{
    Quaternion ans = q1;
    return ans -= q2;
}

MyMath::Quaternion MyMath::Multiply(const Quaternion& q, const Quaternion& r)
{
    Quaternion ans;
    ans.w = q.w * r.w - q.x * r.x - q.y * r.y - q.z * r.z;
    ans.x = q.y * r.z - q.z * r.y + r.w * q.x + q.w * r.x;
    ans.y = q.z * r.x - q.x * r.z + r.w * q.y + q.w * r.y;
    ans.z = q.x * r.y - q.y * r.x + r.w * q.z + q.w * r.z;

    return ans;
}

MyMath::Quaternion MyMath::IdentityQuaternion()
{
    return Quaternion(1.0f, 0.0f, 0.0f, 0.0f);
}

MyMath::Quaternion MyMath::Conjugate(const Quaternion& quaternion)
{
    Quaternion ans;
    ans.x = -quaternion.x;
    ans.y = -quaternion.y;
    ans.z = -quaternion.z;
    ans.w = quaternion.w;

    return ans;
}

float MyMath::Norm(const Quaternion& quaternion)
{
    return sqrtf(quaternion.w * quaternion.w + quaternion.x * quaternion.x + quaternion.y * quaternion.y + quaternion.z * quaternion.z);
}

float MyMath::GetAngle(const Vector3D& axis, const Vector3D& v)
{
    Quaternion qAxis(0.0f, axis);
    Quaternion qV(0.0f, v);
    qAxis.Normalize();
    qV.Normalize();

    return acosf(qAxis.Dot(qV));
}

MyMath::Quaternion MyMath::Normalize(const Quaternion& quaternion)
{
    float len = Norm(quaternion);

    Quaternion ans = quaternion;
    if (len != 0.0f)
    {
        ans.x /= len;
        ans.y /= len;
        ans.z /= len;
        ans.w /= len;
    }

    return ans;
}

MyMath::Quaternion MyMath::Inverse(const Quaternion& quaternion)
{
    Quaternion ans = quaternion;
    float norm = Norm(ans);
    ans /= norm;
    ans /= norm;
    return ans;
}

MyMath::Quaternion MyMath::MakeAxisAngle(const Vector3D& axis, float angle)
{
    Quaternion ans;
    Vector3D axis_ = axis;
    axis_.Normalize();

    float rad = sinf(angle / 2);
    ans.x = axis_.x * rad;
    ans.y = axis_.y * rad;
    ans.z = axis_.z * rad;
    ans.w = cosf(angle / 2);

    ans.Normalize();

    return ans;
}

MyMath::Vector3D MyMath::RotateVector(const Vector3D& vec, const Quaternion& quaternion)
{
    Quaternion vecQ;
    vecQ.SetVec(vec);
    Quaternion q = quaternion;
    q.Normalize();
    Quaternion qConjugate = q.GetConjugation();
    vecQ = Multiply(q, vecQ);
    vecQ = Multiply(vecQ, qConjugate);

    return vecQ.GetVector3();
}

MyMath::Matrix MyMath::MakeRotateMatrix(const Quaternion& quaternion)
{
    Matrix mat;
    mat.m[0][0] = quaternion.w * quaternion.w + quaternion.x * quaternion.x - quaternion.y * quaternion.y - quaternion.z * quaternion.z;
    mat.m[0][0] = 1 - (2 * quaternion.y * quaternion.y) - (2 * quaternion.z * quaternion.z);
    mat.m[0][1] = (2 * quaternion.x * quaternion.y) + (2 * quaternion.w * quaternion.z);
    mat.m[0][2] = (2 * quaternion.x * quaternion.z) - (2 * quaternion.w * quaternion.y);

    mat.m[1][0] = (2 * quaternion.x * quaternion.y) - (2 * quaternion.w * quaternion.z);
    mat.m[1][1] = 1 - (2 * quaternion.x * quaternion.x) - (2 * quaternion.z * quaternion.z);
    mat.m[1][2] = (2 * quaternion.y * quaternion.z) + (2 * quaternion.w * quaternion.x);

    mat.m[2][0] = (2 * quaternion.x * quaternion.z) + (2 * quaternion.w * quaternion.y);
    mat.m[2][1] = (2 * quaternion.y * quaternion.z) - (2 * quaternion.w * quaternion.x);
    mat.m[2][2] = 1 - (2 * quaternion.x * quaternion.x) - (2 * quaternion.y * quaternion.y);
    mat.m[2][2] = quaternion.w * quaternion.w - quaternion.x * quaternion.x - quaternion.y * quaternion.y + quaternion.z * quaternion.z;

    return mat;
}

MyMath::Quaternion MyMath::Slerp(const Quaternion& q0, const Quaternion& q1, float t)
{
    Quaternion q0_ = q0;
    float dot = q0_.Dot(q1);

    if (dot < 0.0f) {
        q0_ = -q0;
        dot = -dot;
    }

    float scale0 = (1.0f - t);
    float scale1 = t;
    if (dot >= 1.0f - 0.001f) {
        return q0_ * (1.0f - t) + t * q1;
    }
    float theta = (float)acos(dot);

    scale0 = (float)(sin(theta * scale0)) / sinf(theta);
    scale1 = (float)(sin(theta * scale1)) / sinf(theta);

    return q0_ * scale0 + scale1 * q1;
}

MyMath::Quaternion MyMath::CameraSlerp(const Quaternion& q0, const Quaternion& q1, float t)
{
    Quaternion q0_ = q0;
    float dot = q0_.Dot(q1);

    float scale0 = (1.0f - t);
    float scale1 = t;
    if (dot >= 1.0f - 0.001f) {
        return q0_ * (1.0f - t) + t * q1;
    }
    float theta = (float)acos(dot);

    scale0 = (float)(sin(theta * scale0)) / sinf(theta);
    scale1 = (float)(sin(theta * scale1)) / sinf(theta);

    return q0_ * scale0 + scale1 * q1;
}



MyMath::Quaternion MyMath::DirectionToDirection(const Vector3D& u, const Vector3D& v)
{
    Vector3D vecU = u;

    float dot = vecU.dot(v);

    //  uの逆ベクトルがvだったら(crossの値がゼロベクトルにならないように)
    if (dot == -1) {
        vecU.x = v.y;
        vecU.y = v.z;
        vecU.z = v.x;
    }
    Vector3D cross = vecU.cross(v);
    cross.Normalize();

    dot = MyMath::mClamp(-1.0f, 1.0f, dot);
    float theta = (float)acos(dot);

    return MakeAxisAngle(cross, theta);
}

MyMath::Vector3D MyMath::GetAxis(const Quaternion& q)
{
    Vector3D result;

    float len = Norm(q);

    result.x = q.x / len;
    result.y = q.y / len;
    result.z = q.z / len;

    return result;
}
