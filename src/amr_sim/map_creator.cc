#include "amr_sim/map_creator.h"

MapCreator::MapCreator():Base("Map Creator")
{
    shapesPtr_.clear();
}


bool MapCreator::OnUserCreate()
{
    return true;
}

void MapCreator::Home()
{
    // show options
    DrawString(0,0," Press: 1. Add Triangle     2. Add Rectangle    3. Add Circle    4. Add Track   5. Export",olc::BLACK);

    // check user-input
    if (GetKey(olc::Key::K1).bPressed)
    {
        /* add triangle */
        shapeOption_ = ShapeOption::TRIANGLE;
        stateMachine_ = StateMachine::CREATE;
    }
    else if (GetKey(olc::Key::K2).bPressed)
    {
        /* add rectangle */
        shapeOption_ = ShapeOption::RECTANGLE;
        stateMachine_ = StateMachine::CREATE;

        // reset
        isSeleted_ = false;
    }
    else if (GetKey(olc::Key::K3).bPressed)
    {
        /* add circle */
        shapeOption_ = ShapeOption::CIRCLE;
        stateMachine_ = StateMachine::CREATE;
    }
    else if (GetKey(olc::Key::K4).bPressed)
    {
        /* add track */
        shapeOption_ = ShapeOption::TRACK;
        stateMachine_ = StateMachine::CREATE;
    }
    else if (GetKey(olc::Key::K5).bPressed)
    {
        /* export */
        stateMachine_ = StateMachine::EXPORT;
    }

    // check if user selected any shapes
    // get mouse input
    if (GetMouse(0).bPressed)
    {
        for (Shape *shapePtr : shapesPtr_)
        {
            if (shapePtr->IsInside(mousePos_))
            {
                // set shape to be editted
                shapeEditPtr_ = shapePtr;
                // change state to EDIT
                stateMachine_ = StateMachine::EDIT;
                // reset
                isSeleted_ = false;
                indexSelected_ = -1;
            }
        }
    }
}

void MapCreator::Create()
{
    switch (shapeOption_)
    {
        case ShapeOption::TRIANGLE:
        {
            DrawString(0,0," Select 3 points to draw a triangle",olc::BLACK);
            
            // mouse pressed
            if (GetMouse(0).bPressed && !GetKey(olc::Key::X).bHeld)
            {
                // get mouse world coordinate as a control point

                controlPoints_Triangle_[numPoint_Triangle_] = mousePos_;
                
                // increase control point count
                numPoint_Triangle_++;

                if (numPoint_Triangle_ == 3)
                {
                    // create triangle
                    Triangle::Params params;
                    params.p1 = controlPoints_Triangle_[0];
                    params.p2 = controlPoints_Triangle_[1];
                    params.p3 = controlPoints_Triangle_[2];
                    Triangle *trianglePtr = new Triangle(params);
                    // append to shapes
                    shapesPtr_.push_back(trianglePtr);
                    // shape to be editted
                    shapeEditPtr_ = trianglePtr;
                    // reset control point count
                    numPoint_Triangle_ = 0;
                    // change state machine to EDIT
                    stateMachine_ = StateMachine::EDIT;
                    // reset
                    isSeleted_ = false;
                    indexSelected_ = -1;
                }              
            }

            // draw control points
            for (int i = 0; i < numPoint_Triangle_; i++)
            {
                olc::vf2d controlPoint;
                controlPoint = controlPoints_Triangle_[i];
                FillCircle(WorldToScreen(controlPoint),WorldToScreen(r_),controlColor_);
            }
            break;
        }
        case ShapeOption::RECTANGLE:
        {
            DrawString(0,0," Hold and drag to draw a rectangle",olc::BLACK);

            if (GetMouse(0).bPressed)
            {
                // capture mouse position
                mousePosPrev_ = mousePos_;
                isSeleted_ = true;
            }
            else if (GetMouse(0).bHeld)
            {
                // difference vector
                olc::vf2d mouseDiff = mousePos_ - mousePosPrev_;
                // center
                recParams_.pCenter = mousePosPrev_ + mouseDiff/2.;
                // width and height
                recParams_.W = std::abs(mouseDiff.x);
                recParams_.H = std::abs(mouseDiff.y);
                // instantiate rectangle
                Rectangle rectangle(recParams_);

                // draw demo rectangle
                Triangle upperTriangle,lowerTriangle;
                Triangle::Params triParams;
                rectangle.GetTriangles(upperTriangle,lowerTriangle);
                upperTriangle.GetParams(triParams);
                FillTriangle(WorldToScreen(triParams.p1),WorldToScreen(triParams.p2),WorldToScreen(triParams.p3),obstacleColor_);
                lowerTriangle.GetParams(triParams);
                FillTriangle(WorldToScreen(triParams.p1),WorldToScreen(triParams.p2),WorldToScreen(triParams.p3),obstacleColor_);
            }
            else
            {
                if (isSeleted_)
                {
                    // create rectangle
                    Rectangle *rectanglePtr = new Rectangle(recParams_);
                    // append shape to shapeptr
                    shapesPtr_.push_back(rectanglePtr);
                    // shape to be edited
                    shapeEditPtr_ = rectanglePtr;
                    // change to EDIT state
                    stateMachine_ = StateMachine::EDIT;
                    // reset
                    isSeleted_ = false;
                }
            }
            break;
        }
        case ShapeOption::CIRCLE:
        {
            break;
        }
        case ShapeOption::TRACK:
        {
            break;
        }
    }
    
}

void MapCreator::Edit()
{
    // prompt user
    DrawString(0,0," Click and drag control points to edit\n\n Click and drag shape to move it around \n\n Press D to delete \n\n Press ESC to return HOME",olc::BLACK);

    std::string shapeType = shapeEditPtr_->GetShape();

    if (shapeType == "triangle")
    {
        // extract pointer of triangle class
        Triangle *trianglePtr = dynamic_cast<Triangle*>(shapeEditPtr_);
        Triangle::Params params;
        trianglePtr->GetParams(params);


        /* check if user selected any control points */
        // get triangle's control points
        std::vector<olc::vf2d> pVec = {params.p1,params.p2,params.p3};
        
        // get mouse input
        if (GetMouse(0).bPressed)
        {
            // reset
            isSeleted_ = false;
            indexSelected_ = -1;

            if (IsSelected(mousePos_,pVec,r_,indexSelected_) || trianglePtr->IsInside(mousePos_))
            {
                // check if either a control point or triangle is selected
                isSeleted_ = true;
                
                if (indexSelected_ == -1)
                {
                    // triangle is selected
                    // record mouse position
                    mousePosPrev_ = mousePos_;
                    // record triangle's control points
                    p1Prev_ = params.p1;
                    p2Prev_ = params.p2;
                    p3Prev_ = params.p3;
                }
            }

        }
        else if (GetMouse(0).bHeld && !GetKey(olc::Key::X).bHeld)
        {
            if (isSeleted_)
            {
                if (indexSelected_ == 0)
                {
                    // control point is selected
                    params.p1 = mousePos_;
                }
                else if (indexSelected_ == 1)
                {
                    // control point is selected
                    params.p2 = mousePos_;
                }
                else if (indexSelected_ == 2)
                {
                    // control point is selected
                    params.p3 = mousePos_;
                }
                else
                {
                    // triangle is selected
                    // find diff vector of mouse position
                    olc::vf2d mouseDiff = mousePos_ - mousePosPrev_;
                    // update triangle's control points
                    params.p1 = p1Prev_ + mouseDiff;
                    params.p2 = p2Prev_ + mouseDiff;
                    params.p3 = p3Prev_ + mouseDiff;
                }
            }
        }

        // update triangle params
        trianglePtr->SetParams(params);

        // draw control points
        FillCircle(WorldToScreen(params.p1),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(params.p2),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(params.p3),WorldToScreen(r_),controlColor_);
    }
    else if (shapeType == "rectangle")
    {
        // extract pointer of rectangle class
        Rectangle *rectanglePtr = dynamic_cast<Rectangle*>(shapeEditPtr_);

        // get 4 corners of rectangle
        olc::vf2d p1,p2,p3,p4;
        rectanglePtr->GetCornerPoints(p1,p2,p3,p4);
        // find rorate point
        olc::vf2d pRotate;
        olc::vf2d v(1.,0.);
        Rectangle::Params rectangleParams;
        rectanglePtr->GetParams(rectangleParams);
        Rotate(v,rectangleParams.theta,v);
        pRotate = rectangleParams.pCenter + (rectangleParams.W/2.*1.2)*v;

        std::vector<olc::vf2d> pVec = {p1,p2,p3,p4,pRotate};

        if (GetMouse(0).bPressed)
        {
            // reset
            isSeleted_ = false;
            indexSelected_ = -1;

            if (IsSelected(mousePos_,pVec,r_,indexSelected_) || rectanglePtr->IsInside(mousePos_))
            {
                // check if either a control point or triangle is selected
                isSeleted_ = true;
                
                if (indexSelected_ == -1)
                {
                    // rectangle is selected
                    // record mouse position
                    mousePosPrev_ = mousePos_;  
                    pCenterPrev_ = rectangleParams.pCenter;
                }
            }

            // reset pivot
            if (indexSelected_ >= 0 && indexSelected_ <= 3)
            {
                pPivotPrev_ = pVec[(indexSelected_+2)%4];
            }
        }
        else if (GetMouse(0).bHeld && !GetKey(olc::Key::X).bHeld)
        {
            if (isSeleted_)
            {
                // p0   p1
                // p3   p2
                if (indexSelected_ >= 0 && indexSelected_ <= 3)
                {
                    olc::vf2d pPivot;
                    pPivot = pVec[(indexSelected_+2)%4];

                    // find center
                    olc::vf2d vDiff = mousePos_ - pPivot;
                    std::cout << "vDiff.x: " << vDiff.x << " , vDiff.y: " << vDiff.y << std::endl;

                    rectangleParams.pCenter = pPivot + vDiff/2.;
                    // find W & H
                    std::cout << "W: " << rectangleParams.W << " , H: " << rectangleParams.H << std::endl;

                    Rotate(vDiff,-rectangleParams.theta,vDiff);
                    rectangleParams.W = std::abs(vDiff.x);
                    rectangleParams.H = std::abs(vDiff.y);

                    std::cout << "W: " << rectangleParams.W << " , H: " << rectangleParams.H << std::endl;
                    
                    // check for pivot change
                    if (std::abs(pPivot.x-pPivotPrev_.x) >= 1e-4)
                    {
                        std::cout << "changeX\n";
                        // 0,1
                        // 2,3
                        if (indexSelected_ == 0) indexSelected_ = 1;
                        else if (indexSelected_ == 1) indexSelected_ = 0;
                        else if (indexSelected_ == 2) indexSelected_ = 3;
                        else indexSelected_ = 2;
                    }
                    else if (std::abs(pPivot.y-pPivotPrev_.y) >= 1e-4)
                    {
                        std::cout << "changeY\n";
                        // 0,3
                        // 1,2
                        if (indexSelected_ == 0) indexSelected_ = 3;
                        else if (indexSelected_ == 1) indexSelected_ = 2;
                        else if (indexSelected_ == 2) indexSelected_ = 1;
                        else indexSelected_ = 0;
                    }

                    std::cout << "------------------------\n";
                    
                    // update pivot
                    pPivotPrev_ = pPivot;
                }
                else if (indexSelected_ == 4)
                {
                    // difference vector
                    olc::vf2d vDiff = mousePos_ - rectangleParams.pCenter;
                    // update theta
                    rectangleParams.theta = std::atan2(vDiff.y,vDiff.x);
                }
                else
                {
                    // rectangle is selected
                    // find diff vector of mouse position
                    olc::vf2d mouseDiff = mousePos_ - mousePosPrev_;
                    // update rectangle's center
                    rectangleParams.pCenter = pCenterPrev_ + mouseDiff;
                }
            }
        }
        
        // update rectangle params
        rectanglePtr->SetParams(rectangleParams);

        // draw control points
        FillCircle(WorldToScreen(p1),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(p2),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(p3),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(p4),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(pRotate),WorldToScreen(r_),controlColor_);

    }
    else if (shapeType == "circle")
    {

    }
    else if (shapeType == "track")
    {

    }
    else
    {
        std::cerr << "Error: Unknown shape\n";
    }

    // get key input
    if (GetKey(olc::Key::ESCAPE).bPressed)
    {
        // return to HOME
        stateMachine_ = StateMachine::HOME;
    }
    else if (GetKey(olc::Key::D).bPressed)
    {
        // delete
        for (int i = 0; i < shapesPtr_.size(); i++)
        {
            if (shapeEditPtr_ == shapesPtr_[i])
            {
                shapesPtr_.erase(shapesPtr_.begin()+i);
                break;
            }
        }
        // return to HOME
        stateMachine_ = StateMachine::HOME;
    }
}

void MapCreator::Draw()
{
    for (Shape *shapePtr : shapesPtr_)
    {
        std::string shapeType = shapePtr->GetShape();
        if (shapeType == "triangle")
        {
            Triangle *trianglePtr = dynamic_cast<Triangle*>(shapePtr);
            Triangle::Params params;
            trianglePtr->GetParams(params);
            FillTriangle(WorldToScreen(params.p1),WorldToScreen(params.p2),WorldToScreen(params.p3),obstacleColor_);
        }
        else if (shapeType == "rectangle")
        {
            Rectangle *rectanglePtr = dynamic_cast<Rectangle*>(shapePtr);
            Triangle upperTriangle,lowerTriangle;
            Triangle::Params triParams;
            rectanglePtr->GetTriangles(upperTriangle,lowerTriangle);
            upperTriangle.GetParams(triParams);
            FillTriangle(WorldToScreen(triParams.p1),WorldToScreen(triParams.p2),WorldToScreen(triParams.p3),obstacleColor_);
            lowerTriangle.GetParams(triParams);
            FillTriangle(WorldToScreen(triParams.p1),WorldToScreen(triParams.p2),WorldToScreen(triParams.p3),obstacleColor_);
        }
        else if (shapeType == "circle")
        {

        }
        else if (shapeType == "track")
        {

        }
        else
        {
            std::cerr << "Error: Unknown shape\n";
        }
    }
}

bool MapCreator::OnUserUpdate(float fElapsedTime)
{
    Clear(backgroundColor_);
    PanAndZoom();

    // draw created shapes
    Draw();

    // update mouse position
    mousePos_.x = GetMouseX();
    mousePos_.y = GetMouseY();
    mousePos_ = ScreenToWorld(mousePos_);

    switch (stateMachine_)
    {
    case StateMachine::INIT:
        stateMachine_ = StateMachine::HOME;
        break;

    case StateMachine::HOME:
        Home();
        break;

    case StateMachine::CREATE:
        Create();
        break;

    case StateMachine::EDIT:
        Edit();
        break;

    case StateMachine::EXPORT:
        break;
    
    default:
        stateMachine_ = StateMachine::INIT;
        break;
    }
    
    return true;
}

bool MapCreator::IsSelected(const olc::vf2d &pIn, const std::vector<olc::vf2d> &pVec, const float &r, int &indexOut)
{
    bool isSelected = false;

    for (int i = 0; i < pVec.size(); i++)
    {
        olc::vf2d p = pVec[i];
        olc::vf2d pDiff = pIn - p;
        if (pDiff.mag() <= r)
        {
            isSelected = true;
            indexOut = i;
            break;
        }
    }

    return isSelected;
}


int main()
{
	MapCreator mapCreator;
	if (mapCreator.Construct(800, 500, 4, 4))
		mapCreator.Start();

	return 0;
}