//
// Created by Diallo on 2019-04-18.
//
#include "Point.h"
#include <iostream>
#include "Color.h"
#include "Surface.h"

#ifndef DEMO_QUADTREE_H
#define DEMO_QUADTREE_H

// The main quadtree class
class QuadTree
{
    /// Hold details of the boundary of this node
    Point topLeft;
    Point botRight;

    /// Children of this tree
    QuadTree *NorthWest;
    QuadTree *NorthEast;
    QuadTree *SouthWest;
    QuadTree *SouthEast;

    /// Node details
    Node * ptrNode;

public:
    QuadTree()
    {
        NorthWest  = NULL;
        NorthEast = NULL;
        SouthWest  = NULL;
        SouthEast = NULL;
    }
    QuadTree(Point topL, Point botR)
    {
        n = NULL;
        NorthWest  = NULL;
        NorthEast = NULL;
        SouthWest  = NULL;
        SouthEast = NULL;
        topLeft = topL;
        botRight = botR;
    }

    void insert(Node*);
    Node* search(Point);
    bool inBoundary(Point);
};

/// Insert a node into the quadtree
void QuadTree::insert(Node *node)
{
    if (node == NULL)
        return;

    /// Current quad cannot contain it
    if (!inBoundary(node->pos))
        return;

    /// We cannot subdivide the quadtree if
    /// area is less than radius of the circle
    if (abs(topLeft.x - botRight.x) <= node->point_.get_radius() &&
        abs(topLeft.y - botRight.y) <= node->point_.get_radius())
    {
        if (n == NULL)
            n = node;
        return;
    }

    if ((topLeft.x + botRight.x) / 2 >= node->point_.x)
    {
        // Indicates NorthWest
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (NorthWest == NULL)
                NorthWest = new QuadTree(
                        Point(topLeft.x, topLeft.y),
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2));
            NorthWest->insert(node);
        }

            // Indicates SouthWest
        else
        {
            if (SouthWest == NULL)
                SouthWest = new QuadTree(
                        Point(topLeft.x,
                              (topLeft.y + botRight.y) / 2),
                        Point((topLeft.x + botRight.x) / 2,
                              botRight.y));
            SouthWest->insert(node);
        }
    }
    else
    {
        // Indicates NorthEast
        if ((topLeft.y + botRight.y) / 2 >= node->pos.y)
        {
            if (NorthEast == NULL)
                NorthEast = new QuadTree(
                        Point((topLeft.x + botRight.x) / 2,
                              topLeft.y),
                        Point(botRight.x,
                              (topLeft.y + botRight.y) / 2));
            NorthEast->insert(node);
        }

            // Indicates SouthEast
        else
        {
            if (SouthEast == NULL)
                SouthEast = new QuadTree(
                        Point((topLeft.x + botRight.x) / 2,
                              (topLeft.y + botRight.y) / 2),
                        Point(botRight.x, botRight.y));
            SouthEast->insert(node);
        }
    }
}

// Find a node in a quadtree
Node* QuadTree::search(Point p)
{
    // Current quad cannot contain it
    if (!inBoundary(p))
        return NULL;

    // We are at a quad of unit length
    // We cannot subdivide this quad further
    if (n != NULL)
        return n;

    if ((topLeft.x + botRight.x) / 2 >= p.x)
    {
        // Indicates NorthWest
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (NorthWest == NULL)
                return NULL;
            return NorthWest->search(p);
        }

            // Indicates SouthWest
        else
        {
            if (SouthWest == NULL)
                return NULL;
            return SouthWest->search(p);
        }
    }
    else
    {
        // Indicates NorthEast
        if ((topLeft.y + botRight.y) / 2 >= p.y)
        {
            if (NorthEast == NULL)
                return NULL;
            return NorthEast->search(p);
        }

            // Indicates SouthEast
        else
        {
            if (SouthEast == NULL)
                return NULL;
            return SouthEast->search(p);
        }
    }
};

// Check if current quadtree contains the point
bool QuadTree::inBoundary(Point p)
{
    return (p.x >= topLeft.x &&
            p.x <= botRight.x &&
            p.y >= topLeft.y &&
            p.y <= botRight.y);
}


#endif //DEMO_QUADTREE_H
