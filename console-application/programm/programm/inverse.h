#ifndef __INVERSE_H
#define __INVERSE_H

#include "direct.h"

struct S_InverseConfig
{
	bool use_alpha;     // Использовать регуляризацию по альфа
	bool use_gamma;     // Использовать регуляризацию по гамма
	double alpha0;      // Начальное значение alpha
	double dalpha;      // Шаг увеличения alpha
	double alpha_coeff; // Во сколько раз может возрати значение функционала
	double gamma0;      // Начальное значение gamma
	double dgamma;      // Шаг увеличения gamma
	double gamma_coeff; // На сколько порядков может возрасти значение функционала
	double gamma_diff;  // Разница между соседями, которую считаем уже разницей
  
  S_InverseConfig();
	friend istream & operator >> (istream & is, S_InverseConfig &a);
	friend ostream & operator << (ostream & os, const S_InverseConfig &a);
};

class C_Inverse
{
public:
	void input(const string &fn_input);//const string & fn_area, const string & fn_receivers, const string & fn_config);
	void calc();

protected:
	vector<vector<double>> A;
	vector<double> b;
	vector<vector<C_Vector3>> L;
	size_t K, N;
	vector<pair<C_Point, C_Vector3>> receivers;

	void make_L();
	void make_A();
	void make_B();
  C_Area ar;

	double alpha;
	vector<C_Vector3> gamma;

	void solve_gauss(vector<vector<double>> &matrix, vector<double> &right_part, vector<double> &solution) const;
	double calc_functional_FI(const vector<double> &solution);
	double calc_functional_FI_alpha_gamma(const vector<double> &solution);

	void print_solution(const vector<double> &solution, const string &filename);
  S_InverseConfig cfg;
};

#endif // __INVERSE_H
