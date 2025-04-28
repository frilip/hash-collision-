#include <openssl/evp.h>
#include <openssl/md5.h>
#include <iostream>
#include <iomanip>

using std::cout, std::endl;

void print_hash(unsigned char * hash, unsigned int hash_len){
    for (unsigned int i = 0; i < hash_len; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    std::cout << std::dec << std::endl; // reset to decimal output
}



void array_copy(unsigned char * a, unsigned char * b, int b_len, int n){
    // a gets b's n least significant bits
    for (int i=0; i<n; i++)
        a[i] = b[b_len - n + i];
}


const int n = 6;

void calculate_md5(EVP_MD_CTX* ctx, unsigned char* input, int length, unsigned char * hash, unsigned int &hash_len){
    // Reinitialize the context (if reusing the same one)
    if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), nullptr)) {
        std::cerr << "DigestInit failed\n";
        return;
    }

    // Update context with input
    if (1 != EVP_DigestUpdate(ctx, input, n)) {
        std::cerr << "DigestUpdate failed\n";
        return;
    }

    // Finalize and get the hash result
    if (1 != EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        std::cerr << "DigestFinal failed\n";
        return;
    }
}


int main(){
    //unsigned char initial []= {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1};
    unsigned char hash [MD5_DIGEST_LENGTH];
    unsigned int hash_len = 0;

    EVP_MD_CTX* ctx = EVP_MD_CTX_new();

    bool condition;



    unsigned char tortoise [n] = {1,1,1,1,1,1};
    unsigned char hare [n] = {1,1,1,1,1,1};
    do {
        // tortoise = f(tortoise)
        calculate_md5(ctx, tortoise, n, hash, hash_len);
        array_copy(tortoise, hash, hash_len, n);

        // hare = f(f(hare))
        calculate_md5(ctx, hare, n, hash, hash_len);
        array_copy(hare, hash, hash_len, n);
        calculate_md5(ctx, hare, n, hash, hash_len);
        array_copy(hare, hash, hash_len, n);

        // check if hare == tortoise
        condition = true;
        for (int i=0; i<n; i++){
            if (tortoise[i] != hare[i])
                condition = false;
        }

    } while (!condition);
    
    // reset tortoise, find entry point
    unsigned char tortoise_2 [n] = {1,1,1,1,1,1};
    unsigned char tortoise_pre [n];
    unsigned char hare_pre [n];
    do{
        array_copy(tortoise_pre, tortoise_2, n, n);
        array_copy(hare_pre, hare, n, n);

        // tortoise = f(tortoise)
        calculate_md5(ctx, tortoise_2, n, hash, hash_len);
        array_copy(tortoise_2, hash, hash_len, n);

        // hare = f(hare)
        calculate_md5(ctx, hare, n, hash, hash_len);
        array_copy(hare, hash, hash_len, n);
  
        // check if hare == tortoise
        condition = true;
        for (int i=0; i<n; i++){
            if (tortoise[i] != hare[i])
                condition = false;
        }

    } while (!condition);

    print_hash(tortoise_pre, n);
    print_hash(hare_pre, n);

    cout << "-------------------" << endl;
    calculate_md5(ctx, tortoise_pre, n, hash, hash_len);
    print_hash(hash, hash_len);
    calculate_md5(ctx, hare_pre, n, hash, hash_len);
    print_hash(hash, hash_len);

    return 0;
}