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
    /* needs implementation */
    assert(dict != NULL && !dict_exists(dict, word));
    return dict;
}

dict_t dict_remove_all(dict_t dict) {
    assert(dict != NULL);
    /* needs implementation */
    assert(dict != NULL && dict_length(dict) == 0);
    return dict;
}

void dict_dump(dict_t dict, FILE *file) {
    assert(dict != NULL && file != NULL);
    dict = dict_from_file(file);

    string_dump(dict->key,file);
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

