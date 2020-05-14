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
        return NULL;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(word != NULL && def != NULL);
    if(dict==NULL){
        dict_t new_dict = malloc(sizeof(struct _node_t));
        new_dict->key=word;
        new_dict->value=def;
        new_dict->left=dict_empty();
        new_dict->right=dict_empty();
        dict = new_dict;
    }
    else if(key_less(dict->key,word)){
        dict->right = dict_add(dict->right,word,def);
    }
    else if(key_less(word,dict->key)){
        dict->left = dict_add(dict->left,word,def);
    }   
    else{ 
        /*if the key entered is not minor, is not greater
        and is not null, then it is equal*/
        dict->value = value_destroy(dict->value);
        dict->value=def;
    }
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    assert(word != NULL);
    value_t def = NULL; 
   
    if (dict !=NULL ){ 
        if (key_eq(dict->key,word)){
            def = dict->value;
        }else if (key_less(word,dict->key)){
            def = dict_search(dict->left,word);
        } else { //key_less(dict->key,word)
            def = dict_search(dict->right,word);
            }
        } 
       // assert((def !=NULL && !dict_exists(dict, word)) || def != NULL);
    return def;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(word != NULL);
    bool exists = false;
    if (dict != NULL){
        exists = key_eq(dict->key,word) || dict_exists(dict->left,word) || dict_exists(dict->right,word);
    }
    return exists;
}

unsigned int dict_length(dict_t dict) {
    //assert(dict != NULL);
    unsigned int length = 0u;
    if(dict != NULL){
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

// Returns the minimum node of the right sub-tree.
static dict_t dict_min_node(dict_t dict){
    dict_t min = dict_empty();
    if(dict->left->key!=NULL){
        min = dict_min_node(dict->left);
    }
    else{
        min->key = dict->key;
        min->value = dict->value;
    }
    return min;
}

dict_t dict_remove(dict_t dict, key_t word) {
    //assert(dict != NULL && word != NULL);
    if (key_less(dict->key,word)){

        dict = dict_remove(dict->left,word);

    }else if(key_less(word,dict->key)){

        dict = dict_remove(dict->right,word);

    } else {
        // no children
        if ((dict->left==NULL)&&(dict->right==NULL)){
            dict->key = key_destroy(dict->key);
            dict->value = value_destroy(dict->value);
            free(dict);
            dict = NULL;
        }
        // one right child 
          else if ((dict->left->key==NULL)&&(dict->left->value==NULL)){
            dict_t daux = dict;
            dict = dict->right;
            daux->key = key_destroy(daux->key);
            daux->value = value_destroy(daux->value);
            free(daux);
        } 
        // one left child
          else if((dict->right->key==NULL)&&(dict->right->value==NULL)){
            dict_t daux = dict;
            dict = dict->left;
            daux->key = key_destroy(daux->key);
            daux->value = value_destroy(daux->value);
            free(daux);
        }
        // two children
          else {
            dict_t daux = dict_min_node(dict->right);
            dict->key = daux->key;
            dict->value = daux->value;
            dict->right = dict_remove(dict->right,daux->key);
        }
    }
    assert(!dict_exists(dict, word));
    return dict;
} 


dict_t dict_remove_all(dict_t dict) {
    if (dict !=NULL){
        dict=dict_remove_all(dict->left);
        dict=dict_remove_all(dict->right);
        free(dict);
        dict = NULL;
    }  
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(file != NULL);
    if (dict != NULL) {
        key_dump(dict->key, file);
        fprintf(file, ": ");
        value_dump(dict->value, file);
        dict_dump(dict->left, file);
        dict_dump(dict->right, file);
    }
    //assert(dict != NULL);
}

dict_t dict_destroy(dict_t dict) {
    if (dict !=NULL){
        dict=dict_remove_all(dict->left);
        dict=dict_remove_all(dict->right);
        free(dict);
    }
    assert(dict == NULL);
    return dict;
}
