#ifndef __POINT_H
#define __POINT_H

#include "lib.h"

class C_Point
{
public:
	// ����������
	double x, y, z;
	// ����� �����
	size_t num;
	// ����������� �� ���������
  C_Point();
	// ����������� �� ���� �����������
  C_Point(double x, double y, double z);
	
  // ����������� �� ���� ����������� � ������
  //C_Point(double x, double y, double z, size_t num);
	// ����������� �� ������ �����
  //C_Point(const C_Point &p);

	// ��������� ���� "������"
	double & operator [] (size_t i);
  double operator [] (size_t i) const;
	// �������� ������ (�� ������)	
	bool operator < (const C_Point &t) const;
	// �������� ��������� (�� ������)
	bool operator == (const C_Point &t) const;
	// �������� ������������
  C_Point &operator = (const C_Point &other);
	// �����
	friend ostream & operator << (ostream & os, const C_Point &a);
	// ��������, ����� �� ����� ������ ��������������� (��� ������)
	bool inside(double x0, double x1, double y0, double y1, double z0, double z1) const;
};

#endif // __POINT_H
