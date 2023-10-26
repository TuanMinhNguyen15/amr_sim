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
                olc::vf2d mousePos;
                mousePos.x = GetMouseX();
                mousePos.y = GetMouseY();
                mousePos = ScreenToWorld(mousePos);
                controlPoints_Triangle_[numPoint_Triangle_] = mousePos;
                
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
                }              
            }

            // draw control points
            for (int i = 0; i < numPoint_Triangle_; i++)
            {
                olc::vf2d controlPoint;
                controlPoint = controlPoints_Triangle_[i];
                FillCircle(WorldToScreen(controlPoint),WorldToScreen(r_));
            }
            break;
        }
        case ShapeOption::RECTANGLE:
        {
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
    std::string shapeType = shapeEditPtr_->GetShape();

    if (shapeType == "triangle")
    {
        // extract pointer of triangle class
        Triangle *trianglePtr = dynamic_cast<Triangle*>(shapeEditPtr_);
        Triangle::Params params;
        trianglePtr->GetParams(params);

        // check if user selected any control points

        // draw control points
        FillCircle(WorldToScreen(params.p1),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(params.p2),WorldToScreen(r_),controlColor_);
        FillCircle(WorldToScreen(params.p3),WorldToScreen(r_),controlColor_);
    }
    else if (shapeType == "rectangle")
    {

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

    Draw();

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

int main()
{
	MapCreator mapCreator;
	if (mapCreator.Construct(800, 500, 4, 4))
		mapCreator.Start();

	return 0;
}