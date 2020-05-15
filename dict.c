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
        } else { 
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
    dict_t min = malloc(sizeof(struct _node_t));
    if(dict->left!=NULL){
        min = dict_min_node(dict->left);
    }
    else{
        min->key = string_clone(dict->key);
        min->value = string_clone(dict->value);
    }
    return min;
}
dict_t dict_remove(dict_t dict, key_t word) {
    //assert(dict != NULL && word != NULL);
    if (key_less(dict->key,word)){

        dict->right = dict_remove(dict->right,word);   //Keep cycling through the right branch until we arrive

    }else if(key_less(word,dict->key)){ 

        dict->left = dict_remove(dict->left,word);     //Keep cycling through the left branch until we arrive

    } else { //If the key isnt 
        // no children
        if ((dict->left==NULL)&&(dict->right==NULL)){
            dict->key = key_destroy(dict->key);       //Free the memory for the old key
            dict->value = value_destroy(dict->value); //Free the memory for the old value
            free(dict);                               //Frees the old dict
            dict = NULL;
        }
        // one right child 
          else if (dict->left==NULL){
            dict_t daux = dict;                 //Create a new pointer in memory
            dict = dict->right;                 //Point the dict to its left branch
            daux->key = key_destroy(daux->key); //Free the memory for the old key
            daux->value = value_destroy(daux->value); //Free the memory for the old value
            free(daux);                               //Frees the old dict
        } 
        // one left child
          else if(dict->right==NULL){
            dict_t daux = dict;                 //Create a new pointer in memory
            dict = dict->left;                  //Point the dict to its left branch
            daux->key = key_destroy(daux->key); //Free the memory for the old key
            daux->value = value_destroy(daux->value); //Free the memory for the old value
            free(daux);                               //Frees the old dict
        }
        // two children;
          else {
            dict_t daux = dict_min_node(dict->right);   //create a new dict containing the min node on the right branch
            dict->key = key_destroy(dict->key);         //free the memory for the old key
            dict->value = value_destroy(dict->value);   // free the memory for the old value
            dict->key = daux->key;                      //point to new key in memory
            dict->value = daux->value;                  //point to new value in memory
            dict->right = dict_remove(dict->right,daux->key); //remove the duplicate node
            free(daux);                                       //free just the dict
        }
    }
    assert(!dict_exists(dict, word));
    return dict;
} 


dict_t dict_remove_all(dict_t dict) {
    if (dict !=NULL){
        dict->key = key_destroy(dict->key);      //Free the memory for the actual key
        dict->value = value_destroy(dict->value); //Free the memory for the actual value
        dict->left = dict_remove_all(dict->left); //Remove everything from the left branch
        dict->right = dict_remove_all(dict->right);//Remove everything from the right branch
        free(dict); //Free the dict itself
        dict = NULL;
    }  
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(file != NULL);
    if (dict != NULL) {
        key_dump(dict->key, file); //Write the key to the file
        fprintf(file, ": ");  //Separator
        value_dump(dict->value, file); //Write the value to the file
        fprintf(file, "\n"); //Insert new line
        dict_dump(dict->left, file); //Write the left branch to file
        dict_dump(dict->right, file); //Write the right branch to file
    }
 
}
dict_t dict_destroy(dict_t dict) {
    if (dict !=NULL){
        dict->key = key_destroy(dict->key); //Free the memory of the key
        dict->value = value_destroy(dict->value); //Free the memory of the value
        dict->left=dict_destroy(dict->left);      //destroy the left branch
        dict->right=dict_destroy(dict->right);    //destroy the right branch
        free(dict);                               //free the dict
    }
    return NULL;
}
