#define _USE_MATH_DEFINES
#include "direct.h"

cube::cube()
{
    for(size_t i = 0; i < 8; i++)
        nodes[i] = NULL;
    for(size_t i = 0; i < 6; i++)
        neighbor[i] = NULL;
    mes = 0;
}

void cube::init()
{
    double hx = nodes[1]->x - nodes[0]->x;
    double hy = nodes[2]->y - nodes[0]->y;
    double hz = nodes[4]->z - nodes[0]->z;
    mes = hx * hy * hz;
    barycenter = point(nodes[0]->x + hx / 2.0, nodes[0]->y + hy / 2.0, nodes[0]->z + hz / 2.0);

    static const double gauss_points_1d[3] =
    {
        - sqrt(3.0 / 5.0),
        0.0,
        sqrt(3.0 / 5.0)
    };
    static const double gauss_weights_1d[3] =
    {
        5.0 / 9.0,
        8.0 / 9.0,
        5.0 / 9.0
    };

    for(size_t i = 0; i < 3; i++)
        for(size_t j = 0; j < 3; j++)
            for(size_t k = 0; k < 3; k++)
            {
                gauss_weights[i * j * k] = gauss_weights_1d[i] * gauss_weights_1d[j] * gauss_weights_1d[k];
                gauss_points[i * j * k] = point(gauss_points_1d[i], gauss_points_1d[j], gauss_points_1d[k]);
            }
    jacobian = mes / 8.0;

    for(size_t j = 0; j < 27; j++)
    {
        gauss_points[j].x = (gauss_points[j].x + 1.0) * hx / 2.0 + nodes[0]->x;
        gauss_points[j].y = (gauss_points[j].y + 1.0) * hy / 2.0 + nodes[0]->y;
        gauss_points[j].z = (gauss_points[j].z + 1.0) * hz / 2.0 + nodes[0]->z;
    }
}

vector3 cube::get_B(point x) const
{
//    double dx = x.x - barycenter.x, dy = x.y - barycenter.y, dz = x.z - barycenter.z;
//    double r = sqrt(dx * dx + dy * dy + dz * dz), r2 = 1.0 / (r * r);
//    double bla = mes / (4.0 * M_PI * r * r * r);
//    double bx = bla * (p.x * (3.0 * dx * dx * r2 - 1.0) + p.y * (3.0 * dx * dy * r2) + p.z * (3.0 * dx * dz * r2));
//    double by = bla * (p.x * (3.0 * dx * dy * r2) + p.y * (3.0 * dy * dy * r2 - 1.0) + p.z * (3.0 * dy * dz * r2));
//    double bz = bla * (p.x * (3.0 * dx * dz * r2) + p.y * (3.0 * dy * dz * r2) + p.z * (3.0 * dz * dz * r2 - 1.0));
//    return vector3(bx, by, bz);

    vector3 result(0, 0, 0);
    for(size_t i = 0; i < 27; i++)
    {
        double dx = x.x - gauss_points[i].x, dy = x.y - gauss_points[i].y, dz = x.z - gauss_points[i].z;
        double r = sqrt(dx * dx + dy * dy + dz * dz), r2 = 1.0 / (r * r);
        double bla = /*mes*/jacobian * gauss_weights[i] / (4.0 * M_PI * r * r * r);
        double bx = bla * (p.x * (3.0 * dx * dx * r2 - 1.0) + p.y * (3.0 * dx * dy * r2) + p.z * (3.0 * dx * dz * r2));
        double by = bla * (p.x * (3.0 * dx * dy * r2) + p.y * (3.0 * dy * dy * r2 - 1.0) + p.z * (3.0 * dy * dz * r2));
        double bz = bla * (p.x * (3.0 * dx * dz * r2) + p.y * (3.0 * dy * dz * r2) + p.z * (3.0 * dz * dz * r2 - 1.0));
        result = result + vector3(bx, by, bz);
    }
    return result;
}

bool include::inside(const cube & p) const
{
    return p.barycenter.x >= x_min && p.barycenter.x <= x_max &&
           p.barycenter.y >= y_min && p.barycenter.y <= y_max &&
           p.barycenter.z >= z_min && p.barycenter.z <= z_max;
}

istream & operator >> (istream & is, include & a)
{
    is >> a.x_min >> a.x_max >> a.y_min >> a.y_max >> a.z_min >> a.z_max >> a.p.x >> a.p.y >> a.p.z;
    return is;
}

vector3 area::get_B(point x) const
{
    vector3 result(0, 0, 0);
    for(size_t i = 0; i < cubes.size(); i++)
    {
        result = result + cubes[i].get_B(x);
    }
    return result;
}

double area::get_abs_B(point x) const
{
    return get_B(x).norm();
}

void area::generate(const string & filename)
{
    ifstream ifs(filename);
    if(!ifs.good())
    {
        cerr << "Error: " << filename << " not good." << endl;
        exit(EXIT_FAILURE);
    }
    double x_min, x_max, y_min, y_max, z_min, z_max;
    ifs >> x_min >> x_max >> y_min >> y_max >> z_min >> z_max;
    size_t x_num, y_num, z_num;
    ifs >> x_num >> y_num >> z_num;

    size_t inc_num;
    ifs >> inc_num;
    vector<include> includes;
    includes.resize(inc_num);
    for(size_t i = 0; i != inc_num; i++)
        ifs >> includes[i];

    if(!ifs.good())
    {
        cerr << "Error: " << filename << " not good." << endl;
        exit(EXIT_FAILURE);
    }
    ifs.close();

    double hx = (x_max - x_min) / static_cast<double>(x_num),
           hy = (y_max - y_min) / static_cast<double>(y_num),
           hz = (z_max - z_min) / static_cast<double>(z_num);
    nodes.resize((x_num + 1) * (y_num + 1) * (z_num + 1));
    for(size_t i = 0, m = 0; i <= x_num; i++)
    {
        double x_curr = x_min + hx * static_cast<double>(i);
        for(size_t j = 0; j <= y_num; j++)
        {
            double y_curr = y_min + hy * static_cast<double>(j);
            for(size_t k = 0; k <= z_num; k++)
            {
                double z_curr = z_min + hz * static_cast<double>(k);
                nodes[m].x = x_curr;
                nodes[m].y = y_curr;
                nodes[m].z = z_curr;
                m++;
            }
        }
    }

    cubes.resize(x_num * y_num * z_num);
    for(size_t i = 0 , m = 0; i < x_num; i++)
    {
        for(size_t j = 0; j < y_num; j++)
        {
            for(size_t k = 0; k < z_num; k++)
            {
                cubes[m].nodes[0] = nodes.data() + (i * (y_num + 1) + j) * (z_num + 1) + k;
                cubes[m].nodes[1] = nodes.data() + ((i + 1) * (y_num + 1) + j) * (z_num + 1) + k;
                cubes[m].nodes[2] = nodes.data() + (i * (y_num + 1) + j + 1) * (z_num + 1) + k;
                cubes[m].nodes[3] = nodes.data() + ((i + 1) * (y_num + 1) + j + 1) * (z_num + 1) + k;
                cubes[m].nodes[4] = nodes.data() + (i * (y_num + 1) + j) * (z_num + 1) + k + 1;
                cubes[m].nodes[5] = nodes.data() + ((i + 1) * (y_num + 1) + j) * (z_num + 1) + k + 1;
                cubes[m].nodes[6] = nodes.data() + (i * (y_num + 1) + j + 1) * (z_num + 1) + k + 1;
                cubes[m].nodes[7] = nodes.data() + ((i + 1) * (y_num + 1) + j + 1) * (z_num + 1) + k + 1;
                cubes[m].init();
                for(size_t i = 0; i < includes.size(); i++)
                    if(includes[i].inside(cubes[m]))
                        cubes[m].p = includes[i].p;
                if(i > 0)         cubes[m].neighbor[0] = cubes.data() + ((i - 1) * y_num + j) * z_num + k;
                if(i < x_num - 1) cubes[m].neighbor[1] = cubes.data() + ((i + 1) * y_num + j) * z_num + k;
                if(j > 0)         cubes[m].neighbor[2] = cubes.data() + (i * y_num + j - 1) * z_num + k;
                if(j < y_num - 1) cubes[m].neighbor[3] = cubes.data() + (i * y_num + j + 1) * z_num + k;
                if(k > 0)         cubes[m].neighbor[4] = cubes.data() + (i * y_num + j) * z_num + k - 1;
                if(k < z_num - 1) cubes[m].neighbor[5] = cubes.data() + (i * y_num + j) * z_num + k + 1;
                cubes[m].num = m;
                m++;
            }
        }
    }

    for(size_t i = 0; i < cubes.size(); i++)
        cubes[i].init();
}
