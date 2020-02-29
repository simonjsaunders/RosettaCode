#include <atomic>
#include <cstdio>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <openssl/sha.h>

struct sha256
{
    unsigned char digest[SHA256_DIGEST_LENGTH];
    void compute(const char* str, int len)
    {
        SHA256((const unsigned char*)str, len, digest);
    }
    bool parse(const std::string& hash)
    {
        if (hash.length() != 2*SHA256_DIGEST_LENGTH)
        {
            std::cerr << "Invalid SHA-256 hash\n";
            return false;
        }
        const char* p = hash.c_str();
        for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i, p += 2)
        {
            unsigned int x;
            if (sscanf(p, "%2x", &x) != 1)
            {
                std::cerr << "Cannot parse SHA-256 hash\n";
                return false;
            }
            digest[i] = x;
        }
        return true;
    }
};

bool operator==(const sha256& a, const sha256& b)
{
    return memcmp(a.digest, b.digest, SHA256_DIGEST_LENGTH) == 0;
}

class password_finder
{
public:
    void find_passwords(const std::vector<std::string>&);
private:
    void find_passwords(char);
    std::vector<std::string> hashes;
    std::vector<sha256> digests;
    std::atomic<int> count;
};

void password_finder::find_passwords(char ch)
{
    const int n = 26;
    char passwd[6] = { 0 };
    passwd[0] = ch;
    sha256 digest;
    for (int i = 0; i < n && count > 0; ++i)
    {
        passwd[1] = 'a' + i;
        for (int j = 0; j < n && count > 0; ++j)
        {
            passwd[2] = 'a' + j;
            for (int k = 0; k < n && count > 0; ++k)
            {
                passwd[3] = 'a' + k;
                for (int l = 0; l < n && count > 0; ++l)
                {
                    passwd[4] = 'a' + l;
                    digest.compute(passwd, 5);
                    for (int m = 0; m < hashes.size(); ++m)
                    {
                        if (digest == digests[m])
                        {
                            --count;
                            std::cout << "password: " << passwd << ", hash: " << hashes[m] << '\n';
                        }
                    }
                }
            }
        }
    }
}

void password_finder::find_passwords(const std::vector<std::string>& h)
{
    hashes = h;
    digests.resize(hashes.size());
    for (int i = 0; i < hashes.size(); ++i)
    {
        if (!digests[i].parse(hashes[i]))
            return;
    }
    count = hashes.size();
    std::vector<std::future<void>> futures;
    const int n = 26;
    for (int i = 0; i < n; ++i)
    {
        char c = 'a' + i;
        futures.push_back(std::async(std::launch::async,
                [&,c]() { return find_passwords(c); }));
    }
    for (int i = 0; i < n; ++i)
    {
        futures[i].get();
    }
}

int main()
{
    std::vector<std::string> hashes{
        "1115dd800feaacefdf481f1f9070374a2a81e27880f187396db67958b207cbad",
        "3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b",
        "74e1bb62f8dabb8125a58852b63bdf6eaef667cb56ac7f7cdba6d7305c50a22f"};
    password_finder pf;
    pf.find_passwords(hashes);
    return 0;
}
