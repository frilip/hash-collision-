#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdint>
#include <cstring>
#include <openssl/md5.h> // Using OpenSSL for fast MD5
#include <sys/types.h>
#include <tuple>

using std::string, std::cout, std::endl, std::tuple;


// Truncate 128-bit MD5 to last 60 bits as uint64_t
uint64_t md5_60bit(uint64_t input) {
    // Convert 60-bit input to 8-byte buffer
    unsigned char data[8];
    std::memcpy(data, &input, 8);

    // Compute MD5
    unsigned char hash[MD5_DIGEST_LENGTH]; // 16 bytes
    MD5(data, 8, hash);

    // Take last 60 bits of the MD5 digest
    uint64_t result = 0;
    for (int i = 10; i < 16; ++i) {
        result <<= 8;
        result |= hash[i];
    }
    result &= 0x0FFFFFFFFFFFFFFF; // Mask to 60 bits

    return result;
}

std::string to_hex(uint64_t val) {
    std::stringstream ss;
    ss << std::hex << std::setw(15) << std::setfill('0') << val;
    return ss.str();
}






tuple <uint64_t, uint64_t> Floyd (uint64_t initial){
    uint64_t tortoise = initial;
    uint64_t hare = initial;
    do {
        tortoise = md5_60bit(tortoise);
        hare = md5_60bit(md5_60bit(hare));
    } while (tortoise != hare);
    
    // reset tortoise, find entry point
    tortoise = initial;
    uint64_t tortoise_pre;
    uint64_t hare_pre;
    do{
        tortoise_pre = tortoise;
        hare_pre = hare;
        tortoise = md5_60bit(tortoise);
        hare = md5_60bit(hare);
    } while(tortoise != hare);
    // tortoise and hare are the entry point
    return std::make_tuple(tortoise_pre, hare_pre);
}


/*
tuple <string, string> Brent (MD5 md5, string initial, int char_amount){
    int power = 1;
    int lambda = 1;
    string tortoise = initial;
    string hare = md5_reduced(md5, initial, char_amount);
    while (tortoise != hare){
        if (power == lambda){
            tortoise = hare;
            power *= 2;
            lambda = 0;
        }
        hare = md5_reduced(md5, hare, char_amount);
        lambda++;
    }
    tortoise = initial;
    hare = initial;
    for (int i=0; i<lambda; i++){
        hare = md5_reduced(md5, hare, char_amount);
    }
    int mu = 0;
    string tortoise_pre, hare_pre;
    while (tortoise != hare){
        tortoise_pre = tortoise;
        hare_pre = hare;
        tortoise = md5_reduced(md5, tortoise, char_amount);
        hare = md5_reduced(md5, hare, char_amount);
        mu++;
    }
    return std::make_tuple(tortoise_pre, hare_pre);
}
*/







int main() {
    uint64_t initial = 1234567890123456ULL;
    
    uint64_t x,y;
    std::tie(x,y) = Floyd(initial);

    cout << to_hex(x) << endl << to_hex(y) << endl << "------------" << endl;
    cout << to_hex(md5_60bit(x)) << endl << to_hex(md5_60bit(y)) << endl;

    return 0;
}
