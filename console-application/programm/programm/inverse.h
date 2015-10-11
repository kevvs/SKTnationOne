#ifndef INVERSE_H
#define INVERSE_H

#include <vector>
#include <array>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>
#include "vector3.h"
#include "direct.h"

using namespace std;

class inverse_config
{
public:
	bool use_alpha;     // ������������ ������������� �� �����
	bool use_gamma;     // ������������ ������������� �� �����
	double alpha0;      // ��������� �������� alpha
	double dalpha;      // ��� ���������� alpha
	double alpha_coeff; // �� ������� ��� ����� ������� �������� �����������
	double gamma0;      // ��������� �������� gamma
	double dgamma;      // ��� ���������� gamma
	double gamma_coeff; // �� ������� �������� ����� �������� �������� �����������
	double gamma_diff;  // ������� ����� ��������, ������� ������� ��� ��������
	inverse_config();
	friend istream & operator >> (istream & is, inverse_config & a);
	friend ostream & operator << (ostream & os, const inverse_config & a);
};

class inverse
{
public:
	void input(const string &fn_input);//const string & fn_area, const string & fn_receivers, const string & fn_config);
	void calc();

protected:
	vector<vector<double> > A;
	vector<double> b;
	vector<vector<vector3> > L;
	size_t K, N;
	vector<pair<point, vector3> > receivers;

	void make_L();
	void make_A();
	void make_B();
	area ar;

	double alpha;
	vector<vector3> gamma;

	void solve_gauss(vector<vector<double> > & matrix, vector<double> & right_part, vector<double> & solution) const;
	double calc_functional_FI(const vector <double> & solution);
	double calc_functional_FI_alpha_gamma(const vector <double> & solution);

	void print_solution(const vector<double> & solution, const string & filename);
	inverse_config cfg;
};

#endif // INVERSE_H
