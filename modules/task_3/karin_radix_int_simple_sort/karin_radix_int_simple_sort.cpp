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

std::vector<int> GetRandVec(int size) {
  if (size <= 0)
    throw - 1;
  std::vector<int> vec(size);
  std::mt19937 el;
  el.seed(static_cast<unsigned int>(time(0)));
  for (int i = 0; i < size; i++) {
    vec[i] = el() % 1000000000;
  }
  return vec;
}

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

std::vector<int> Radix_sort(const std::vector<int>& vec)
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

std::vector<int> Merge_sort(const std::vector<int>& loc_vec, int loc_size, 
                              const std::vector<int> neig_vec, int neig_size)
{
  std::vector<int> res(loc_size+neig_size);
  int l=0, n=0, r=0;
  while (l < loc_size && n < neig_size)
  {
    if (loc_vec[l] < neig_vec[n]) {
      res[r]=loc_vec[l];
      r++;
      l++;
    } else {
      res[r]=neig_vec[n];
      n++;
      r++;
    }
  }
  while (l < loc_size) {
    res[r]=loc_vec[l];
    r++;
    l++;
  }
  while (n < neig_size) {
    res[r]=neig_vec[n];
    r++;
    n++;
  }
  return res;
}

int pow2(int st)
{
  int res = 1;
  for (int i = 0; i < st; i++)
    res *= 2;
  return res;
}

std::vector<int> Par_Radix_sort(std::vector<int> source)
{
  int rank, size;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  
  int length, ost;
  if (rank == 0)
  {
    length = source.size()/size;
    ost = source.size() % size;
  }
  MPI_Bcast(&length, 1, MPI_INT, 0, MPI_COMM_WORLD);

  std::vector<int> local_vec(length);
  std::vector<int> neig_vec;
  MPI_Scatter(&source[0], length, MPI_INT, &local_vec[0], length, MPI_INT, 0, MPI_COMM_WORLD);
  if (rank == 0 && ost != 0){
      local_vec.insert(local_vec.end(), source.end()-ost, source.end());//
  }
  
  local_vec = Radix_sort(local_vec);

  int i = size;
  int iter = 1;
  int sosed;
  int loc_size = length;
  if (rank==0)
    loc_size+=ost;

  while (1)
  {
    if (rank == 0 && i<=1)
    {
    cout << "I am 0 rank. I am here"<<endl;
      for (int i=0; i<loc_size; i++)
        cout << local_vec[i]<<" ";
      cout << endl;
      return local_vec;
    }
    if (i % 2 == 1)
    {
      if (rank == 0)
      {
        MPI_Status status1;
        neig_vec.resize(length*iter);
        sosed= pow2(iter - 1) * (i - 1);
        cout <<"necot " << "rank = " << rank << " sosed=" << sosed << " iter=" << iter << " loc_size="<<loc_size << endl;
        MPI_Recv(&neig_vec[0], length*iter, MPI_INT, sosed, 1, MPI_COMM_WORLD, &status1);
        local_vec.resize(loc_size + length*iter);
        local_vec = Merge_sort(local_vec, loc_size, neig_vec, length*iter);
        loc_size += length * iter;
        /*for (int i=0; i<loc_size; i++)
          cout << local_vec[i]<< " ";
        cout << endl<<endl;*/

      }
      if (rank == pow2(iter - 1) * (i - 1))
      {
        MPI_Send(&local_vec[0], length*iter, MPI_INT, 0, 1, MPI_COMM_WORLD);
        return local_vec;
      }
    }

    if (rank % pow2(iter) == 0)
    {
      sosed = rank + pow2(iter-1);
      neig_vec.resize(length*iter);
      local_vec.resize(loc_size + length*iter);
      MPI_Status status3;
      MPI_Recv(&neig_vec[0], length*iter, MPI_INT, sosed, 3, MPI_COMM_WORLD, &status3);
      local_vec = Merge_sort(local_vec, loc_size, neig_vec, length*iter);
      loc_size += length * iter;
      cout << "rank = " << rank << " sosed=" << sosed << " iter=" << iter << " loc_size=" << loc_size << endl;
      /*for (int i=0; i<loc_size; i++)
        cout << local_vec[i]<< " ";
      cout << endl;*/
    }
    if (rank % pow2(iter) != 0)
    {
      sosed = rank - pow2(iter-1);
      //cout << "rank = " << rank << " sosed=" << sosed << " iter=" << iter << "send" << endl;
      MPI_Send(&local_vec[0], length*iter, MPI_INT, sosed, 3, MPI_COMM_WORLD);
      cout << rank << "ended"<<endl;
      return local_vec;
    }
    iter++;
    i = i/2;
  }

  /*for (int i=0; i<length; i++)
    cout << local_vec[i]<< " ";
  cout << endl<<endl;*/
  return source;

}