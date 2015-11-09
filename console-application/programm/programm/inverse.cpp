#include "inverse.h"

// Конфиг по умолчанию
S_InverseConfig::S_InverseConfig(){

	use_alpha = true;   // Использовать регуляризацию по альфа
	use_gamma = true;   // Использовать регуляризацию по гамма
	alpha0 = 1e-15;     // Начальное значение alpha
	dalpha = 2.0;       // Шаг увеличения alpha
	alpha_coeff = 10.0; // Во сколько раз может возрати значение функционала
	gamma0 = 1e-15;     // Начальное значение gamma
	dgamma = 2.0;       // Шаг увеличения gamma
	gamma_coeff = 3.0;  // На сколько порядков может возрасти значение функционала
	gamma_diff = 1e-5;  // Разница между соседями, которую считаем уже разницей
}

// Ввод конфига из файла
istream & operator >> (ifstream &is, S_InverseConfig &a){

	is >> a.use_alpha >> a.use_gamma
		 >> a.alpha0 >> a.dalpha >> a.alpha_coeff
		 >> a.gamma0 >> a.dgamma >> a.gamma_coeff
		 >> a.gamma_diff;
	return is;
}

// Распечатка текущего конфига
ostream & operator << (ostream &os, const S_InverseConfig &a){

	os << "-- BEGIN CONFIG --" << endl;
	os << "use_alpha   = " << a.use_alpha << endl
		 << "use_gamma   = " << a.use_gamma << endl
		 << "alpha0      = " << a.alpha0 << endl
		 << "dalpha      = " << a.dalpha << endl
		 << "alpha_coeff = " << a.alpha_coeff << endl
		 << "gamma0      = " << a.gamma0 << endl
		 << "dgamma      = " << a.dgamma << endl
		 << "gamma_coeff = " << a.gamma_coeff << endl
		 << "gamma_diff  = " << a.gamma_diff << endl;
	os << "-- END CONFIG --" << endl;
	return os;
}

// Ввод данных
void C_Inverse::input(const string &fn_input){

	ifstream ifs(fn_input);
	// Генерируем область
	ar.generate(ifs);
	K = ar.cubes.size();

	// Читаем приемники и выводим аналитику
	ofstream ofs("bx_analytical.txt");
	//ifstream ifs(fn_receivers);
	ifs >> N;
	receivers.resize(N);
	for(size_t i = 0; i < N; i++){
		// Читаем приемники
		ifs >> receivers[i].first.x >> receivers[i].first.y >> receivers[i].first.z;
		// Считаем аналитику
		receivers[i].second = ar.get_B(receivers[i].first);
		// Выводим аналитику
		ofs << receivers[i].first.x << "\t" << receivers[i].second.x << endl;
    //ofs << receivers[i].first.y << "\t" << receivers[i].second.y << endl;
    //ofs << receivers[i].first.z << "\t" << receivers[i].second.z << endl;
	}
	//ifs.close();
	ofs.close();

	A.resize(3 * K);
#pragma omp parallel for
  for(int i = 0; i < A.size(); i++){
    A[i].resize(3 * K);
  }

	b.resize(3 * K);
	L.resize(3 * K);
#pragma omp parallel for
  for(int i = 0; i < L.size(); i++){
    L[i].resize(N);
  }

	// Читаем конфиг
	ifs >> cfg;
	ifs.close();
	cout << cfg;
}

// Делаем вспомогательную матрицу L
void C_Inverse::make_L(){
#pragma omp parallel for
	// Применяем силу товарища Гаусса
	for(int mu = 0; mu < K; mu++){
		for(int i = 0; i < N; i++){
			for (int j = 0; j < 27; j++){

				double dx = receivers[i].first.x - ar.cubes[mu].gauss_points[j].x;
				double dy = receivers[i].first.y - ar.cubes[mu].gauss_points[j].y;
        double dz = receivers[i].first.z - ar.cubes[mu].gauss_points[j].z;
        double r = sqrt(dx * dx + dy * dy + dz * dz);
        double r2 = 1.0 / (r * r);
        double bla = (ar.cubes[mu].jacobian * ar.cubes[mu].gauss_weights[j]) / (4.0 * M_PI * r * r * r);

        L[3 * mu][i][0] += bla * (3.0 * dx * dx * r2 - 1.0);
        L[3 * mu][i][1] += bla * (3.0 * dx * dy * r2);
        L[3 * mu][i][2] += bla * (3.0 * dx * dz * r2);

        L[3 * mu + 1][i][0] += bla * (3.0 * dx * dy * r2);
        L[3 * mu + 1][i][1] += bla * (3.0 * dy * dy * r2 - 1.0);
        L[3 * mu + 1][i][2] += bla * (3.0 * dy * dz * r2);

        L[3 * mu + 2][i][0] += bla * (3.0 * dx * dz * r2);
        L[3 * mu + 2][i][1] += bla * (3.0 * dy * dz * r2);
        L[3 * mu + 2][i][2] += bla * (3.0 * dz * dz * r2 - 1.0);
			}
		}
	}
}
// Делаем основную матрицу A
void C_Inverse::make_A(){
#pragma omp parallel for
  for(int q = 0; q < A.size(); q++){
    for(int s = 0; s < A[q].size(); s++){
      A[q][s] = 0.0;
      for(int i = 0; i < N; i++){
        A[q][s] += L[q][i] * L[s][i];
      }
    }
  }
}
// Делаем правую часть b
void C_Inverse::make_B(){
#pragma omp parallel for
	for(int q = 0; q < b.size(); q++){
		b[q] = 0.0;

    for(int i = 0; i < N; i++){      
      b[q] += L[q][i] * receivers[i].second;
    }
	}
}

// Считаем значения функционала без альфа-гамма
double C_Inverse::calc_functional_FI(const vector<double> &solution){
	// Заполняем значения pi в ячейках области ar
#pragma omp parallel for
	for(int i = 0; i < ar.cubes.size(); i++){
		ar.cubes[i].p = C_Vector3(solution[3 * ar.cubes[i].num],
			                        solution[3 * ar.cubes[i].num + 1],
			                        solution[3 * ar.cubes[i].num + 2]);
	}
	// Ну а теперь считаем сам функционал
	double f = 0;
#pragma omp parallel for
	for(int i = 0; i < N; i++){
		C_Vector3 b_a = receivers[i].second;
		C_Vector3 b_c = ar.get_B(receivers[i].first);
		C_Vector3 d = b_a - b_c;
    //f += d.x * d.x;
    //f += d.y * d.y;
		//f += d.z * d.z;
    f += d.x * d.x + d.y * d.y + d.z * d.z;
	}
	return f;
}

// Считаем значения функционала с альфа-гамма
double C_Inverse::calc_functional_FI_alpha_gamma(const vector<double> &solution){
	// Простой функционал
	double f = calc_functional_FI(solution);
	// Добавка от альфы
#pragma omp parallel for
  for(int i = 0; i < K; i++){
    f += alpha * ar.cubes[i].p.norm2();
  }

	// Добавка от гаммы
	for(size_t k = 0; k < K; k++){
		C_Vector3 sum(0.0, 0.0, 0.0);
		for(size_t a = 0; a < 6; a++){
			if(ar.cubes[k].neighbor[a]){
				C_Vector3 tmp = ar.cubes[k].p - ar.cubes[k].neighbor[a] -> p;
        //sum = sum + C_Vector3(tmp.x * tmp.x, 0.0, 0.0);
        //sum = sum + C_Vector3(0.0, tmp.y * tmp.y, 0.0);
				//sum = sum + C_Vector3(0.0, 0.0, tmp.z * tmp.z);
        sum = sum + C_Vector3(tmp.x * tmp.x, tmp.y * tmp.y, tmp.z * tmp.z);
			}
		}
		f += sum * gamma[k];
	}

	return f;
}

// Основные расчеты
void C_Inverse::calc(){

	cout << "Make L" << endl;
	make_L();
	cout << "Make A" << endl;
	make_A();
	cout << "Make B" << endl;
	make_B();

	vector<vector<double>> M(A);
	vector<double> rp(b);
  /*for(int i = 0; i < A.size(); i++)
    for(int j = 0; j < A[i].size(); j++)
      printf("%lf\n", A[i][j]);*/

	// Решаем СЛАУ
	vector<double> solution(A.size());
	cout << "Solving SLAE" << endl;
	solve_gauss(M, rp, solution);
  /*for(int i = 0; i < solution.size(); i++)
    printf("%lf\n", solution[i]);*/

	double func_prev, func_curr;

	// Замутим регуляризацию по альфа
	if(cfg.use_alpha){
		alpha = cfg.alpha0;
		func_curr = func_prev = calc_functional_FI(solution);
		
    while(func_prev * cfg.alpha_coeff >= func_curr){

			cout << "Alpha = " << alpha << " \tFI = " << func_curr << " \tFI_prev = " << func_prev << endl;
			M = A;
			rp = b;
#pragma omp parallel for
      for(int i = 0; i < A.size(); i++){
        A[i][i] += alpha;
      }
			solve_gauss(M, rp, solution);
			func_curr = calc_functional_FI(solution);
			alpha *= cfg.dalpha;
		}
	}	else {
		alpha = 0;
		func_curr = func_prev = calc_functional_FI(solution);
	}

	// Снова делаем бэкап, на этот раз с альфой
	vector<vector<double> > A_alpha(A);
	// Ну, мы там лишний раз домножили, непорядок
	alpha /= cfg.dalpha;
#pragma omp parallel for
	for(int i = 0; i < A.size(); i++){
		A_alpha[i][i] += alpha;
  }

	// Тут будем хранить гаммы
	gamma.resize(K);
#pragma omp parallel for
	for(int i = 0; i < K; i++){
		gamma[i] = C_Vector3(cfg.gamma0, cfg.gamma0, cfg.gamma0);
  }

	// Восстановим функционал
	M = A_alpha;
	rp = b;
	solve_gauss(M, rp, solution);
	func_curr = func_prev = calc_functional_FI(solution);

	// Замутим регуляризацию по гамма
	if(cfg.use_gamma){
		size_t w = 0;
		bool is_changed = true;
		// Цикл пока не достигнем ухудшения в 3 порядка и пока изменяются гаммы
		while((fabs(log10(func_prev) - log10(func_curr)) <= cfg.gamma_coeff || func_curr <= func_prev) && is_changed){
			
      is_changed = false;

			M = A_alpha;
			rp = b;
			// По всем ячейкам
			for(size_t k = 0; k < K; k++){
				// Сюда будем суммировать вклады в диагональ
        C_Vector3 add_di;
				// А тут - считать число живых соседей
				double add_di_coeff = 0.0;
				// По всем соседям
				for(size_t a = 0; a < 6; a++){
					// Воображаемых соседей рассматривать не будем
					if(ar.cubes[k].neighbor[a]){
						size_t m = ar.cubes[k].neighbor[a] -> num;
						// Диагональ отдельно будет
						if(k != m){
							// Внедиагональные элементы
              C_Vector3 add = gamma[k] + gamma[m];
							M[k * 3][m * 3] -= add.x;
							M[k * 3 + 1][m * 3 + 1] -= add.y;
							M[k * 3 + 2][m * 3 + 2] -= add.z;
							// Кусочек диагонального элемента
							add_di = add_di + gamma[m];
							add_di_coeff++;
						}
					}
				}
				// Второй кусочек диагонального элемента
				add_di = add_di + add_di_coeff * gamma[k];
				M[k * 3][k * 3] += add_di.x;
				M[k * 3 + 1][k * 3 + 1] += add_di.y;
				M[k * 3 + 2][k * 3 + 2] += add_di.z;
			}

			// Посчитаем СЛАУ и функционал
			solve_gauss(M, rp, solution);
			func_curr = calc_functional_FI(solution);
			cout << "Gamma (" << w++ << ") \tFI = " << func_curr << " \tFI_prev = " << func_prev << endl;
      
			// Посчитаем новые значения гаммы
			// По всем ячейкам
			for(size_t k = 0; k < K; k++){
				// По всем соседям
				for(size_t a = 0; a < 6; a++){
					// Воображаемых соседей рассматривать не будем
					if(ar.cubes[k].neighbor[a]){
						// По всем координатам
						for(size_t g = 0; g < 3; g++){
							// Если разница между соотв. компонентами P больше порядка
							// И при этом это не околонулевые значения
							// Тогда обновим гамму и запомним, что кто-то еще шевелится
							if(fabs(log10(ar.cubes[k].p[g]) - log10(ar.cubes[k].neighbor[a]->p[g])) > 1.0 &&
								fabs(ar.cubes[k].p[g]) > cfg.gamma_diff && fabs(ar.cubes[k].neighbor[a]->p[g]) > cfg.gamma_diff){
								
                gamma[k][g] *= cfg.dgamma;
								is_changed = true;
							}
						}
					}
				}
			}
			print_solution(solution, "output.txt");
		}
	}

	// Распечатаем значения функционалов
    cout << "FI_std = " << calc_functional_FI(solution) << endl;
    cout << "FI_reg = " << calc_functional_FI_alpha_gamma(solution) << endl;

    // Распечатаем посчитанное значение Bx
    for(size_t i = 0; i < ar.cubes.size(); i++){
        ar.cubes[i].p = C_Vector3(
          solution[3 * ar.cubes[i].num],
          solution[3 * ar.cubes[i].num + 1],
          solution[3 * ar.cubes[i].num + 2]);
    }
    ofstream ofs2("bx_calculated.txt");
    for(size_t i = 0; i < N; i++){
        ofs2 << receivers[i].first.x << "\t" << ar.get_B(receivers[i].first).x << endl;
    }
    ofs2.close();

    // Напечатаем решение в файл
    print_solution(solution, "output.txt");
}

// Печать решения в файл
void C_Inverse::print_solution(const vector<double> &solution, const string &filename){
  // Ищем куб куда попало (0, 0, 0)
  C_Cube *q = NULL;
  // Сперва найдем куда попала серединка
  for(size_t i = 0; i < ar.cubes.size() && !q; i++){
    const double eps = 1e-12;
    double x_c = 2500;
    double y_c = 0;
    double z_c = -750;
    if(ar.cubes[i].nodes[0]->x <= x_c + eps && ar.cubes[i].nodes[1]->x >= x_c - eps &&
       ar.cubes[i].nodes[0]->y <= y_c + eps && ar.cubes[i].nodes[2]->y >= y_c - eps &&
       ar.cubes[i].nodes[0]->z <= z_c + eps && ar.cubes[i].nodes[4]->z >= z_c - eps){

      q = &(ar.cubes[i]);
    }
  }
  // Если не нашли - печалька
  if(!q){
    cerr << "Cube with (0, 0, 0) not found!" << endl;
    return;
  }
  // Отшагаем от серединки влево до упора
  while(q->neighbor[0]){
    q = q->neighbor[0];
  }
  // А затем вверх до упора
  while(q->neighbor[5]){
    q = q->neighbor[5];
  }

  // Распечатаем значения P в ячейках
  ofstream ofs(filename);
  bool end_z = false;
  while(!end_z)
  {
    bool end_x = false;
    C_Cube *curr = q;
    while(!end_x){
      double px = solution[curr->num * 3];
      ofs << px << "\t";

      curr = curr -> neighbor[1]; // Сосед справа
      if(!curr) end_x = true;
    }
    ofs << endl;

    q = q->neighbor[4]; // Сосед снизу
    if(!q) end_z = true;
  }
  ofs.close();
}

// Решаем СЛАУ методом товарища Гаусса
void C_Inverse::solve_gauss(vector<vector<double> > &matrix, vector<double> &right_part, vector<double> &solution) const {
	size_t n = matrix.size();

	//верхний треугольный вид
	for(size_t i = 0; i < n; i++){
		if(!matrix[i][i]){
			bool flag = false;
			for(size_t j = i + 1; j < n && !flag; j++){
				if(matrix[j][i]){
#pragma omp parallel for shared(i, j)
					for(int k = i; k < n; k++){
						double tmp = matrix[i][k];
						matrix[i][k] = matrix[j][k];
						matrix[j][k] = tmp;
					}
					double tmp = right_part[i];
					right_part[i] = right_part[j];
					right_part[j] = tmp;
					flag = true;
				}
      }
		}
		right_part[i] = right_part[i] / matrix[i][i];    
    for(size_t j = n - 1, jj = n; jj > i; j--, jj--){
			matrix[i][j] = matrix[i][j] / matrix[i][i];
    }
#pragma omp parallel for shared(i)
		for(int j = i + 1; j < n; j++){
			right_part[j] -= right_part[i] * matrix[j][i];
			for(size_t k = n - 1, kk = n; kk > i; k--, kk--){
				matrix[j][k] -= matrix[i][k] * matrix[j][i];
      }
		}
	}
	//диагональный вид
	for(int i = n - 1; i > 0; i--){
		for(size_t j = i - 1, jj = i; jj > 0; j--, jj--){
			right_part[j] -= matrix[j][i] * right_part[i];
    }
  }

#pragma omp parallel for
	for(int i = 0; i < n; i++){
		solution[i] = right_part[i];
  } 
}
