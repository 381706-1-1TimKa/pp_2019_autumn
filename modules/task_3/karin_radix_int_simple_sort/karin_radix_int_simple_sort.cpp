// Copyright 2019 Karin Timofey
#include <mpi.h>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include "../../../modules/task_3/karin_radix_int_simple_sort/karin_radix_int_simple_sort.h"
#include<iostream>
using namespace std;

void Radix_sort_iter(std::vector<int>& source, std::vector<int>& res, long* count, long n, int iter)
{
  unsigned char* br = (unsigned char*)source.data() + iter;
  for (int i=0; i<256; i++)
    count[i]=0;

  unsigned char tmp;
  for (int i = 0; i < n; i++){
    tmp = br[i*4];
    count[tmp]++;
  }

  int sum=0;
  for (int i = 0; i < 256; i++){
    sum+=count[i];
    count[i] = sum - count[i];
  }

  long* cp;
  for (int i = 0; i < n; i++){
    res[count[*br]] = source[i];
    count[*br]++;
    br += 4;
  }
}

void Radix_sort_last_iter(vector<int>& source, vector<int>& res, long* count, long n)
{
  unsigned char* br = (unsigned char*)source.data() + 3;
  for (int i = 0; i < 256; i++)
    count[i] = 0;

  unsigned char tmp;
  for (int i = 0; i < n; i++){
    tmp = br[i * 4];
    count[tmp]++;
  }

  int sum = 0;
  for (int i = 128; i < 256; i++){
    sum += count[i];
    count[i] = sum - count[i];
  }

  for (int i = 0; i < 128; i++){
    sum += count[i];
    count[i] = sum - count[i];
  }

  long* cp;
  for (int i = 0; i < n; i++)
  {
    res[count[*br]] = source[i];
    count[*br]++;
    br += 4;
  }
  
}

std::vector<int> Radix_sort(std::vector<int>& vec)
{
  int length = vec.size();
  vector<int> source(vec);
  vector<int> res(length);
  long* count = new long[256];
  Radix_sort_iter(source, res, count, length, 0);
  Radix_sort_iter(res, source, count, length, 1);
  Radix_sort_iter(source, res, count, length, 2);
  Radix_sort_last_iter(res, source, count, length);
  delete[] count;
  return source;

}

std::vector<int> Par_Radix_sort(std::vector<int> source)
{
  /*int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  int length;
  if (rank == 0)
    length = n/size;
  MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);*/
  return source;

}