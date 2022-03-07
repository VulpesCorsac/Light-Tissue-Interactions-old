#pragma once

#ifndef ENABLE_CHECK_CONTRACTS
    #define ENABLE_CHECK_CONTRACTS
#endif // ENABLE_CHECK_CONTRACTS

#include "Mesh3.h"

#include "../Utils/Utils.h"

#include <gtest/gtest.h>

#include <tuple>

using namespace Math_NS;
using namespace Utils_NS;
using namespace std;

class Mesh3Tests : public ::testing::Test {
protected:
    Mesh3Tests() {
        mesh = Mesh3<int>(N, M, L, value);
    }

protected:
    static constexpr int N = 10;
    static constexpr int M = 20;
    static constexpr int L = 30;
    static constexpr int value = 100;

    Mesh3<int> mesh;
};

TEST_F(Mesh3Tests, Constructor) {
    EXPECT_EQ(isize(mesh.data), N);
    for (int i = 0; i <= N; ++i) {
        if (i < N) {
            EXPECT_EQ(isize(mesh.data.at(i)), M);
            for (int j = 0; j <= M; ++j) {
                if (j < M) {
                    EXPECT_EQ(isize(mesh.data.at(i).at(j)), L);
                    for (int k = 0; k <= L; ++k) {
                        if (k < L)
                            ASSERT_EQ(mesh.data.at(i).at(j).at(k), value);
                        else
                            EXPECT_THROW(ignore = mesh.data.at(i).at(j).at(k), out_of_range);
                    }
                } else
                    EXPECT_THROW(ignore = mesh.data.at(i).at(j), out_of_range);
            }
        } else
            EXPECT_THROW(ignore = mesh.data.at(i), out_of_range);
    }
}

TEST_F(Mesh3Tests, ValueGetter) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            for (int k = 0; k < L; ++k)
                ASSERT_EQ(mesh.value(i, j, k), value);

    EXPECT_THROW(ignore = mesh.value(N, 0, 0), out_of_range);
    EXPECT_THROW(ignore = mesh.value(0, M, 0), out_of_range);
    EXPECT_THROW(ignore = mesh.value(0, 0, L), out_of_range);
}

TEST_F(Mesh3Tests, DataSetter) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < M; ++j)
            for (int k = 0; k < L; ++k) {
                mesh.value(i, j, k) = value + 1;
                ASSERT_EQ(mesh.value(i, j, k), value+1);
            }

    EXPECT_THROW(mesh.value(N, 0, 0) = value + 1, out_of_range);
    EXPECT_THROW(mesh.value(0, M, 0) = value + 1, out_of_range);
    EXPECT_THROW(mesh.value(0, 0, L) = value + 1, out_of_range);
}
