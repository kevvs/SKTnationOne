#include "point.h"

// ����������� �� ���������
C_Point::C_Point() {

  x = y = z = 0.0;
  num = 0;
}

// ����������� �� ���� �����������
C_Point::C_Point(double x, double y, double z) {

  this->x = x;
  this->y = y;
  this->z = z;
  num = 0;
}

//// ����������� �� ���� ����������� � ������
//template<typename T>
//template<typename U>
//point_t<T>::point_t(U x, U y, U z, size_t num)
//{
//	this->x = static_cast<T>(x);
//	this->y = static_cast<T>(y);
//	this->z = static_cast<T>(z);
//	this->num = num;
//}
//
//// ����������� �� ������ �����
//template<typename T>
//template<typename U>
//point_t<T>::point_t(const point_t<U> & p)
//{
//	x = static_cast<T>(p.x);
//	y = static_cast<T>(p.y);
//	z = static_cast<T>(p.z);
//	num = p.num;
//}

// ��������� ���� "������"
double &C_Point::operator [] (size_t i) {

  assert(i < 3);
  switch (i) {
  case 0:	return x;
  case 1:	return y;
  case 2:	return z;
  }
  return x;
}

double C_Point::operator [] (size_t i) const {

  assert(i < 3);
  switch (i) {
  case 0: return x;
  case 1: return y;
  case 2: return z;
  }
  return 0.0;
}

// �������� ������ (�� ������)
bool C_Point::operator < (const C_Point &t) const {

  return num < t.num;
}

// �������� ��������� (�� ������)
bool C_Point::operator == (const C_Point &t) const {

  return num == t.num;
}

// �������� ������������
C_Point &C_Point::operator = (const C_Point &other){

  if(this != &other){
    this -> x = other.x;
    this -> y = other.y;
    this -> z = other.z;
    this -> num = other.num;
  }
  return *this;
}

// �����
ostream &operator << (ostream & os, const C_Point &a){

  os << "{ " << a.x << ", " << a.y << ", " << a.z << " }";
  return os;
}

// ��������, ����� �� ����� ������ ��������������� (��� ������)
bool C_Point::inside(double x0, double x1, double y0, double y1, double z0, double z1) const{
	
  if(x >= x0 && x <= x1 && y >= y0 && y <= y1 && z >= z0 && z <= z1){
    return true;
  }
	return false;
}
