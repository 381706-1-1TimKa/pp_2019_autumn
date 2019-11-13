// Copyright 2019 Karin Timofey
#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "./bubble_sort.h"


TEST(Check, 1)
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
   res = ParBubbleSort(vec, 100);
    
    if (rank == 0)
    {
      std::cout << vec.size() <<std::endl;

      for (int i=0; i<res.size(); i++)
        std::cout << res[i] << " ";
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
  std::vector<int> vec;
  for (int i=100; i>0; i--)
    vec.push_back(i);
  BubbleSort(vec, 100);
  ASSERT_TRUE(vec[29] == 30);
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
