#include <assert.h>
#include "dict.h"
#include "key_value.h"


struct _node_t {
    dict_t left;
    dict_t right;
    key_t key;
    value_t value;
};


dict_t dict_empty() {
    dict_t dict = NULL;
    dict = (dict_t)malloc(sizeof(struct _node_t));
    dict->key = NULL;
    dict->value = NULL;
    assert(dict_length(dict) == 0);
    return dict;
}

//To do
dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(word != NULL && def != NULL);
    
    if ((dict->key == NULL) && (dict->value == NULL)){
        dict->key = word;
        dict->value=def;
        dict->left = dict_empty();
        dict->right = dict_empty();
    }
    else if (key_less(dict->key,word)){
        dict = dict_add(dict->right,word,def);
    }
    else if(key_less(word,dict->key)){
        dict = dict_add(dict->left,word,def);
    }   
    else{ 
        /*if the key entered is not minor, is not greater
        and is not null, then it is equal*/
        value_destroy(dict->value);
        dict->value=def;
    }
    assert(dict != NULL && value_eq(def, dict_search(dict, word)));
    return dict;
}


value_t dict_search(dict_t dict, key_t word) {

    key_t def=NULL;
    assert(dict != NULL && word != NULL);
    
    if (key_less(dict->key,word)) {
        def = dict_search(dict->right,word);
    }
    
    else {
        def = dict_search(dict->left,word);
    }
    
    assert((def==NULL && !dict_exists(dict, word)) || def != NULL);

    return def;
}


bool dict_exists(dict_t dict, key_t word) {

    assert(dict != NULL && word != NULL);
    bool res = false;
    
    if (key_eq(dict->key,word)) {
        res = true;
    }

    else{

        if(key_less(dict->key,word)){
            res = dict_exists(dict->right,word);
        }

        else {
            res = dict_exists(dict->left,word);
        }
    }

    return res;
}


unsigned int dict_length(dict_t dict) {

    assert(dict != NULL);

    unsigned int count = 1;

    if(dict->key != NULL){

        if (dict->left != NULL){
            count += dict_length(dict->left);
        }
        
        if (dict->right != NULL){
            count += dict_length(dict->right);
        }
    }

    return count;
}


//Auxiliary function for remove
dict_t dict_min(dict_t dict){
    dict_t min = dict;
    while (min->left != NULL){
        min = min->left;
    }
    return min;
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    //Search node to delete

    if(key_less(word,dict->key)){
        dict->left =  dict_remove(dict->left,word);
    }

    else if (key_less(dict->key,word)){
        dict->right = dict_remove(dict->right,word);
    }
    
    else{
        //If word not greater or lesser, then it's equal
        //Case 1: Node to be deleted is leaf (has no child)
        if (dict->left == NULL && dict->right == NULL){
            key_destroy(dict->key);
            value_destroy(dict->value);
            free(dict);
            dict = NULL;
        }
        //Case 2: Node to be deleted has 1 child
        else if (dict->left == NULL){
            dict_t temp = dict;
            dict = dict->right;
            key_destroy(temp->key);
            value_destroy(temp->value);
            free(temp);
            
        }
        else if (dict->right == NULL){
            dict_t temp = dict;
            dict = dict->left;
            key_destroy(temp->key);
            value_destroy(temp->value);
            free(temp);
            
        }
        //Case 3: Node to be deleted has 2 children
        else{
            dict_t temp = dict_min(dict->right);
            dict->key = temp->key;
            dict->value = temp->value;
            dict->right = dict_remove(dict->right,temp->key);
        }
        
    }
    assert(!dict_exists(dict, word));
    return dict;
}


dict_t dict_remove_all(dict_t dict) {
    assert(dict != NULL);

    if ((dict->left->key == NULL) && (dict->right->key == NULL)){
        dict = dict_remove(dict,dict->key);
    }

    else{

        dict = dict_remove_all(dict->left);
        dict = dict_remove_all(dict->right);
    }
    
    assert(dict != NULL && dict_length(dict) == 0);
    return dict;
}

//To do
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
    dict = dict_remove_all(dict);
    free(dict);
    assert(dict == NULL);
    return dict;
}