#include <stdlib.h> //include the standard C libs
#include <stdio.h>
#include <string.h>

void printByteArray(unsigned char bytes[], size_t length); // you need to predeclare your methods
unsigned char* xor(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* shift(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* unshift(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* expandKey(unsigned char key[], size_t key_length);
unsigned int modPow(int base, int exp, int mod);
unsigned char* encrypt(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);
unsigned char* decrypt(unsigned char data[], size_t data_length, unsigned char key[], size_t key_length);

int main(int argc, unsigned char* argv[]) { // [CMD] [OPTION] [KEY] [FILE] [NEWFILE]
    if (argv == NULL || argc < 5) {
        printf("Not enough arguments were given, aborting operation.\n");
        return 1;
    }
    if (strcmp((char*)argv[1], "--e") != 0 && strcmp((char*)argv[1], "--d") != 0) {
        printf("Option %s not recognized.\n", argv[1]);
        return 1;
    }

    size_t key_size = strlen((char*)argv[2]);
    FILE* f = fopen((char*)argv[3], "rb");
    if (f == NULL) {
        printf("Failed to open file '%s'.\n", argv[3]);
        return 1;
    }

    FILE* nf = fopen((char*)argv[4], "wb");
    if (nf == NULL) {
        printf("Failed to create new file '%s'.\n", argv[4]);
        fclose(f);
        return 1;
    }

    unsigned char b[512]; // block size of 512 bytes
    fseek(f, 0, SEEK_END);
    long filesize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (filesize < sizeof(b) && filesize > 0) { // check if the file is smaller than 512 bytes (1 block)
        printf("File is under 512 bytes, using dynamic block size.\n");
        unsigned char bb[filesize];
        fread(bb, 1, filesize, f);
        unsigned char* e = strcmp((char*)argv[1], "--e") == 0
            ? encrypt(bb, filesize, argv[2], key_size)
            : decrypt(bb, filesize, argv[2], key_size);
        if (e == NULL) {
            printf("Encryption/Decryption failed.\n");
            fclose(f);
            fclose(nf);
            return 1;
        }
        if (fwrite(e, filesize, 1, nf) < 1) {
            printf("Failed to write to file, aborting.\n");
            free(e);
            fclose(f);
            fclose(nf);
            return 1;
        }
        free(e);
    } else {
        size_t blocks = (filesize + sizeof(b) - 1) / sizeof(b); // total number of blocks, including a potential partial block
        for (size_t i = 0; i < blocks; i++) {
            size_t block_size = (i == blocks - 1 && filesize % sizeof(b) != 0) ? (filesize % sizeof(b)) : sizeof(b);
            fread(b, 1, block_size, f);
            unsigned char* e = strcmp((char*)argv[1], "--e") == 0
                ? encrypt(b, block_size, argv[2], key_size)
                : decrypt(b, block_size, argv[2], key_size);
            if (e == NULL) {
                printf("Encryption/Decryption failed.\n");
                fclose(f);
                fclose(nf);
                return 1;
            }
            if (fwrite(e, block_size, 1, nf) < 1) {
                printf("Failed to write to file, aborting.\n");
                free(e);
                fclose(f);
                fclose(nf);
                return 1;
            }
            free(e);
        }
    }
    printf("Processing done. Saved cipherfile as '%s'.\n", argv[4]);
    fclose(nf);
    fclose(f);
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