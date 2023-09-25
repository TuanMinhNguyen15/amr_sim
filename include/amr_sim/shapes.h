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

class Triangle : public Shape
{
    public:
        struct Params
        {
            // 3 vertices of triangle
            olc::vf2d p1,p2,p3;
        };

    public:
        Triangle(Params params);

        void GetParams(Params &params);
        void SetParams(const Params &params);
        std::string GetShape() override;
        bool isInside(const olc::vf2d &p) override;

    private:
        Params params_;
};

class Rectangle : public Shape
{			
    public:
        struct Params
        {
            // center point coordinate
            olc::vf2d pCenter;
            // width and height
            float W,H;
            // rotation angle
            float theta;
        };

    public:
        Rectangle(Params param);

        void GetParams(Params &params);
        void SetParams(const Params &params);
        void GetVertices(std::vector<TriangleVertices> &vertices);
        std::string GetShape() override;
        
    private:
        // rectangle parameters
        Params params_;
        // 4 vectices of rectangle
        olc::vf2d p1_,p2_,p3_,p4_;

    private:
        void UpdateVertices();

};

void Rotate(const olc::vf2d &pIn, const float &theta, olc::vf2d &pOut);
