#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <cstring>

// Helper function to compute MD5 and print the result
void compute_md5(EVP_MD_CTX* ctx, const std::string& input) {
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len = 0;

    // Reinitialize the context (if reusing the same one)
    if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), nullptr)) {
        std::cerr << "DigestInit failed\n";
        return;
    }

    // Update context with input
    if (1 != EVP_DigestUpdate(ctx, input.c_str(), input.size())) {
        std::cerr << "DigestUpdate failed\n";
        return;
    }

    // Finalize and get the hash result
    if (1 != EVP_DigestFinal_ex(ctx, hash, &hash_len)) {
        std::cerr << "DigestFinal failed\n";
        return;
    }

    // Print the resulting hash in hex format
    std::cout << "MD5(\"" << input << "\") = ";
    for (unsigned int i = 0; i < hash_len; ++i)
        std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    std::cout << std::dec << std::endl; // reset to decimal output
}



int main() {
    // Create the context only once
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) {
        std::cerr << "Failed to create context\n";
        return 1;
    }

    // Compute MD5 for multiple strings
    compute_md5(ctx, "hello");
    compute_md5(ctx, "world");

    // Clean up
    EVP_MD_CTX_free(ctx);

    return 0;
}
