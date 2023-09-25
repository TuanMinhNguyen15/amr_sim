#include "amr_sim/olcPixelGameEngine.h"

#pragma once

struct TriangleVertices
{
    TriangleVertices(){}
    TriangleVertices(olc::vf2d p1, olc::vf2d p2, olc::vf2d p3):p1(p1),p2(p2),p3(p3){}

    olc::vf2d p1,p2,p3;
};

class Shape 
{
    public:
        virtual bool isInside(const olc::vf2d &p) = 0;
        virtual std::string GetShape() = 0;
};

class Rectangle : public Shape
{				
    public:
        Rectangle(olc::vf2d pCenter, float W, float H, float theta);

        void UpdateCenter(const olc::vf2d &pCenter);
        void UpdateWH(const float &W, const float &H);
        void GetVertices(std::vector<TriangleVertices> &vertices);
        std::string GetShape() override;
        
    private:
        // center point coordinate
        olc::vf2d pCenter_;
        // width and height
        float W_,H_;
        // rotation angle
        float theta_;
        // 4 vectices of rectangle
        olc::vf2d p1_,p2_,p3_,p4_;

    private:
        void UpdateVertices();

};

void Rotate(const olc::vf2d &pIn, const float &theta, olc::vf2d &pOut);
