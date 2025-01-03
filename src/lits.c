#include <stdlib.h> //include the standard C libs
#include <stdio.h>

void printByteArray(unsigned char bytes[], size_t length); // you need to predeclare your methods
unsigned char* xor(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* shift(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* unshift(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* expandKey(unsigned char key[], size_t key_length);
unsigned int modPow(int base, int exp, int mod);
unsigned char* encrypt(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* decrypt(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);

int main() {
    unsigned char data[] = {
        0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF, 0x00, 0x11, 
        0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 
        0x00, 0xFF, 0xAA, 0xBB, 0xCC, 0xDD, 0xEE, 0xFF
    };
    unsigned char key[] = {0xAF, 0xFF, 0xAA, 0xEF, 0xFF, 0xBB, 0xAB, 0xEA};

    size_t data_length = sizeof(data); // we need to get the length because sizeof() is only valid at compile time

    /* unsigned char* out = xor(data, data_length, key, sizeof(key));
    unsigned char* outout = xor(out, sizeof(out), key, sizeof(key));
    unsigned char* out2 = shift(data, data_length, key, sizeof(key));
    unsigned char* outout2 = unshift(out2, data_length, key, sizeof(key));

    printByteArray(data, data_length);
    printByteArray(out2, data_length);
    printByteArray(outout2, data_length);

    unsigned char* expandedKey = expandKey(key, sizeof(key));
    printByteArray(key, sizeof(key));
    printByteArray(expandedKey, sizeof(key) * 2);

    free(expandedKey);
    free(out2);
    free(outout2);
    free(outout);
    free(out); // since we used malloc to allocate memory in the heap, we need to free that memory again to the OS */

    unsigned char* encrypted = encrypt(data, data_length, key, sizeof(key));
    unsigned char* decrypted = decrypt(encrypted, data_length, key, sizeof(key));
    printByteArray(data, data_length);
    printByteArray(encrypted, data_length);
    printByteArray(decrypted, data_length);

    free(encrypted);
    free(decrypted);
    return 0;
}

unsigned char* encrypt(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length) {
    unsigned char* ekey = expandKey(key, key_length);
    unsigned char* xored = xor(data, data_length, ekey, key_length * 2);
    unsigned char* shifted = shift(xored, data_length, ekey, key_length * 2);
    return shifted;
}

unsigned char* decrypt(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length) {
    unsigned char* ekey = expandKey(key, key_length);
    unsigned char* shifted = unshift(data, data_length, ekey, key_length * 2);
    unsigned char* xored = xor(shifted, data_length, ekey, key_length * 2);
    return xored;
}

unsigned char* shift(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length) {
    unsigned char* out = malloc(data_length * sizeof(unsigned char));
    if(out == NULL) {
        return NULL;
    }
    for(int i = 0; i < data_length; i++) {
        out[i] = data[i];
    }
    for(int x = 0; x < data_length; x++) {
        int newshiftidx = (int) x % key_length;
        int newshift = out[newshiftidx];
        int origin = (int) out[x];
        out[newshiftidx] = (char) origin;
        out[x] = (char) newshift;
    }
    return out;
}

unsigned char* unshift(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length) {
    unsigned char* out = malloc(data_length * sizeof(unsigned char));
    if(out == NULL) {
        return NULL;
    }
    for(int i = 0; i < data_length; i++) {
        out[i] = data[i];
    }
    for(int x = data_length; x-- > 0 ;) {
        int newshiftidx = (int) x % key_length;
        int newshift = out[newshiftidx];
        int origin = (int) out[x];
        out[newshiftidx] = (char) origin;
        out[x] = (char) newshift;
    }
    return out;
}

unsigned char* xor(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length) {
    unsigned char* out = malloc(data_length * sizeof(unsigned char));
    if(out == NULL) {
        return NULL;
    }
    for(int x = 0; x < data_length; x++) {
        out[x] = data[x] ^ key[x % key_length]; // xor the byte of data with the repeated byte of the key
    }
    return out;
}

unsigned char* expandKey(unsigned char key[], size_t key_length) {
    char* newkey = malloc((key_length*2) * sizeof(unsigned char));
    if(newkey == NULL) {
        return NULL;
    }
    for(int x = 0; x < key_length; x++) {
        newkey[x * 2] = key[x];
        int val = (int) key[x];
        int point = (int) key[val % key_length];
        int bound = (int) key[point % key_length];
        if(bound == 0) bound = 1;
        if(point == 0) point = 1;
        int tmp = point;
        for(int o = x; o != point; o += (x < point ? 1 : -1)) {
            int pox = val ^ (key[o % key_length] ^ bound);
            tmp ^= pox;
        }
        newkey[x * 2 + 1] = (char) modPow(tmp, point, bound);
    }
    return newkey;
}

unsigned int modPow(int base, int exp, int mod) {
    int result = 1;
    while(exp > 0) {
        if((exp & 1) == 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}

/**
 * print an byte array
 * @param bytes byte array to be printed
 * @param length length of the byte array
 */
void printByteArray(unsigned char bytes[], size_t length) {
    for (size_t i = 0; i < length; i++) {
        printf("%02X ", bytes[i]);
    }
    printf("\n");
}