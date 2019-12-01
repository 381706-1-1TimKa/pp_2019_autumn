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

void Radix_sort_iter(int* source, int* res, long* count, long n, int iter)
{
  unsigned char* br = (unsigned char*)source + iter;
  for (int i=0; i<256; i++)
    count[i]=0;

  unsigned char tmp;
  for (int i = 0; i < n; i++)
  {
    tmp = br[i*4];
    count[tmp]++;
  }

  int sum=0;
  for (int i = 0; i < 256; i++)
  {
    sum+=count[i];
    count[i] = sum - count[i];
  }

  long* cp;
  for (int i = 0; i < n; i++)
  {
    //cp = count + *br;
    res[count[*br]] = source[i];
    //(*cp)++;
    count[*br]++;
    br += 4;
  }
}

void Radix_sort_last_iter(int* source, int* res, long* count, long n)
{
  unsigned char* br = (unsigned char*)source + 3;
  for (int i = 0; i < 256; i++)
    count[i] = 0;

  unsigned char tmp;
  for (int i = 0; i < n; i++)
  {
    tmp = br[i * 4];
    count[tmp]++;
  }
  int sum = 0;
  for (int i = 128; i < 256; i++)
  {
    sum += count[i];
    count[i] = sum - count[i];
  }
  for (int i = 0; i < 128; i++)
  {
    sum += count[i];
    count[i] = sum - count[i];
  }
  long* cp;
  for (int i = 0; i < n; i++)
  {
    //cp = count + *br;
    res[count[*br]] = source[i];
    //(*cp)++;
    count[*br]++;
    br += 4;
  }
  
}

void Radix_sort(int* source, int n)
{
  int* res = new int [n];
  long* count = new long[256];
  Radix_sort_iter(source, res, count, n, 0);
  Radix_sort_iter(res, source, count, n, 1);
  Radix_sort_iter(source, res, count, n, 2);
  Radix_sort_last_iter(res, source, count, n);
  //std::swap(*res, *source);
  //Radix_sort_last_iter(source, res, count, n);
  delete[] res;
  delete[] count;
}