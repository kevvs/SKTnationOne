#ifndef __VECTOR3_H
#define __VECTOR3_H

#include "point.h"

// Класс трехмерный вектор
class C_Vector3
{
public:
    // Элементы
    double x, y, z;
    // Констуктор по умолчанию
    C_Vector3();
    // Конструктор из трех элементов
    C_Vector3(double x, double y, double z);
    // Конструктор из точки
    C_Vector3(const C_Point &p);
    // Конструктор из двух точек - начала и конца
    C_Vector3(const C_Point &beg_, const C_Point &end_);
    // Конструктор из другого вектора
    C_Vector3(const C_Vector3 &v);

    // Кастование к точке
    C_Point pnt() const;
    // Норма вектора
    double norm() const;
    // Квадрат нормы вектора
    double norm2() const;
    // Векторное произведение
    C_Vector3 cross(const C_Vector3 &other) const;
    // Операторы типа "скобка"
    double &operator [] (size_t i);
    double operator [] (size_t i) const;
    // Сложение и вычитание векторов
    C_Vector3 operator + (const C_Vector3 &other) const;
    C_Vector3 operator - (const C_Vector3 &other) const;
    // Деление вектора на число
    C_Vector3 operator / (const double &a) const;
    // Умножение числа на вектор
    friend C_Vector3 operator * (const double &a, const C_Vector3 &vec);
    // Скалярное произведение
    double operator * (const C_Vector3 &other) const;
    // Вывод
    friend ostream &operator << (ostream &os, const C_Vector3 &a);
};

#endif // __VECTOR3_H
