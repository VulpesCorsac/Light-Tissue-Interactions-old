#pragma once

#include "Vector3.h"

#include <vector>

using namespace std;

void Bresenham3D(const Vector3D<int>& start, const Vector3D<int>& finish, vector<Vector3D<int>>& points){
    Vector3D<int> point = start;
    int dx = finish.x - start.x;
    int dy = finish.y - start.y;
    int dz = finish.z - start.z;
    int x_inc = (dx < 0) ? -1 : 1;
    int l = abs(dx);
    int y_inc = (dy < 0) ? -1 : 1;
    int m = abs(dy);
    int z_inc = (dz < 0) ? -1 : 1;
    int n = abs(dz);
    int dx2 = 2 * l;
    int dy2 = 2 * m;
    int dz2 = 2 * n;

    int err_1, err_2;

    if ((l >= m) && (l >= n)) {
        err_1 = dy2 - l;
        err_2 = dz2 - l;
        for (int i = 0; i < l; i++) {
            points.push_back(point);
            if (err_1 > 0) {
                point.y += y_inc;
                err_1 -= dx2;
            }
            if (err_2 > 0) {
                point.z += z_inc;
                err_2 -= dx2;
            }
            err_1 += dy2;
            err_2 += dz2;
            point.x += x_inc;
        }
    } else if ((m >= l) && (m >= n)) {
        err_1 = dx2 - m;
        err_2 = dz2 - m;
        for (int i = 0; i < m; i++) {
            points.push_back(point);
            if (err_1 > 0) {
                point.x += x_inc;
                err_1 -= dy2;
            }
            if (err_2 > 0) {
                point.z += z_inc;
                err_2 -= dy2;
            }
            err_1 += dx2;
            err_2 += dz2;
            point.y += y_inc;
        }
    } else {
        err_1 = dy2 - n;
        err_2 = dx2 - n;
        for (int i = 0; i < n; i++) {
            points.push_back(point);
            if (err_1 > 0) {
                point.y += y_inc;
                err_1 -= dz2;
            }
            if (err_2 > 0) {
                point.x += x_inc;
                err_2 -= dz2;
            }
            err_1 += dy2;
            err_2 += dx2;
            point.z += z_inc;
        }
    }
    points.push_back(point);
}
