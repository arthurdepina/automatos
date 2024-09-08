#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _INTEGER_                   "<INTEIRO>"
#define _SIGNED_INTEGER_            "<INTEIRO COM SINAL>"
#define _FLOATING_POINT_            "<P. FLUTUANTE>"
#define _SIGNED_FLOATING_POINT_     "<P. FLUTUANTE COM SINAL>"
#define _ERROR_                     "<ERRO>"


char** split_words(const char* str, int* word_count)
{
    char* tmp_str = strdup(str);
    char* token;
    char** words = NULL;
    int count = 0;

    token = strtok(tmp_str, " ");
    while (token != NULL) {
        words = realloc(words, sizeof(char*) * (count + 1));
        words[count] = strdup(token);
        count++;
        token = strtok(NULL, " ");
    }

    *word_count = count;
    free(tmp_str);
    return words;
}


char* scanner(const char* word)
{
    char* result;
    int current = 0;
    int length = strlen(word);

    qO:
        if (word[current] >= '0' && word[current] <= '9') {
            goto q_integer;
        } else if (word[current] == ',') {
            goto error_float;
        } else if (word[current] == '+' || word[current] == '-') {
            goto error_signal;
        } else {
            return _ERROR_;
        }

    q_integer:
        current += 1;
        if (current == length) {
            return _INTEGER_;
        } else if (word[current] >= '0' && word[current] <= '9') {
            goto q_integer;
        } else if (word[current] == ',') {
            goto error_float;
        } else {
            return _ERROR_;
        }

    error_float:
        current += 1;
        if (current == length) {
            return _ERROR_;
        } else if (word[current] >= '0' && word[current] <= '9') {
            goto q_float;
        } else {
            return _ERROR_;
        }

    q_float:
        current += 1;
        if (current == length) {
            return _FLOATING_POINT_;
        } else if (word[current] >= '0' && word[current] <= '9') {
            goto q_float;
        } else {
            return _ERROR_;
        }

    error_signal:
        current += 1;
        if (current == length) {
            return _ERROR_;
        } else if (word[current] == '0') {
            goto error_signed_zero;
        } else if (word[current] >= '1' && word[current] <= '9') {
            goto q_signed_integer;
        } else if (word[current] == ',') {
            goto error_signed_float;
        } else {
            return _ERROR_;
        }

    error_signed_zero:
        current += 1;
        if (current == length) {
            return _ERROR_;
        } else if (word[current] == '0') {
            goto error_signed_zero;
        } else if (word[current] >= '1' && word[current] <= '9') {
            goto q_signed_integer;
        } else {
            return _ERROR_;
        }

    q_signed_integer:
        current += 1;
        if (current == length) {
            return _SIGNED_INTEGER_;
        } else if (word[current] >= '0' && word[current] <= '9') {
            goto q_signed_integer;
        } else {
            return _ERROR_;
        }

    error_signed_float:
        current += 1;
        if (current == length) {
            return _ERROR_;
        } else if (word[current] == '0') {
            goto error_signed_float_zero;
        } else if (word[current] >= '1' && word[current] <= '9') {
            goto q_signed_float;
        } else {
            return _ERROR_;
        }
    
    error_signed_float_zero:
        current += 1;
        if (current == length) {
            return _ERROR_;
        } else if (word[current] == '0') {
            goto error_signed_float_zero;
        } else if (word[current] >= '1' && word[current] <= '9') {
            goto q_signed_float;
        } else {
            return _ERROR_;
        }

    q_signed_float:
        current += 1;
        if (current == length) {
            return _SIGNED_FLOATING_POINT_;
        } else if (word[current] >= '0' && word[current] <= '9') {
            goto q_signed_float;
        } else {
            return _ERROR_;
        }

    return _ERROR_;
}


int main ()
{
    const char* inputs = "21 +4+5,6 00,123 +,456 -42. xx -123 +0 +,0 -,00";
    int word_count;
    char** words = split_words(inputs, &word_count);

    for (int i = 0; i < word_count; i++) {
        printf("%d: %s ", i + 1, words[i]);
        printf("%s\n", scanner(words[i]));
        free(words[i]);
    }

    free(words);

    return 0;
}
