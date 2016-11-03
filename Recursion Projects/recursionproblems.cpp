/*
 * Jonah Zamora Epstein
 * CS106B
 * Section Leader: Kensen Shi
 * This program provides the two recursive functions used in the recursionmain.cpp for the problems
 * of figuring out the weight on a specific person's knees in a human pyrmaid and for drawing Sierpinski's
 * triangle. Each one is a recursive function which call itself in order to complete the task
 */

// Please feel free to add any other #includes you need!
#include "recursionproblems.h"
#include <cmath>
#include <iostream>
#include "hashmap.h"
#include "map.h"
#include "random.h"
#include "gobjects.h"
#include "gtypes.h"

using namespace std;

/*
 * This function is the recursive function used to find the weight on a specified person's knees in a
 * human pyramid. It does this by first checking the base case of the person being "outside the pyramid" or
 * non-existent, and it then returns the weight of the person at that spot plus half the weight on the knees
 * of the two people above him. That is where the recursion comes in, as to find half the weight of the two
 * people above, it calls weightOnKnees again.
 */
double weightOnKnees(int row, int col, const Vector<Vector<double> >& weights) {
    //base case of person not being in pyramid
    if (row < 0 || col >= weights[row].size() || col < 0) {
        return 0.0;
    }
    return weights[row][col] + ((.5) * weightOnKnees(row - 1, col - 1, weights)) +
            ((.5) * weightOnKnees(row - 1, col, weights));
}

/*
 * This function is the recursive function used to draw Sierpinski's triangle. It does this by first by being
 * passed in the starting point (x, y), the size, and the order. The base case for this is when the order is zero,
 * and that is when it returns. The order gets to zero by taking one away from it everytime it goes through
 * recursion. For clarity's sake, the three points of the triangle are set as GPoints, and then three lines are
 * draw to draw on triangle. Then it recursivle calls the function to draw three more smaller triangles centered
 * inside the original. It only draws the triangle when the order == 1, therefore ensuring that it doesn't redraw lines.
 */
void drawSierpinskiTriangle(GWindow& gw, double x, double y, int size, int order) {
    if (order == 0) {
        return;
    }
    if (order < 0 || x < 0 || y < 0) {
        cout << "Invalid parameters passed! (threw an exception)" << endl;
        return;
    }
    GPoint a(x, y);
    GPoint b(x + size, y);
    GPoint c(x + .5 * size, y + (.5 * size * sqrt(3)));
    if (order == 1) {
        gw.drawLine(a, b);
        gw.drawLine(b, c);
        gw.drawLine(c, a);
    } else {
        //the order gets minused by one so that it will stop recursing eventually.
        drawSierpinskiTriangle(gw, x, y, size * .5, order - 1);
        drawSierpinskiTriangle(gw, x + .5 * size, y, size * .5, order - 1);
        drawSierpinskiTriangle(gw, x + .25 * size, y + .5 * (.5 * size * sqrt(3)), size * .5, order - 1);
    }

}

