#include "amr_sim/shapes.h"

/* Rectangle */

Rectangle::Rectangle(olc::vf2d pCenter, float W, float H, float theta):pCenter_(pCenter),W_(W),H_(H),theta_(theta)
{}

void Rectangle::UpdateVertices()
{
    // find diagonal length
    float D = std::sqrt(std::pow(W_,2) + std::pow(H_,2));
    
    // find angles of 4 vertices
    float theta2 = std::asin(H_/D) + theta_;
    float theta1 = theta2 + std::asin(W_/D) + theta_;
    float theta3 = -theta2 + theta_;
    float theta4 = -theta1 + theta_;

    // update the 4 vertices
    olc::vf2d v = (D/2.)*olc::vf2d(1,0);
    olc::vf2d v1,v2,v3,v4;

    Rotate(v,theta1,v1);
    Rotate(v,theta2,v2);
    Rotate(v,theta3,v3);
    Rotate(v,theta4,v4);

    p1_ = pCenter_ + v1;
    p2_ = pCenter_ + v2;
    p3_ = pCenter_ + v3;
    p4_ = pCenter_ + v4;
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