#include <openssl/evp.h>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <sstream>
#include <vector>
#include <cstring>
#include <openssl/md5.h>

// Compute full MD5 hash using EVP
std::vector<unsigned char> md5_hash(const std::string& input) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;

    EVP_DigestInit_ex(ctx, EVP_md5(), nullptr);
    EVP_DigestUpdate(ctx, input.data(), input.size());
    EVP_DigestFinal_ex(ctx, hash, &hash_len);

    EVP_MD_CTX_free(ctx);

    return std::vector<unsigned char>(hash, hash + hash_len);
}

// Extract the last `n_bytes` from the MD5 hash
std::string get_md5_tail(const std::string& input, size_t n_bytes) {
    auto hash = md5_hash(input);
    std::ostringstream oss;
    for (size_t i = MD5_DIGEST_LENGTH - n_bytes; i < MD5_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

// Floyd’s cycle detection applied to partial MD5 output
void find_partial_md5_collision(size_t tail_bytes) {
    std::unordered_map<std::string, std::string> seen;

    std::string tortoise = "start";
    std::string hare = "start";

    do {
        tortoise = get_md5_tail(tortoise, tail_bytes);
        hare = get_md5_tail(get_md5_tail(hare, tail_bytes), tail_bytes);
    } while (tortoise != hare);

    // Now find the actual inputs that produce the same tail
    std::string a = "start";
    std::string b = "start";

    while (get_md5_tail(a, tail_bytes) != get_md5_tail(b, tail_bytes)) {
        a = get_md5_tail(a, tail_bytes);
        b = get_md5_tail(get_md5_tail(b, tail_bytes), tail_bytes);
    }

    // Now walk the sequence until two *different* inputs yield the same tail
    std::unordered_map<std::string, std::string> history;
    std::string x = "start";
    while (true) {
        std::string tail = get_md5_tail(x, tail_bytes);
        if (history.count(tail) && history[tail] != x) {
            std::cout << "Collision found!\n";
            std::cout << "Input A: " << history[tail] << "\n";
            std::cout << "Input B: " << x << "\n";
            std::cout << "Matching tail: " << tail << std::endl;
            break;
        }
        history[tail] = x;
        x = get_md5_tail(x, tail_bytes);
    }
}

int main() {
    size_t n_hex_digits = 12; // match last 4 hex digits = 2 bytes
    size_t n_bytes = n_hex_digits / 2;

    find_partial_md5_collision(n_bytes);
    return 0;
}
