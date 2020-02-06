/*****************************************************************************
 Abdoul Diallo
 Copyright
 *****************************************************************************/

#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <ctime>
#include "Includes.h"
#include "Constants.h"
#include "compgeom.h"
#include "Surface.h"
#include "Event.h"
#include "Point.h"

///Check if the coordinates of the circle is within boundary
void generate_coordinates (int &x, int &y, int radius)
{
    do
    {
        x = rand() % W;
        y = rand() % H;
    }while (x + radius >= W || x - radius <= 0 ||
            y - radius <= 0 || y + radius >= H);
}
/// Game start here
void game()
{
  Surface surface(W, H);
  Event event;
  int numPoints = 200; /// Number of circles to draw

  /// Create a pointer array of circles
  Point * pPoints[numPoints];
  std::vector<Point *>VectPoints;

  /// Allocate memory for circles
  int col = 0;
  for (int i = 0; i < numPoints; ++i)
    {
      int x, y;
      generate_coordinates(x, y, pPoints[i]->get_radius());
      Point newPoint(&surface, x, y);
        for (int j = 0 + i; j < i; ++j)
        {
            while (pPoints[j]->is_colide(newPoint) && pPoints[j]->not_good_point(newPoint))
            {
                generate_coordinates(x, y, pPoints[i]->get_radius());
                newPoint.set_x_y(x, y);
            }
        }
            ///Insert circle with good coordinates
        pPoints[i] = new Point(&surface, x, y);
        VectPoints.push_back(pPoints[i]);
    }

  /// Verify that generated coordinates do not overlap
//    for (int k = 0; k < numPoints ; ++k)
//    {
//        if (pPoints[k]->get_x() - pPoints[k]->get_radius() <  0 ||
//            pPoints[k]->get_x() + pPoints[k]->get_radius() >= W ||
//            pPoints[k]->get_y() - pPoints[k]->get_radius() <  0 ||
//            pPoints[k]->get_y() + pPoints[k]->get_radius() >= H   )
//
//            std::cout<< "\nPoint: " << k << " x: " << pPoints[k]->get_x() << " y: " << pPoints[k]->get_y();
//    }

  /// Drawing setup
  bool draw_quad = true;
  bool double_drawing = false;

  while (1)
  {
    ///Create a QuadTree vector
    QuadTree QT (&surface, VectPoints);

    /// Check user inputs
    if (event.poll() && event.type() == QUIT)
        break;

    ///Handling keyboard inputs
    KeyPressed keypressed = get_keypressed();

    ///Turn off Quad
    if (keypressed[LEFTARROW])
    {
        draw_quad = false;
    }
    ///Turn on Quad
    if (keypressed[RIGHTARROW])
    {
        draw_quad = true;
    }

    ///Turn on Double Quad
    if (keypressed[UPARROW])
    {
        double_drawing = true;
    }
    ///Turn off double Quad
    if (keypressed[DOWNARROW])
    {
        double_drawing = false;
    }

    /// Move circles
    for (int i = 0; i < numPoints; ++i)
        pPoints[i]->run();

    /// Drawing circles in the screen
    surface.lock();
    surface.fill(DARKGRAY);

    /// If the user choose to use the quad
    if (draw_quad)
    {
        if (double_drawing)     /// double draw quad lines
        {
            QT.double_draw();
            QT.collision_detect();
        }
        else                   /// single draw quad lines
        {
            QT.draw();
            QT.collision_detect();
        }
    }

    /// If the user choose to use the quad
    else
    {
        for (int i = 0; i < numPoints; ++i)
        {
            for (int j = i + 1; j < numPoints; ++j)
            {
                if(pPoints[i]->is_colide(pPoints[j]) )
                {
                    pPoints[i]->hit();
                    pPoints[j]->hit();
                }
            }
        }
    }

    /// Loop that simulate the space
    for (int i = 0; i < numPoints; ++i)
    {
        pPoints[i]->draw();
    }

    surface.unlock();
    surface.flip();

  } /// END OF WHILE LOOP

  /// Delete circles from the heap
  for (int i = 0; i < numPoints; ++i)
  {
    delete pPoints[i];
  }

}  /// END OF QUAD GAME

int main(int argc, char* argv[])
{
    srand((unsigned int) time(NULL));
    game();

return 0;
}
