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
    dict_t dict = malloc(sizeof(struct _node_t));
    dict ->key = NULL;
    dict -> value = NULL;
    assert(dict != NULL && dict_length(dict) == 0);
    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(dict != NULL && word != NULL && def != NULL);
    if (key_eq(dict->key,word))
    {
        dict->value=def;
    }
    else if (key_less(dict->key,word))
    {
        dict=dict_add(dict->left,word,def);
    }
    else if(key_less(word,dict->key))
    {
        dict=dict_add(dict->right,word,def);
    }   
    else
    {
        dict->key=word;
        dict->value=def;
        dict->left=dict_empty();
        dict->right=dict_empty();
    }
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    key_t def=NULL;
    assert(dict != NULL && word != NULL);
    if (string_eq(dict->key,word)){
        def = dict->value;
    }else
    {
        if (string_less(dict->key,word))
        {
            def = dict_search(dict->left,word);
        } else if(string_less(word,dict->key))
        {
            def = dict_search(dict->right,word);
        }
        
        
    }
    assert((def==NULL && !dict_exists(dict, word)) || def != NULL);
    return def;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    bool exists = false;
    if(string_eq(dict->key,word)){
        exists = true;
    }
    else{
        if (string_less(dict->key,word))
        {
            exists = dict_exists(dict->left, word);
        }
        else if (string_less(word,dict->key))
        {
            exists = dict_exists(dict->right, word);
        }
        
        
    }
    return exists;
}

unsigned int dict_length(dict_t dict) {
    assert(dict != NULL);
    unsigned int length = 0u;
    if ((dict->left->key==NULL)||(dict->right->key==NULL))
    {
        length++;
    }
    else{
        length += (dict_length(dict->left)+dict_length(dict->right));
    }
    return length;
}


dict_t dict_remove(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    if (string_less(dict->key,word)){

        dict->key = dict_remove(dict->left,word);

    } else if (string_less(word,dict->right)){

        dict->key = dict_remove(dict->right,word);

    } else{

        if (dict->left->key==NULL){
            dict_t daux = dict->right;
            string_destroy(dict->key);
            string_destroy(dict->value);
            free(dict);
            dict = daux;

        }else if(dict->right->key==NULL){
            dict_t daux = dict->left;
            string_destroy(dict->key);
            string_destroy(dict->value);
            free(dict);
            dict = daux; //no se si es asi, deberia ser un return daux
        }
        else{
            dict_t daux = dict_min_node(dict->right);
            dict->key = daux->key;
            dict->right = dict_remove(dict->right,daux->key);
        }
    }
    assert(dict != NULL && !dict_exists(dict, word));
    return dict;
} // no preguntes está horrible tengo miedo :(
dict_t dict_min_node(dict_t dict){
    dict_t min;
    if(dict->right->key!=NULL){
    min = disc_min_node(dict->right);
    }
    else{
    min->key = dict->key;
    min->value = dict->value;
    }
    return min;
}
dict_t dict_remove_all(dict_t dict) {
    assert(dict != NULL);
    if ((dict->left->key==NULL)&&(dict->right->key==NULL))
    {
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
    dict = dict_from_file(file);
    string_dump(dict->key,file);
    string_dump(dict->value,file);
    dict_dump(dict->left,file);
    dict_dump(dict->right,file);
    assert(dict != NULL);
}

dict_t dict_destroy(dict_t dict) {
    assert(dict != NULL);
    dict=dict_remove_all;
    free(dict);
    assert(dict == NULL);
    return dict;
}
