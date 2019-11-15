// Copyright 2019 Karin Timofey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./bubble_sort.h"


TEST(BubbleSort, values_are_sorted)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  if (rank == 0)
  {
    for (int i=100; i>0; i--)
      vec.push_back(i);
  }
  std::vector<int> res; 
  res = ParBubbleSort(vec);
  if (rank == 0)
  {
     for (int i=0; i<100; i++)
     std::cout << res[i]<< " ";
   std::cout<<std::endl<<std::endl;
    for (int i=0; i<res.size()-1; i++)
    ASSERT_TRUE(res[i]<=res[i+1]);
  }
}

TEST(BubbleSort, Sequential_and_Parallel_vectors_are_equel)
{
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  if (rank == 0)
  {
    for (int i = 100; i > 0; i--)
      vec.push_back(i);
  }
  std::vector<int> Pres;
  Pres = ParBubbleSort(vec);
  if (rank == 0)
  {
    std::vector<int> Sres;
    Sres = BubbleSort(vec, 100);
    ASSERT_EQ(Pres, Sres);
  }
}

TEST(BubbleSort, Rand_Vector_500)
{
  int n = 500;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  if (rank == 0)
  {
    vec = GetRandVec(n);
   /* for (int i=0; i<n; i++)
      std::cout << vec[i]<< " ";
    std::cout<<std::endl<<std::endl;*/
  }
  std::vector<int> Pres;
  Pres = ParBubbleSort(vec);
  if (rank == 0)
  {
   /* for (int i = 0; i < n; i++)
      std::cout << Pres[i] << " ";
    std::cout << std::endl<<std::endl;*/
    std::vector<int> Sres;
    Sres = BubbleSort(vec, n);
    /*for (int i = 0; i < n; i++)
      std::cout << Sres[i] << " ";
    std::cout << std::endl << std::endl;*/
    ASSERT_EQ(Pres, Sres);
  }
}

TEST(BubbleSort, Rand_Vector_2000)
{
  int n = 2000;
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vec;
  if (rank == 0)
  {
    vec = GetRandVec(n);
    /* for (int i=0; i<n; i++)
       std::cout << vec[i]<< " ";
     std::cout<<std::endl<<std::endl;*/
  }
  std::vector<int> Pres;
  Pres = ParBubbleSort(vec);
  if (rank == 0)
  {
    /* for (int i = 0; i < n; i++)
       std::cout << Pres[i] << " ";
     std::cout << std::endl<<std::endl;*/
    std::vector<int> Sres;
    Sres = BubbleSort(vec, n);
    /*for (int i = 0; i < n; i++)
      std::cout << Sres[i] << " ";
    std::cout << std::endl << std::endl;*/
    ASSERT_EQ(Pres, Sres);
  }
}

TEST(BubbleSort, Can_create_vector)
{
  ASSERT_NO_THROW(std::vector<int> vec = GetRandVec(1000));
}

TEST(BubbleSort, Can_not_create_vector_negative_size)
{
  ASSERT_ANY_THROW(std::vector<int> vec = GetRandVec(-1000));
}


TEST(BubbleSort, sort_is_working)
{
  std::vector<int> vec, res;
  for (int i=100; i>0; i--)
    vec.push_back(i);
  res = BubbleSort(vec, 100);
  ASSERT_TRUE(res[29] == 30);
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
