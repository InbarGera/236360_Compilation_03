#include "utills.hpp"

int string_to_num(char* input){
    int sum =0;
    while(input[0] != '\0'){
        sum = sum*10 + (input++[0] - '0');
    }
    return sum;
}

char* remove_double_quotes(char* input){
    char* res = (char*)malloc(strlen(input)* sizeof(char));
    strcpy(res,input+1);
    res[strlen(res) - 1] = '\0';
    return res;
}

std::string num_to_string(int input){
    if(input == 0)
        return "0";
    std::string res = "";

    int mod = 1;
    while(input/mod)
        mod *= 10;
    mod /= 10;

    while(mod){
        res += (input % (mod*10)) /mod + '0';
        mod /= 10;
    }
    return res;
}
