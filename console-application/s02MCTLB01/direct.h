#ifndef DIRECT_H
#define DIRECT_H

#include <iostream>
#include <fstream>
#include <cmath>
#include <vector>
#include <string>
#include <cstdlib>
#include "point.h"
#include "vector3.h"

using namespace std;

#if !defined M_PI
#define M_PI 3.14159265358979323846
#endif

/*   _____________
 *  /|6          /|7    ^ z
 * /____________/ |     |  ^ y
 * |4|          |5|     | /
 * | |          | |     |/----> x
 * | |          | |
 * | |          | |
 * | |__________|_|
 * | /2         | /3
 * |/___________|/
 * 0            1
 */

/* Список соседей:
 * 0 - слева
 * 1 - справа
 * 2 - спереди
 * 3 - сзади
 * 4 - снизу
 * 5 - сверху
 */

class cube
{
public:
    point * nodes[8];
    vector3 p;
    double mes;
    point barycenter;
    cube * neighbor[6];
    cube();
    void init();
    vector3 get_B(point x) const;

    double gauss_weights[27];
    point gauss_points[27];
    double jacobian;

    size_t num;
};

class include
{
public:
    double x_min, x_max;
    double y_min, y_max;
    double z_min, z_max;
    vector3 p;
    bool inside(const cube & p) const;
    friend istream & operator >> (istream & is, include & a);
};

class area
{
public:
    vector<point> nodes;
    vector<cube> cubes;
    vector3 get_B(point x) const;
    double get_abs_B(point x) const;
    void generate(const string & filename);
};

#endif // DIRECT_H
