#ifndef COMPILATION_03_UTILLS_HPP
#define COMPILATION_03_UTILLS_HPP

#include <cstring>
#include <iostream>
#include <cstdlib>
#include <vector>

int string_to_num(const char* input);
//int string_to_num(std::string input);

char* remove_double_quotes(char* input);

std::string num_to_string(int input);

std::vector<int> mergeVectors(const std::vector<int> &l1,const std::vector<int> &l2);
#endif //COMPILATION_03_UTILLS_HPP