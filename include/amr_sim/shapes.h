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
        void GetTriangles(Triangle &upperTriangle, Triangle &lowerTriangle);
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

struct Spline 
{
    public:
        Spline();
        Spline(bool isLoop);
        Spline(int pNum, bool isLoop);

        bool Interpolate(float t, olc::vf2d &p);
        bool GetGradient(float t, olc::vf2d &p);
        int GetMaxT();

    public:
        std::vector<olc::vf2d> controlPoints;
        bool isLoop;

    private:
        void Select4Points(float &t, olc::vf2d &p0, olc::vf2d &p1, olc::vf2d &p2, olc::vf2d &p3);
        bool IsValid(const float &t);
};		


class Road : public Shape
{
    public:
        struct Params
        {
            // waypoints of road middle line
            std::vector<olc::vf2d> controlPoints;
            // road width
            float w;
            // road color
            olc::Pixel color;
        };

    public:
        Road();
        Road(Params params);

        void GetParams(Params &params);
        void SetParams(const Params &params);

        // int GetMaxT();
        // void Interpolate(olc::vf2d &p);
        // void GetGradient(olc::vf2d &p);

        void GetTriangles(std::vector<Triangle> &triangles);

    private:
        Params params_;

        Spline innerSpline_,middleSpline_,outerSpline_;
};

void Rotate(const olc::vf2d &pIn, const float &theta, olc::vf2d &pOut);
