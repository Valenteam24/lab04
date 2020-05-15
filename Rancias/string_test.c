#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dict.h"
#include "dict_helpers.h"
#include "helpers.h"
#include "string.h"

int main(void){
    string_t s1 = string_create("-1");
    string_t s2 = string_create("1");
    bool b =  string_eq(s1,s2);
    bool r = string_less(s1,s2);
    printf("b: %s\n\n",(b ? "True" : "False"));
    printf("r: %s\n\n",(r ? "True" : "False"));
    string_destroy(s1);
    string_destroy(s2);
    return 0;
}