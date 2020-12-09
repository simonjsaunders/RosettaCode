#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <vector>
#include <openssl/sha.h>

class sha256 {
public:
    sha256() { reset(); }
    sha256(const sha256&) = delete;
    sha256& operator=(const sha256&) = delete;
    bool reset() {
        return SHA256_Init(&context_) != 0;
    }
    bool update(const void* data, size_t length) {
        return SHA256_Update(&context_, data, length) != 0;
    }
    bool finalize() {
        return SHA256_Final(digest_, &context_) != 0;
    }
    const unsigned char* digest() const {
        return digest_;
    }
    size_t length() const {
        return SHA256_DIGEST_LENGTH;
    }
private:
    unsigned char digest_[SHA256_DIGEST_LENGTH];
    SHA256_CTX context_;
};

void print_digest(std::ostream& out, const std::vector<unsigned char>& digest) {
    out << std::hex;
    for (size_t i = 0; i < digest.size(); ++i)
        out << std::setw(2) << std::setfill('0') << static_cast<int>(digest[i]);
    out << '\n';
}

std::vector<unsigned char> sha256_merkle_tree(std::istream& in, size_t block_size) {
    std::vector<std::vector<unsigned char>> hashes;
    std::vector<char> buffer(block_size);
    sha256 md;
    while (in) {
        in.read(buffer.data(), block_size);
        size_t bytes = in.gcount();
        if (bytes == 0)
            break;
        md.reset();
        md.update(buffer.data(), bytes);
        md.finalize();
        hashes.emplace_back(md.digest(), md.digest() + md.length());
    }
    if (hashes.empty())
        return {};
    size_t length = hashes.size();
    while (length > 1) {
        size_t j = 0;
        for (size_t i = 0; i < length; i += 2, ++j) {
            auto& digest1 = hashes[i];
            auto& digest_out = hashes[j];
            if (i + 1 < length) {
                auto& digest2 = hashes[i + 1];
                md.reset();
                md.update(digest1.data(), digest1.size());
                md.update(digest2.data(), digest2.size());
                md.finalize();
                digest_out.assign(md.digest(), md.digest() + md.length());
            } else {
                digest_out = digest1;
            }
        }
        length = j;
    }
    return hashes[0];
}

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: " << argv[0] << " filename\n";
        return EXIT_FAILURE;
    }
    std::ifstream in(argv[1]);
    if (!in) {
        std::cerr << "Cannot open file " << argv[1] << ".\n";
        return EXIT_FAILURE;
    }
    print_digest(std::cout, sha256_merkle_tree(in, 1024));
    return EXIT_SUCCESS;
}
