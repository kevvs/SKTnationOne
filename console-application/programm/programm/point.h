#if !defined POINT_H_INCLUDED
#define POINT_H_INCLUDED

//#include "../common/common.h"
#include <iostream>
#include <fstream>
#include <complex>
#include <assert.h>
using namespace std;

// ������� typedef'��
template<typename T> class point_t;
typedef point_t<double> point;
typedef point_t< complex<double> > cpoint;

// ����� �����
template<typename T>
class point_t
{
public:
	// ����������
	T x, y, z;
	// ����� �����
	size_t num;
	// ����������� �� ���������
	point_t();
	// ����������� �� ���� �����������
	template<typename U>
	point_t(U x, U y, U z);
	// ����������� �� ���� ����������� � ������
	template<typename U>
	point_t(U x, U y, U z, size_t num);
	// ����������� �� ������ �����
	template<typename U>
	point_t(const point_t<U> & p);
	// ��������� ���� "������"
	T & operator [] (size_t i);
	T operator [] (size_t i) const;
	// �������� ������ (�� ������)
	template<typename U>
	bool operator < (const point_t<U> & t) const;
	// �������� ��������� (�� ������)
	template<typename U>
	bool operator == (const point_t<U> & t) const;
	// �������� ������������
	point_t<T> & operator = (const point_t<T> & other);
	template<typename U>
	point_t<T> & operator = (const point_t<U> & other);
	// �����
	template<typename U>
	friend ostream & operator << (ostream & os, const point_t<U> & a);
	// ��������, ����� �� ����� ������ ��������������� (��� ������)
	bool inside(double x0, double x1, double y0, double y1, double z0, double z1) const;
};

// ����������� �� ���������
template<typename T>
point_t<T>::point_t()
{
	x = y = z = static_cast<T>(0);
	num = 0;
}

// ����������� �� ���� �����������
template<typename T>
template<typename U>
point_t<T>::point_t(U x, U y, U z)
{
	this->x = static_cast<T>(x);
	this->y = static_cast<T>(y);
	this->z = static_cast<T>(z);
	num = 0;
}

// ����������� �� ���� ����������� � ������
template<typename T>
template<typename U>
point_t<T>::point_t(U x, U y, U z, size_t num)
{
	this->x = static_cast<T>(x);
	this->y = static_cast<T>(y);
	this->z = static_cast<T>(z);
	this->num = num;
}

// ����������� �� ������ �����
template<typename T>
template<typename U>
point_t<T>::point_t(const point_t<U> & p)
{
	x = static_cast<T>(p.x);
	y = static_cast<T>(p.y);
	z = static_cast<T>(p.z);
	num = p.num;
}

// ��������� ���� "������"
template<typename T>
T & point_t<T>::operator [] (size_t i)
{
	assert(i < 3);
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return x;
}

template<typename T>
T point_t<T>::operator [] (size_t i) const
{
	assert(i < 3);
	switch (i)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	}
	return 0;
}

// �������� ������ (�� ������)
template<typename T>
template<typename U>
bool point_t<T>::operator < (const point_t<U> & t) const
{
	return num < t.num;
}

// �������� ��������� (�� ������)
template<typename T>
template<typename U>
bool point_t<T>::operator == (const point_t<U> & t) const
{
	return num == t.num;
}

// �������� ������������
template<typename T>
point_t<T> & point_t<T>::operator = (const point_t<T> & other)
{
	if (this != &other)
	{
		this->x = other.x;
		this->y = other.y;
		this->z = other.z;
		this->num = other.num;
	}
	return *this;
}

template<typename T>
template<typename U>
point_t<T> & point_t<T>::operator = (const point_t<U> & other)
{
	this->x = static_cast<T>(other.x);
	this->y = static_cast<T>(other.y);
	this->z = static_cast<T>(other.z);
	this->num = other.num;
	return *this;
}

// �����
template<typename U>
ostream & operator << (ostream & os, const point_t<U> & a)
{
	os << "{ " << a.x << ", " << a.y << ", " << a.z << " }";
	return os;
}

// ��������, ����� �� ����� ������ ��������������� (��� ������)
template<>
bool point_t<double>::inside(double x0, double x1, double y0, double y1, double z0, double z1) const;

#endif // POINT_H_INCLUDED
