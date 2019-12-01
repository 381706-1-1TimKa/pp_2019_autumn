// Copyright 2019 Karin Timofey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include "./karin_radix_int_simple_sort.h"
#include <iostream>
using namespace std;

TEST(Radix_sort, radix_sort_is_working_with_unsigned)
{
  vector<int> mas(100);
  vector<int> res(100);
  for (int i=100; i>0; i--)
    mas[100-i] = i*i*i*i;
  //for (int i=0; i<100; i+=2)
  //  mas[i] = -mas[i];
  res = Radix_sort(mas);
  for (int i=0; i<99; i++)
    ASSERT_TRUE(res[i]<=res[i+1]);
}

TEST(Radix_sort, radix_sort_is_working_with_signed)
{
  vector<int> mas(100);
  vector<int> res(100);
  for (int i = 100; i > 0; i--)
    mas[100 - i] = i * i * i * i;
  for (int i=0; i<100; i+=2)
    mas[i] = -mas[i];
  res = Radix_sort(mas);
  for (int i = 0; i < 100; i ++)
    cout << res[i]<<endl;
  for (int i = 0; i < 99; i++)
    ASSERT_TRUE(res[i] <= res[i + 1]);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    MPI_Init(&argc, &argv);

    ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
    ::testing::TestEventListeners& listeners =
        ::testing::UnitTest::GetInstance()->listeners();

    listeners.Release(listeners.default_result_printer());
    listeners.Release(listeners.default_xml_generator());

    listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
    return RUN_ALL_TESTS();
}
