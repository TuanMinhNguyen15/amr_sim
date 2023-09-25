#include "amr_sim/shapes.h"

/* Triangle */

Triangle::Triangle(Params params):params_(params)
{}

void Triangle::GetParams(Params &params)
{
    params = params_;
}

void Triangle::SetParams(const Params &params)
{
    params_ = params;
}

std::string Triangle::GetShape()
{
    return "triangle";
}


bool Triangle::isInside(const olc::vf2d &p)
{
    olc::vf2d p1 = params_.p1;
    olc::vf2d p2 = params_.p2;
    olc::vf2d p3 = params_.p3;

    // Calculate the vectors
    olc::vf2d v0 = p2 - p1;
    olc::vf2d v1 = p3 - p1;
    olc::vf2d v2 = p  - p1;

    // Calculate dot products
    float dot00 = v0.dot(v0);
    float dot01 = v0.dot(v1);
    float dot02 = v0.dot(v2);
    float dot11 = v1.dot(v1);
    float dot12 = v1.dot(v2);

    // Calculate barycentric coordinates
    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
    float v = (dot00 * dot12 - dot01 * dot02) * invDenom;
    float w = 1 - u - v;

    return (u >= 0 && u <= 1 && v >= 0 && v <= 1 && w >= 0 && w <= 1);
}

/* Rectangle */

Rectangle::Rectangle(Params params):params_(params)
{}

void Rectangle::UpdateVertices()
{
    float W = params_.W;
    float H = params_.H;
    float theta = params_.theta;
    olc::vf2d pCenter = params_.pCenter;

    // find diagonal length
    float D = std::sqrt(std::pow(W,2) + std::pow(H,2));
    
    // find angles of 4 vertices
    float theta2 = std::asin(H/D) + theta;
    float theta1 = theta2 + std::asin(W/D) + theta;
    float theta3 = -theta2 + theta;
    float theta4 = -theta1 + theta;

    // update the 4 vertices
    olc::vf2d v = (D/2.)*olc::vf2d(1,0);
    olc::vf2d v1,v2,v3,v4;

    Rotate(v,theta1,v1);
    Rotate(v,theta2,v2);
    Rotate(v,theta3,v3);
    Rotate(v,theta4,v4);

    p1_ = pCenter + v1;
    p2_ = pCenter + v2;
    p3_ = pCenter + v3;
    p4_ = pCenter + v4;
}

void Rectangle::GetParams(Params &params)
{
    params = params_;
}

void Rectangle::SetParams(const Params &params)
{
    params_ = params;
    UpdateVertices();
}

void Rectangle::GetVertices(std::vector<TriangleVertices> &vertices)
{
    vertices.clear();
    vertices.push_back(TriangleVertices{p1_,p2_,p3_});
    vertices.push_back(TriangleVertices{p1_,p3_,p4_});
}


std::string Rectangle::GetShape()
{
    return "rectangle";
}

void Rotate(const olc::vf2d &pIn, const float &theta, olc::vf2d &pOut)
{
    pOut.x = std::cos(theta)*pIn.x - std::sin(theta)*pIn.y;
    pOut.y = std::sin(theta)*pIn.x + std::cos(theta)*pIn.y;
}