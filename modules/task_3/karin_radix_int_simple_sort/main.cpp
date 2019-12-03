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
 /* for (int i = 0; i < 100; i ++)
    cout << res[i]<<endl;*/
  for (int i = 0; i < 99; i++)
    ASSERT_TRUE(res[i] <= res[i + 1]);
}

TEST(Radix_sort, Par_Radix_sort_is_working)
{
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    std::vector<int> vec(100);
    std::vector<int> res(100);
    std::vector<int> res2(100);
    if (rank == 0){
      for (int i = 100; i > 0; i--)
        vec[100 - i] = i * i * i * i;
      for (int i = 0; i < 100; i += 2)
        vec[i] = -vec[i];
    }

    res = Par_Radix_sort(vec);
    
    if (rank==0)
    { 
      res2 = Radix_sort(vec);
      ASSERT_EQ(res, res2);
    }
}

TEST(Radix_sort, Parallel_with_rand_1000)
{
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> vec;
  std::vector<int> res;
  std::vector<int> res2;
  if (rank == 0) {
    vec=GetRandVec(1000);
  }

  res = Par_Radix_sort(vec);

  if (rank == 0)
  {
    res2 = Radix_sort(vec);
    ASSERT_EQ(res, res2);
  }
}

TEST(Radix_sort, Parallel_with_rand_2000)
{
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  std::vector<int> vec;
  std::vector<int> res;
  std::vector<int> res2;
  if (rank == 0) {
    vec = GetRandVec(2000);
  }

  res = Par_Radix_sort(vec);

  if (rank == 0)
  {
    res2 = Radix_sort(vec);
    ASSERT_EQ(res, res2);
  }
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
