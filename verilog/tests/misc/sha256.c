
#include "c_test.h"

#define ROTR256(X, N) ((X >> N) | (X << (32-N)))

#define CH256(x,y,z) ((x & y) ^ (~x & z))
#define MAJ256(x,y,z) ((x & y) ^ (x & z) ^ (y & z))
#define EP0256(x) (ROTR256(x,2) ^ ROTR256(x,13) ^ ROTR256(x,22))
#define EP1256(x) (ROTR256(x,6) ^ ROTR256(x,11) ^ ROTR256(x,25))
#define SIG0256(x) (ROTR256(x,7) ^ ROTR256(x,18) ^ (x >> 3))
#define SIG1256(x) (ROTR256(x,17) ^ ROTR256(x,19) ^ (x >> 10))

typedef unsigned char Hash256[32];

static const unsigned int sha_k256[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

void hashSHA256(Hash256 ret, const unsigned char* data, int size) {
    unsigned int hash[8] = {
        0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
        0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19
    };
    unsigned long long bitsize = size*8;
    for(int i = 0; i < size+9; i+=64) {
        unsigned char chunk[64];
        unsigned int w[64];
        int j;
        for(j = 0; j < 64; j++) {
            w[j] = 0;
            chunk[j] = 0;
        }
        for(j = 0; j < 64 && i+j < size; j++) {
            chunk[j] = data[i+j];
        }
        if(j < 64) {
            if(j+i == size) {
                chunk[j] = 0x80;
                j++;
            }
            if(j < 56) {
                chunk[63] = bitsize & 0xff;
                chunk[62] = (bitsize >> 8) & 0xff;
                chunk[61] = (bitsize >> 16) & 0xff;
                chunk[60] = (bitsize >> 24) & 0xff;
                chunk[59] = (bitsize >> 32) & 0xff;
                chunk[58] = (bitsize >> 40) & 0xff;
                chunk[57] = (bitsize >> 48) & 0xff;
                chunk[56] = (bitsize >> 56) & 0xff;
            }
        }
        for(j = 0; j < 16; j++) {
            w[j] = (chunk[4*j] << 24) | (chunk[4*j+1] << 16) | (chunk[4*j+2] << 8) | chunk[4*j+3];
        }
        for(; j < 64; j++) {
            w[j] = w[j-16] + SIG0256(w[j-15]) + w[j-7] + SIG1256(w[j-2]);
        }
        unsigned int a = hash[0];
        unsigned int b = hash[1];
        unsigned int c = hash[2];
        unsigned int d = hash[3];
        unsigned int e = hash[4];
        unsigned int f = hash[5];
        unsigned int g = hash[6];
        unsigned int h = hash[7];
        for(j = 0; j < 64; j++) {
            unsigned int temp1 = h + EP1256(e) + CH256(e, f, g) + sha_k256[j] + w[j];
            unsigned int temp2 = EP0256(a) + MAJ256(a, b, c);
            h = g;
            g = f;
            f = e;
            e = d + temp1;
            d = c;
            c = b;
            b = a;
            a = temp1 + temp2;
        }
        hash[0] += a;
        hash[1] += b;
        hash[2] += c;
        hash[3] += d;
        hash[4] += e;
        hash[5] += f;
        hash[6] += g;
        hash[7] += h;
    }
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 4; j++) {
            ret[4*i+j] = (hash[i] >> (24 - 8*j)) & 0xff;
        }
    }
}

void test() {
    Hash256 res;
    hashSHA256(res, (unsigned char*)"Hello world!", 12);
    ASSERT(2, res[0] == 0xc0);
    ASSERT(3, res[1] == 0x53);
    ASSERT(4, res[2] == 0x5e);
    ASSERT(5, res[3] == 0x4b);
    ASSERT(6, res[4] == 0xe2);
    ASSERT(7, res[5] == 0xb7);
    ASSERT(8, res[6] == 0x9f);
    ASSERT(9, res[7] == 0xfd);
    ASSERT(10, res[8] == 0x93);
    ASSERT(11, res[9] == 0x29);
    ASSERT(12, res[10] == 0x13);
    ASSERT(13, res[11] == 0x05);
    ASSERT(14, res[12] == 0x43);
    ASSERT(15, res[13] == 0x6b);
    ASSERT(16, res[14] == 0xf8);
    ASSERT(17, res[15] == 0x89);
    ASSERT(18, res[16] == 0x31);
    ASSERT(19, res[17] == 0x4e);
    ASSERT(20, res[18] == 0x4a);
    ASSERT(21, res[19] == 0x3f);
    ASSERT(22, res[20] == 0xae);
    ASSERT(23, res[21] == 0xc0);
    ASSERT(24, res[22] == 0x5e);
    ASSERT(25, res[23] == 0xcf);
    ASSERT(26, res[24] == 0xfc);
    ASSERT(27, res[25] == 0xbb);
    ASSERT(28, res[26] == 0x7d);
    ASSERT(29, res[27] == 0xf3);
    ASSERT(30, res[28] == 0x1a);
    ASSERT(31, res[29] == 0xd9);
    ASSERT(32, res[30] == 0xe5);
    ASSERT(33, res[31] == 0x1a);
}

