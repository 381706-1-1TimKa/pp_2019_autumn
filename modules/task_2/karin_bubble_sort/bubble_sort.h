// Copyright 2019 Karin Timofey
#ifndef MODULES_TASK_2_KARIN_BUBBLE_SORT_H_
#define MODULES_TASK_2_KARIN_BUBBLE_SORT_H_

std::vector<int> GetRandVec(int size);
std::vector<int> BubbleSort(std::vector<int>& vect, int length);
std::vector<int> GetMinVec(std::vector<int>& local_vec, int local_size, std::vector<int>& neig_vec, int neig_size);
std::vector<int> GetMaxVec(std::vector<int>& local_vec, int local_size, std::vector<int>& neig_vec, int neig_size);
std::vector<int> ParBubbleSort(std::vector<int>& vect);

#endif  // MODULES_TASK_1_KARIN_BUBBLE_SORT_H_
