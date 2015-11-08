#ifndef __POINT_H
#define __POINT_H

#include "lib.h"

class C_Point
{
public:
	// Координаты
	double x, y, z;
	// Номер точки
	size_t num;
	// Конструктор по умолчанию
  C_Point();
	// Конструктор по трем координатам
  C_Point(double x, double y, double z);
	
  // Конструктор по трем координатам и номеру
  //C_Point(double x, double y, double z, size_t num);
	// Конструктор из другой точки
  //C_Point(const C_Point &p);

	// Операторы типа "скобка"
	double & operator [] (size_t i);
  double operator [] (size_t i) const;
	// Оператор меньше (по номеру)	
	bool operator < (const C_Point &t) const;
	// Оператор равенства (по номеру)
	bool operator == (const C_Point &t) const;
	// Оператор присваивания
  C_Point &operator = (const C_Point &other);
	// Вывод
	friend ostream & operator << (ostream & os, const C_Point &a);
	// Проверка, лежит ли точка внутри параллелепипеда (для дерева)
	bool inside(double x0, double x1, double y0, double y1, double z0, double z1) const;
};

#endif // __POINT_H
