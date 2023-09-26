#include "amr_sim/olcPixelGameEngine.h"

#pragma once

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
        Triangle();

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
        Rectangle();

        void GetParams(Params &params);
        void SetParams(const Params &params);
        void GetInternalTriangles(Triangle &upperTriangle, Triangle &lowerTriangle);
        std::string GetShape() override;
        bool isInside(const olc::vf2d &p) override;
        
    private:
        // rectangle parameters
        Params params_;
        // 2 internal triangles
        Triangle upperTriangle_,lowerTriangle_;

    private:
        void UpdateVertices();

};

void Rotate(const olc::vf2d &pIn, const float &theta, olc::vf2d &pOut);
