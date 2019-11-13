// Copyright 2019 Karin Timofey
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iostream>
#include "../../../modules/task_2/karin_bubble_sort/bubble_sort.h"

std::vector<int> GetRandVec(int size){
  if (size <= 0)
    throw -1;
  std::vector<int> vec(size);
  std::mt19937 el;
  el.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++){
    vec[i] = el() % 1000;
  }
  return vec;
}

void BubbleSort(std::vector<int>& vect, int length){
  int tmp;
  for (int i = 0; i < length; i++)
  {
    for (int j = 0; j < length - i-1; j++)
    {
      if (vect[j] > vect[j + 1])
      {
        std::swap(vect[j], vect[j+1]);
      }
    }
  }
}

std::vector<int> ParBubbleSort(std::vector<int>& vect, int len)
{
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  int length = len;
  int num = length / size;
  int ost = length % size;
  int start, it1, it2, count;
  
  
  if (rank < ost)
  {
    start = rank*num+rank;
    count = num+1;//распределяем остаток между первыми процессами
  }
  else
  {
    start = rank*num+ost;
    count=num;
  }

  std::vector<int> local_vec(num+1);
  std::vector<int> neig_vec(num+1);
  std::vector<int> res;
  vect.resize(len);
  std::cout << "rank=" << rank << " start=" << start << " num=" << num << " count=" << count << std::endl;
  for (int i = 0; i < count; i++)
  {
    local_vec[i] = vect[start+i];
  }


  for (int i = 0; i < size; i++)
  {
    //BubbleSort(local_vec, count);
    //if (i%2 == 0) //чётная итерация
    //{
    //  if ((rank % 2 == 0) && (rank != size - 1))
    //  {
    //    it1 = start+count-1;
    //    it2 = start+count;
    //    //while ((vect[it1] > vect[it2]) && (it1>=start) && (it2<=start+count+ncount-1));
    //    //{
    //    //  std::swap(vect[it1], vect[it2]);
    //    //  it1--;
    //    //  it2++;
    //    //}
    //  }
    //}
    //else //нечётная итерация
    //{
    //  if ((rank % 2 == 1) && (rank != size - 1))
    //  {
    //    //it1 = start + count - 1;
    //    //it2 = start + count;
    //    //while ((vect[it1] > vect[it2]) && (it1 >= start) && (it2 <= start + count + ncount - 1));
    //    //{
    //    //  std::swap(vect[it1], vect[it2]);
    //    //  it1--;
    //    //  it2++;
    //    //}
    //  }
    //}
  }
  if (rank == 0)
  {
    res = local_vec;
    for (int i = 1; i < size; i++)
    {
      MPI_Status status;
      MPI_Recv(&neig_vec[0], num+1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
      if (i < ost)
        res.insert(res.end(), neig_vec.begin(), neig_vec.end());
      else
        res.insert(res.end(), neig_vec.begin(), neig_vec.end()-1);
    }
  }
  else
  {
    MPI_Send(&local_vec[0], num+1, MPI_INT, 0, 0, MPI_COMM_WORLD);
  }
  return res;
}
