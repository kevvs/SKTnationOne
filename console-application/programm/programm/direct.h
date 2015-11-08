#ifndef __DIRECT_H
#define __DIRECT_H

#include "vector3.h"

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

class C_Cube
{
public:
	C_Point * nodes[8];
	C_Vector3 p;
	double mes;
	C_Point barycenter;
	C_Cube * neighbor[6];
	C_Cube();
	void init();
	C_Vector3 get_B(C_Point x) const;

	double gauss_weights[27];
	C_Point gauss_points[27];
	double jacobian;

	size_t num;
};

class C_Include
{
public:
	double x_min, x_max;
	double y_min, y_max;
	double z_min, z_max;
	C_Vector3 p;
	bool inside(const C_Cube &p) const;
	friend istream &operator >> (istream &is, C_Include &a);
};

class C_Area
{
public:
	vector<C_Point> nodes;
	vector<C_Cube> cubes;
	C_Vector3 get_B(C_Point x) const;
	double get_abs_B(C_Point x) const;
	void generate(ifstream &ifs);
};

#endif // __DIRECT_H
