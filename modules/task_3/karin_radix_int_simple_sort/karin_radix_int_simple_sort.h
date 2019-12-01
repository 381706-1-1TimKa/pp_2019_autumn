// Copyright 2019 Karin Timofey
#ifndef MODULES_TASK_3_KARIN_RADIX_INT_SIMPLE_SORT_H_
#define MODULES_TASK_3_KARIN_RADIX_INT_SIMPLE_SORT_H_

#include <vector>
#include <string>

void Radix_sort_iter(std::vector<int>& source, std::vector<int>& res, long* count, long n, int iter);
void Radix_sort_last_iter(std::vector<int>& source, std::vector<int>& res, long* count, long n);
std::vector<int> Radix_sort(std::vector<int>& source);
std::vector<int> Par_Radix_sort(std::vector<int> source);

#endif  // MODULES_TASK_3_KARIN_RADIX_INT_SIMPLE_SORT_H_
