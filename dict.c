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
    dict->key = dict->value = NULL;
    assert(dict != NULL && dict_length(dict) == 0);

    return dict;
}

dict_t dict_add(dict_t dict, key_t word, value_t def) {
    assert(dict != NULL && word != NULL && def != NULL);
    /* needs implementation */
    assert(value_eq(def, dict_search(dict, word)));
    return dict;
}

value_t dict_search(dict_t dict, key_t word) {
    key_t def=NULL;
    assert(dict != NULL && word != NULL);
    if (dict->key == word){
        def = dict->value;
    }else
    {
        if (string_less(dict->key,word)){

            def = dict_search(dict->left,word);

        } else if(string_less(word,dict->key)){

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
        if (string_less(dict->key,word)){
            exists = dict_search(dict->left,word);
        } else if (string_less(word,dict->key)){
            exists = dict_search(word,dict->right);
        }  
    }
    return exists;
}

unsigned int dict_length(dict_t dict) {
    assert(dict != NULL);
    if(!(dict->key == NULL)){
        unsigned int count = 1;
        if (dict->left != NULL) {
        count += dict_length(dict->left);
        }
        if (dict->right != NULL) {
            count += dict_length(dict->right);
        }
        return count;
    }
}

dict_t dict_remove(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    if (string_less(dict->key,word)){

        dict->key = dict_remove(dict->left,word);

    } else if (string_less(word,dict->right)){

        dict->key = dict_remove(dict->right,word);

    } else if(string_eq(dict->key,word)){ //capaz se puede poner solo else

        if ((dict->left->key==NULL)&&(dict->left->value==NULL)){
            dict_t daux = dict->right;
            free(dict);
            dict = daux; //lo mesmo que abajo

        }else if((dict->right->key==NULL)&&(dict->right->value==NULL)){
            dict_t daux = dict->left;
            free(dict);
            dict = daux; //no se si es asi, deberia ser un return daux
        }
        dict_t daux = dict_min_node(dict->right);
        dict->key = daux->key;
        dict->right = dict_remove(dict->right,daux->key);
    }
    assert(dict != NULL && !dict_exists(dict, word));
    return dict;
} // no preguntes está horrible tengo miedo :(

static dict_t dict_min_node(dict_t dict){
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
    dict = dict_from_file(file);

    string_dump(dict->key,file);
    string_dump(dict->value,file);
    dict_dump(dict->left,file);
    dict_dump(dict->right,file);
    assert(dict != NULL);
}

dict_t dict_destroy(dict_t dict) {
    assert(dict != NULL);
    /* needs implementation */
    assert(dict == NULL);
    return dict;
}

