#include <assert.h>
#include <stdlib.h>
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
    return dict;
}


dict_t dict_add(dict_t dict, key_t word, value_t def) {
    /* Adds a [word] and its definition [def] in the dictionary, or if [word] is already
       in the dictionary, its definition is replaced by [def] */
    assert(word != NULL && def != NULL);
    
    if (dict == NULL) {
        dict = malloc(sizeof(struct _node_t));
        dict->key = word;
        dict->value = def;
        dict->left = dict_empty();
        dict->right = dict_empty();
    } 

    else if (key_eq(word, dict->key)) {
        dict->value = value_destroy(dict->value);
        dict->value = def;
    } 

    else if (key_less(word, dict->key)) {
        dict->left = dict_add(dict->left, word, def);
    } 

    else {
        dict->right = dict_add(dict->right, word, def);
    }
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}


value_t dict_search(dict_t dict, key_t word) {
    value_t def=NULL; 
    assert(word != NULL);
    if (dict != NULL){
        if (dict->key == word){
            def = dict->value;
        }
    else{
        if (key_less(dict->key,word)){
                def = dict_search(dict->left,word);
                
            } else if(key_less(dict->key,word)){
                def = dict_search(dict->right,word);
            }
        }
    }
    
    //assert((def !=NULL && !dict_exists(dict, word)) || def != NULL);
    return def;
}



bool dict_exists(dict_t dict, key_t word) {
    assert(word != NULL);
    bool exists = false;
    if (dict != NULL){
        if (dict->key == word){
            exists = true;
        }
        else {
            if (key_less(dict->key,word)){
                exists = dict_search(dict->left,word);
            } 
            else if (key_less(word,dict->key)){
                exists = dict_search(dict->right,word);
            }  
        }
    }
    
    return exists;
}


unsigned int dict_length(dict_t dict) {
    unsigned int count = 0u;
    if(dict == NULL){
        return count;
    }

    else{
        
        unsigned int count = 1;
    
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
    dict_t min = dict_empty();

    if(dict->left->key != NULL){
        min = dict_min(dict->left);
    }

    else{
        min->key = dict->key;
        min->value = dict->value;
    }
    return min;
}

dict_t dict_remove(dict_t dict, key_t word) {
    ////assert(dict != NULL && word != NULL);
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
            dict->key = key_destroy(dict->key);
            dict->value = value_destroy(dict->value);
            free(dict);
            dict = NULL;
        }
        //Case 2: Node to be deleted has 1 child
        else if (dict->left == NULL){
            dict_t temp = dict;
            dict = dict->right;
            dict->key = key_destroy(temp->key);
            dict->value = value_destroy(temp->value);
            free(temp);
            
        }
        else if (dict->right == NULL){
            dict_t temp = dict;
            dict = dict->left;
            dict->key = key_destroy(temp->key);
            dict->value = value_destroy(temp->value);
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
    //assert(!dict_exists(dict, word));
    return dict;
}


dict_t dict_remove_all(dict_t dict) {
    //assert(dict != NULL);
    if (dict == NULL){
        return dict;
    }
    if ((dict->left == NULL) && (dict->right == NULL)){
        dict = dict_remove(dict,dict->key);
    }

    else{

        dict = dict_remove_all(dict->left);
        dict = dict_remove_all(dict->right);
    }
    
    //assert(dict != NULL && dict_length(dict) == 0);
    return dict;
}


void dict_dump(dict_t dict, FILE *file) {
    assert(file != NULL);
    if (dict != NULL){
        fprintf(file, "(");
        key_dump(dict->key, file);
        fprintf(file, ": ");
        value_dump(dict->value, file);
        fprintf(file, ")\n");
        dict_dump(dict->left, file);
        dict_dump(dict->right, file);
    
    }
    
}


dict_t dict_destroy(dict_t dict) {
    ////assert(dict != NULL);
    dict = dict_remove_all(dict);
    free(dict);
    //assert(dict == NULL);
    return dict;
}