/*****************************************************************************
 Abdoul Diallo
 Copyright
 *****************************************************************************/

#include <iostream>
#include "Color.h"
#include "Surface.h"
#include <vector>
#include <cmath>

#ifndef DEMO_POINT_H
#define DEMO_POINT_H

/// Circle class
class Point {
public:
    Point (Surface *psurf=NULL, int x = 0, int y = 0)
    :
    psurface_(psurf), x_(x), y_(y)

    {}

    /// Set coordinates of circle
    void set_x_y(int x, int y)
    {
        x_ = x;
        y_ = y;
    }

    /// Return Methods
    int get_x() { return x_; }
    int get_y() { return y_; }
    int get_radius() { return radius_; }

    /// Draw the circle in the space
    void draw()
    {
        Color color; /// Color setup
        if (is_prim)
            color = prim_color;

        else
            color = snd_color;

        if (numberHits > 100)
            color = C6;

        if (numberHits > 200)
            color = WHITE;

        if (numberHits > 300)
        {
            color = prim_color;
            numberHits = 0;
        }

        /// Call of draw circle method
        psurface_->put_circle(x_, y_, get_radius(), color);
    }

    /// simulate moving circles on the space
    void run()
    {
        ++switch_;
        int direction = rand() % 4 + 1;

        if (direction == 1)
        {
            if (switch_ > 50)
                dy_ = -dy_;

            if (switch_ > 100)
            {
                dx_ = -dx_;
                switch_ = 0;
            }
        }

        if (y_ + radius_ >= H || y_ - radius_ <= 0)
        {
            dy_ = -dy_;
        }

        if (x_ - radius_ <= 0 || x_ + radius_ >= W)
        {
            dx_ = -dx_;
        }
        x_ += dx_;
        y_ += dy_;
    }

    /// Test if point collide with an existing point
    bool is_colide(Point * circle)
    {
        return (sqrt(pow((x_ - circle->get_x()),2) +
                pow((y_ - circle->get_y()),2)) <= radius_ * 2);
    }

    bool is_colide(Point circle)
    {
        return (sqrt(pow((x_ - circle.get_x()),2) +
                pow((y_ - circle.get_y()),2)) <= radius_ * 2);
    }

    /// Is the point not less than or greater than 0, H, or W
    bool not_good_point(Point circle)
    {
        /// Test to see if the generated point is not at the edge of the screen
        if (circle.x_ - radius_ <= 0 || circle.x_ + radius_ >= W ||
            circle.y_ - radius_ <= 0 || circle.y_ + radius_ >= H)
            return true;

        return false;
    }

    /// Change direction whenever a collision happens
    void hit()
    {
        ++delay_;
        if (delay_ >= 10)
        {
            dx_ = -dx_;
            dy_ = -dy_;
            is_prim = !is_prim;
            delay_ = 0;
            ++numberHits;
        }
    }

    /// Class attributes
private:
    int x_;
    int y_;
    int dx_ = 1;
    int dy_ = 1;
    int delay_ = 0;
    int switch_ = 0;
    int numberHits = 0;
    static int radius_;
    Color prim_color = C1;
    Color snd_color  = C3;
    bool is_prim = true;
    Surface * psurface_;

};

/// Static attribute initialization
int Point:: radius_ = 3;

///QuadTree class
class QuadTree {
public:
    QuadTree (Surface *psurf, std::vector<Point*> Points,
              int x=0, int y=0, int w=W, int h=H)
    : psurface_(psurf), x_(x), y_(y), w_(w), h_(h)
    {
        for (int i = 0; i < Points.size(); ++i)
        {
            if (contains_point(Points[i])) /// add circle if within boundary
            {
                array_.push_back(Points[i]);
            }
        }
        ///Subdivide so long as a circle can fit into the quadrant
        if (array_.size() >= capacity_ && (w_ - h_) > ptr->get_radius() * 2)
        {
            subdivide();
        }
    }

    /// Subdivide main quadrant into four equal quadrants
    void subdivide()
    {
        child_.push_back(QuadTree(psurface_, array_, x_, y_, w_/2, h_/2));                 ///NW
        child_.push_back(QuadTree(psurface_, array_, x_, y_ + h_/2, w_/2, h_/2));          ///SW
        child_.push_back(QuadTree(psurface_, array_, x_ + w_/2, y_ + h_/2, w_/2, h_/2));   ///SE
        child_.push_back(QuadTree(psurface_, array_, x_ + w_/2, y_, w_/2, h_/2));          ///NE
        is_splited_ = true;  ///
        array_.clear();     /// Clear the array
    }

    /// Is current circle within child's boundary
    bool contains_point(Point * Pt)
    {
        return (Pt->get_x() >= x_ && Pt->get_x() <= x_ + w_ &&
           Pt->get_y() >= y_ && Pt->get_y() <= y_ + h_);
    }
    void draw ()
    {
        doubled = false;
        psurface_->put_line(x_, y_ + h_/2, x_ + w_ - 1, y_ + h_/2, WHITE);
        psurface_->put_line(x_ + w_/2, y_, x_ + w_/2, y_ + h_-1, WHITE);

        for (int i = 0; i < child_.size(); ++i)
        {
            child_[i].draw();
        }
    }

    void double_draw()
    {
        doubled = true;
        psurface_->put_line(x_, y_ + h_/2, x_ + w_ - 1, y_ + h_/2, WHITE);
        psurface_->put_line(x_ + 1, y_ + h_/2 + 1, x_ + w_ - 1, y_ + h_/2 + 1, CYAN); /// double line

        psurface_->put_line(x_ + w_/2, y_, x_ + w_/2, y_ + h_-1, WHITE);
        psurface_->put_line(x_ + w_/2 + 1, y_ + 1, x_ + w_/2 + 1, y_ + h_ - 1, C5); /// double line

        for (int i = 0; i < child_.size(); ++i)
        {
            child_[i].double_draw();
        }
    }

    ///Collision detection method within the four children
    void collision_detect()
    {
        for (int i = 0; i < array_.size() ; ++i) {
            for (int j = i + 1; j < array_.size(); ++j) {
                if (array_[i]->is_colide(array_[j]))
                {
                    array_[i]->hit();
                    array_[j]->hit();
                }
            }
        }

        for (int i = 0; i < child_.size(); ++i)
        {
            child_[i].collision_detect();
        }
    }
private:
    int x_, y_, w_, h_;
    Surface * psurface_;
    std::vector < Point * > array_;
    std::vector < QuadTree > child_;
    bool is_splited_ = false;
    bool doubled = false;
    static int capacity_;
    Point * ptr;

};
int QuadTree::capacity_ = 4;

#endif //DEMO_POINT_H
