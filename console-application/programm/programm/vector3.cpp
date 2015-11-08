#include "vector3.h"

// Констуктор по умолчанию
C_Vector3::C_Vector3() {

  x = y = z = 0.0;
}

// Конструктор из трех элементов
C_Vector3::C_Vector3(double x, double y, double z) {

  this->x = x;
  this->y = y;
  this->z = z;
}

// Конструктор из точки
C_Vector3::C_Vector3(const C_Point &p) {

  x = p.x;
  y = p.y;
  z = p.z;
}

// Конструктор из двух точек - начала и конца
C_Vector3::C_Vector3(const C_Point &beg_, const C_Point &end_) {

  x = end_.x - beg_.x;
  y = end_.y - beg_.y;
  z = end_.z - beg_.z;
}

// Конструктор из другого вектора
C_Vector3::C_Vector3(const C_Vector3 &v) {

  x = v.x;
  y = v.y;
  z = v.z;
}

// Кастование к точке
C_Point C_Vector3::pnt() const {

  return C_Point(x, y, z);
}

// Векторное произведение
C_Vector3 C_Vector3::cross(const C_Vector3 &other) const {

  return C_Vector3(y * other.z - z * other.y,
                   z * other.x - x * other.z,
                   x * other.y - y * other.x);
}

// Операторы типа "скобка"
double &C_Vector3::operator [] (size_t i) {

  assert(i < 3);
  switch (i){
    case 0: return x;
    case 1: return y;
    case 2: return z;
  }
  return x;
}

double C_Vector3::operator [] (size_t i) const {

  assert(i < 3);
  switch (i){
    case 0: return x;
    case 1: return y;
    case 2: return z;
  }
  return 0.0;
}

// Скалярное произведение
double C_Vector3::operator * (const C_Vector3 &other) const {

  return x * other.x + y * other.y + z * other.z;
}

// Сложение и вычитание векторов
C_Vector3 C_Vector3::operator + (const C_Vector3 &other) const {

  return C_Vector3(x + other.x, y + other.y, z + other.z);
}
C_Vector3 C_Vector3::operator - (const C_Vector3 &other) const {

  return C_Vector3(x - other.x, y - other.y, z - other.z);
}

// Деление вектора на число
C_Vector3 C_Vector3::operator / (const double &a) const {

  return C_Vector3(x / a, y / a, z / a);
}

// Умножение числа на вектор
C_Vector3 operator * (const double &a, const C_Vector3 &vec) {

  return C_Vector3(a * vec.x, a * vec.y, a * vec.z);
}

// Вывод
ostream &operator << (ostream &os, const C_Vector3 &a) {

  os << "{ " << a.x << ", " << a.y << ", " << a.z << " }";
  return os;
}

// Норма вектора (действительная)
double C_Vector3::norm() const{

    return sqrt(norm2());
}

// Квадрат нормы вектора (действительной)
double C_Vector3::norm2() const{

    return x * x + y * y + z * z;
}
