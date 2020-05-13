#include <assert.h>
#include "dict.h"
#include "key_value.h"
#include <stdlib.h>
#include <stdbool.h>

struct _node_t {
    dict_t left;
    dict_t right;
    key_t key;
    value_t value;
};

dict_t dict_empty() {
    dict_t dict = NULL;
    dict = (dict_t)malloc(sizeof(struct _node_t));
    dict->key = dict->value = NULL;
    assert(dict != NULL && dict_length(dict) == 0);

    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(dict != NULL && word != NULL && def != NULL);
    if (key_eq(dict->key,word)){
        dict->value = value_destroy(dict->value);
        dict->value=def;
    }
    else if (key_less(dict->key,word)){
        dict=dict_add(dict->left,word,def);
    }
    else if(key_less(word,dict->key)){
        dict=dict_add(dict->right,word,def);
    }   
    else{ 
        /*if the key entered is not minor, is not greater
        and is not equal, then it is null (and value too)*/
        dict->key=word;
        dict->value=def;
        dict->left=dict_empty();
        dict->right=dict_empty();
    }
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    value_t def=NULL; 
    assert(dict != NULL && word != NULL);
    if (dict->key == word){
        def = dict->value;
    }else
    {
        if (key_less(dict->key,word)){

            def = dict_search(dict->left,word);

        } else if(key_less(dict->key,word)){

            def = dict_search(dict->right,word);
        }
    }
    assert((def==NULL && !dict_exists(dict, word)) || def != NULL);
    return def;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    bool exists = false;
    if (dict->key == word){
        exists = true;
    }else {
        if (key_less(dict->key,word)){
            exists = dict_search(dict->left,word);
        } else if (key_less(word,dict->key)){
            exists = dict_search(dict->right,word);
        }  
    }
    return exists;
}

unsigned int dict_length(dict_t dict) {
    assert(dict != NULL);
    unsigned int length = 0u;
    if(dict->key != NULL){
        length = 1;
        if (dict->left != NULL) {
        length += dict_length(dict->left);
        }
        if (dict->right != NULL) {
            length += dict_length(dict->right);
        }
    }
    return length;
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    if (key_less(dict->key,word)){

        dict = dict_remove(dict->left,word);

    }else if(key_less(word,dict->key)){

        dict = dict_remove(dict->right,word);

    } else {

        if ((dict->left->key==NULL)&&(dict->left->value==NULL)){
            dict_t daux = dict->right;
            key_destroy(dict->key);
            value_destroy(dict->value);
            free(dict);
            dict = daux; //???

        }else if((dict->right->key==NULL)&&(dict->right->value==NULL)){
            dict_t daux = dict->left;
            key_destroy(dict->key);
            value_destroy(dict->value);
            free(dict);
            dict = daux; //????
        } else {
            dict_t daux = dict_min_node(dict->right);
            dict->key = daux->key;
            dict->right = dict_remove(dict->right,daux->key);
        }
    }
    assert(dict != NULL && !dict_exists(dict, word));
    return dict;
} 

dict_t dict_min_node(dict_t dict){
    dict_t min = dict_empty();
    if(dict->right->key!=NULL){
        min = dict_min_node(dict->right);
    }
    else{
        min->key = dict->key;
        min->value = dict->value;
    }
    return min; //deberia retornar un nodo pero idk
}

dict_t dict_remove_all(dict_t dict) {
    assert(dict != NULL);
    if ((dict->left->key==NULL)&&(dict->right->key==NULL)){
        dict=dict_remove(dict,dict->key);
    }
    else{
        dict=dict_remove_all(dict->left);
        dict=dict_remove_all(dict->right);
    }
    assert(dict != NULL && dict_length(dict) == 0);
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(dict != NULL && file != NULL);
    if (dict->key != NULL) {
        fprintf(file, "(");
        key_dump(dict->key, file);
        fprintf(file, ",");
        value_dump(dict->value, file);
        fprintf(file, ")\n");
        dict_dump(dict->left, file);
        dict_dump(dict->right, file);
    }
    assert(dict != NULL);
}

dict_t dict_destroy(dict_t dict) {
    assert(dict != NULL);
    dict=dict_remove_all(dict);
    free(dict);
    assert(dict == NULL);
    return dict;
}

