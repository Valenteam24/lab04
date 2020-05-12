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
        if (dict->key < word)
        {
            def = dict_search(dict->left,word);
        } else if(dict->key > word)
        {
            def = dict_search(dict->right,word);
        }
        
        
    }
    
    
    assert((def==NULL && !dict_exists(dict, word)) || def != NULL);
    return NULL;
}

bool dict_exists(dict_t dict, key_t word) {
    assert(dict != NULL && word != NULL);
    /* needs implementation */
    return false;
}

unsigned int dict_length(dict_t dict) {
    assert(dict != NULL);
    /* needs implementation */
    return 0u;
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
    /* needs implementation */
    assert(dict != NULL);
}

dict_t dict_destroy(dict_t dict) {
    assert(dict != NULL);
    /* needs implementation */
    assert(dict == NULL);
    return dict;
}

