#include "amr_sim/olcPixelGameEngine.h"
#include <list>

#pragma once

class Shape 
{
    public:
        virtual bool IsInside(const olc::vf2d &p) = 0;
        virtual bool AllInside(const std::vector<olc::vf2d> &ps, std::vector<olc::vf2d> &pOutside) = 0;
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
        bool IsInside(const olc::vf2d &p) override;
        bool AllInside(const std::vector<olc::vf2d> &ps, std::vector<olc::vf2d> &pOutside) override;

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
        bool IsInside(const olc::vf2d &p) override;
        bool AllInside(const std::vector<olc::vf2d> &ps, std::vector<olc::vf2d> &pOutside) override;
        
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
        float GetMaxT();

    public:
        std::vector<olc::vf2d> controlPoints;
        bool isLoop;

    private:
        void Select4Points(float &t, olc::vf2d &p0, olc::vf2d &p1, olc::vf2d &p2, olc::vf2d &p3);
        bool IsValid(const float &t);
};		


class Track : public Shape
{
    public:
        struct Params
        {
            // waypoints of road middle line
            std::vector<olc::vf2d> controlPoints;
            // road width
            float w;
        };

    public:
        Track();
        Track(Params params);

        void GetParams(Params &params);
        void SetParams(const Params &params);

        void GetTriangles(std::vector<Triangle> &triangles);

        bool IsInside(const olc::vf2d &p) override;
        bool AllInside(const std::vector<olc::vf2d> &ps, std::vector<olc::vf2d> &pOutside) override;
        std::string GetShape() override;

    private:
        void UpdateSplines();
        void UpdateTriangles();

    private:
        Params params_;
        float res = 0.2;

        Spline innerSpline_,middleSpline_,outerSpline_;
        std::vector<Triangle> triangles_;
};

void Rotate(const olc::vf2d &pIn, const float &theta, olc::vf2d &pOut);
