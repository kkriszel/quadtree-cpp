// Graphical Interface - olc::PixelGameEngine by javidx9
#define OLC_PGE_APPLICATION
#include "olc/olcPixelGameEngine.h"

#include "shape_container.hpp"      // RectangleContainer, QuadTreeContainer, LinearContainer

#include <cstdlib>                  // srand()
#include <ctime>                    // time()
#include <string>                   // std::string
#include <chrono>                   // std::chrono

class QuadTreeDemo : public olc::PixelGameEngine
{
    private:
        int nrItems;                                        // number of Rectangles on the screen
        int minSizeRect;                                    // the minimum size of a Shape (important upon populating the containers)
        int maxSizeRect;                                    // the maximum size of a Shape (important upon populating the containers)

        enum SCType {QUAD_TREE = 0, LINEAR, SCSIZE};        // the indices of the different containers (SC - ShapeContainer)
        enum QRType {OVERLAP = 0, CONTAIN, QRSIZE};         // the types of query/remove operations (QR - Query/Remove)

        ShapeContainer* containers[SCType::SCSIZE];      
        SCType currentContainer = SCType::QUAD_TREE;        // index of current working container
        bool setBoundariesVisible = false;                  // whether or not the bounds of the containers should be visible
        QRType opType = QRType::OVERLAP;                    // what kind of operation are we performing (ovelapping/full containinig)
        bool displayHelp = false;

        olc::Sprite *rectangleSprite;                       // let's draw the objects to a sprite for faster drawing between the frames
        olc::Sprite *boundSprites[SCType::SCSIZE];          // let's also draw the boundaries to a sprite for the same reason

        qt::Bound screenBound;                              // bounds of the screen ({0, 0}, {width-1, height-1})
        olc::vf2d queryBoundSize = olc::vf2d(100, 100);     // size of the query rectangle
        olc::vf2d textScale;                                // scale of text according to the size of the screen
        olc::vf2d textOffset;                               // offset of text according to the size of the screen (for better visibility)

        const double queryBoundZoomRate = 120.0;            // user zoom in/out speed

        // predefined colors for different drawing components
        const olc::Pixel BACKGROUND_COL = olc::BLACK; 
        const olc::Pixel BOUND_COL = olc::CYAN;
        const olc::Pixel QUERY_OBJ_COL = olc::RED;
        const olc::Pixel QUERY_BOUND_COL[QRType::QRSIZE] = {olc::GREEN, olc::YELLOW};

        const std::string helpMsg = "N-increment query\nM-decremenet query\nA-add shape\nX-remove shapes\nC-toggle query\nB-toggle bound draw\nSPACE-toggle model\nH-toggle help msg\n";

        // updates the sprites according to the current objects
        void updateSprites() {
            // let's draw the Rectangles
            SetDrawTarget(rectangleSprite);
            Clear(BACKGROUND_COL);

            // get all of the Rectangles in the frame from the LinearContainer (faster in this case)
            auto query = containers[SCType::LINEAR]->queryOverlap(screenBound);
            for(const auto &item : query) {
                // draw one Shape at a time
                FillRect({item->topLeft.x, item->topLeft.y}, {item->bottomRight.x - item->topLeft.x, item->bottomRight.y - item->topLeft.y}, {item->color.r, item->color.g, item->color.b});
            }

            // how many Rectangles are on the screen
            nrItems = query.size();

            // for all the containers (2), get the boundaries and draw them to a different sprite, together with the Rectangles
            for(int i = 0; i < SCType::SCSIZE; i++) {
                SetDrawTarget(boundSprites[i]);
                DrawSprite({0, 0}, rectangleSprite);
                for(const auto &bound : containers[i]->getBounds()) {
                    DrawRect({bound.topLeft.x, bound.topLeft.y}, {bound.bottomRight.x - bound.topLeft.x, bound.bottomRight.y - bound.topLeft.y}, BOUND_COL);
                }
            }

            // reset the target to the main screen
            SetDrawTarget(nullptr);
        } 

    public:
        // constructs an application with given initial Shape numbers and the minimum and maximum size (width and height) for a Shape
        QuadTreeDemo(int nrItems, int minSizeRect, int maxSizeRect) : nrItems(nrItems), minSizeRect(minSizeRect), maxSizeRect(maxSizeRect) {
            sAppName = "QuadTree Demo";
        }

    public:
        // gets called only once, when initialising the application
        bool OnUserCreate() override {
            // init the random number generator
            srand(time(0));
            
            // init the screen bounds
            screenBound = qt::Bound(qt::Vec2D_i32(0, 0), qt::Vec2D_i32(ScreenWidth() - 1, ScreenHeight() - 1));

            textScale = {ScreenWidth() / 250.0f, ScreenHeight() / 250.0f};
            textOffset = textScale / 2.0f;

            // instantiate the RectangleContainers
            containers[SCType::QUAD_TREE] = new QuadTreeContainer(screenBound);
            containers[SCType::LINEAR] = new LinearContainer(screenBound);

            // let's create the given ammount of Rectangles, randomly
            for(int i = 0; i < nrItems; i++) {
                Shape r = Shape::getRandomGrayShape(screenBound, qt::Vec2D_i32(minSizeRect, minSizeRect), qt::Vec2D_i32(maxSizeRect, maxSizeRect));

                // insert it in both of the containers
                containers[SCType::QUAD_TREE]->insert(r);
                containers[SCType::LINEAR]->insert(r);
            }

            // init the sprites
            rectangleSprite = new olc::Sprite(ScreenWidth(), ScreenHeight());
            boundSprites[0] = new olc::Sprite(ScreenWidth(), ScreenHeight());
            boundSprites[1] = new olc::Sprite(ScreenWidth(), ScreenHeight());

            // draw the Rectangles and bounds to the sprites
            updateSprites();

            return true;
        }

        // main frame method, gets called forever until close operation
        bool OnUserUpdate(float elapsedTime) override {
            // get the mouse location
            olc::vd2d mouse = olc::vd2d(GetMouseX(), GetMouseY());

            // calculate the boundaries of the query
            olc::vd2d queryBoundTopLeft = mouse - queryBoundSize / 2;
            olc::vd2d queryBoundBottomRight = mouse + queryBoundSize / 2;
            qt::Bound queryBound = qt::Bound(qt::Vec2D_i32(queryBoundTopLeft.x, queryBoundTopLeft.y), qt::Vec2D_i32(queryBoundBottomRight.x, queryBoundBottomRight.y));

            // make the query smaller with N key
            if(GetKey(olc::N).bHeld) {
                queryBoundSize.x = std::max(queryBoundSize.x - elapsedTime * queryBoundZoomRate, 0.0);
                queryBoundSize.y = std::max(queryBoundSize.y - elapsedTime * queryBoundZoomRate, 0.0);
            }

            // make the query bigger with M key
            if(GetKey(olc::M).bHeld) {
                queryBoundSize.x = std::min(queryBoundSize.x + elapsedTime * queryBoundZoomRate, static_cast<double>(ScreenWidth()));
                queryBoundSize.y = std::min(queryBoundSize.y + elapsedTime * queryBoundZoomRate, static_cast<double>(ScreenHeight()));
            }

            // add a new shape to the screen
            if(GetKey(olc::A).bReleased) {
                Shape r = Shape(qt::Vec2D_i32(queryBoundTopLeft.x, queryBoundTopLeft.y), qt::Vec2D_i32(queryBoundBottomRight.x, queryBoundBottomRight.y), Shape::Color(255, 255, 255));
                containers[SCType::QUAD_TREE]->insert(r);
                containers[SCType::LINEAR]->insert(r);
                updateSprites();
            }

            // remove Rectangles in the query with X key
            if(GetKey(olc::X).bReleased) {
                switch(opType) {
                    case QRType::OVERLAP:
                        containers[SCType::QUAD_TREE]->removeOverlap(queryBound);
                        containers[SCType::LINEAR]->removeOverlap(queryBound);
                        break;
                    case QRType::CONTAIN:
                        containers[SCType::QUAD_TREE]->removeContain(queryBound);
                        containers[SCType::LINEAR]->removeContain(queryBound);
                        break;
                    default:
                        break;
                }
                updateSprites();
            }

            // change operation type with C key
            if(GetKey(olc::C).bReleased) {
                opType = static_cast<QRType>(opType ^ 1);
            }

            // toggle boundaries with B key
            if(GetKey(olc::B).bReleased) {
                setBoundariesVisible ^= 1;
            }

            // switch container with SPACE key
            if(GetKey(olc::SPACE).bPressed) {
                currentContainer = static_cast<SCType>(currentContainer ^ 1);
            }

            // toggle message text
            if(GetKey(olc::H).bReleased) {
                displayHelp ^= 1;
            }

            // get the results of the query in this vector
            std::vector<std::list<Shape>::iterator> query;

            // make a query, and also measure the time
            auto clockStart = std::chrono::high_resolution_clock::now();
            switch(opType) {
                case QRType::OVERLAP:
                    query = containers[currentContainer]->queryOverlap(queryBound);
                    break;
                case QRType::CONTAIN:
                    query = containers[currentContainer]->queryContain(queryBound);
                    break;
                default:
                    break;
            }
            auto clockStop = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration<double>(clockStop - clockStart);

            // check if the boundaries should be drawn
            if(setBoundariesVisible) {
                // draw the sprite with the Rectangles and boundaries
                DrawSprite({0, 0}, boundSprites[currentContainer]);
            } else {
                // draw the sprite with only the Rectangles
                DrawSprite({0, 0}, rectangleSprite);
            }

            // iterate through the std::list<Shape>::iterators, and draw the Rectangles on the screen with different color
            for(const auto& item : query) {
                 FillRect({item->topLeft.x, item->topLeft.y}, {item->bottomRight.x - item->topLeft.x, item->bottomRight.y - item->topLeft.y}, QUERY_OBJ_COL);
            }

            // draw the query Boundary
            DrawRect(queryBoundTopLeft, queryBoundBottomRight - queryBoundTopLeft, QUERY_BOUND_COL[opType]);

            // draw information about the current container, the number of Rectangles in the quert/the total number of Rectangles, the time of the query
            std::string currentContainerStr;
            if(currentContainer == SCType::QUAD_TREE) {
                currentContainerStr = "QuadTree";
            } else {
                currentContainerStr = "Linear";
            }

            currentContainerStr += + ":" + std::to_string(query.size()) + "/" + std::to_string(nrItems) + "\n" + std::to_string(duration.count()) + "s";

            DrawStringDecal({0, 0}, currentContainerStr, olc::BLACK, textScale);
            DrawStringDecal(textOffset, currentContainerStr, olc::YELLOW, textScale);

            // draw help message
            if(displayHelp) {
                DrawStringDecal(olc::vf2d(0, textScale.y * 2 * 8), helpMsg, olc::BLACK, textScale / 2.0f);
                DrawStringDecal(olc::vf2d(0, textScale.y * 2 * 8) + textOffset, helpMsg, olc::YELLOW, textScale / 2.0f);
            }

            // return that the application should continue running
            return true;
        }

        // gets called once after closing the application
        bool OnUserDestroy() override {
            // free the dynamically allocated objects
            delete rectangleSprite;
            delete containers[SCType::QUAD_TREE];
            delete boundSprites[SCType::QUAD_TREE];
            delete containers[SCType::LINEAR];
            delete boundSprites[SCType::LINEAR];
            return true;
        }
};

// main entry point of the program
int main(int argc, char **argv)
{
	
    int nrItems, screenWidth, screenHeight, minSize, maxSize;
    if(argc != 6) {
	std::cerr << "Usage: " << argv[0] << " <nrItems> <screenWidth> <screenHeight> <minSize> <maxSize>\n";
        nrItems = 100, screenWidth = 800, screenHeight = 800, minSize = 10, maxSize = 60;
    } else {
	// convert the arguments of the program to integer
	nrItems = std::atoi(argv[1]);
	screenWidth = std::atoi(argv[2]);
	screenHeight = std::atoi(argv[3]);
	minSize = std::atoi(argv[4]);
	maxSize = std::atoi(argv[5]);
    }

    // construct the application, then start it
	QuadTreeDemo demo(nrItems, minSize, maxSize);
	if(demo.Construct(screenWidth, screenHeight, 1, 1))
		demo.Start();

	return 0;
}
