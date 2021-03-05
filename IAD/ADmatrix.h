#ifndef ADMATRIX_H
#define ADMATRIX_H

#include <Eigen/Dense>

using namespace Eigen;

template <class T, size_t M>
class ADmatrix
{
    public:
        ADmatrix() {};

    protected:
        Matrix<T, M, M> m;
};






#endif // ADMATRIX_H
