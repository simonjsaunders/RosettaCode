#include <atomic>
#include <cstdio>
#include <cstring>
#include <future>
#include <iostream>
#include <string>
#include <vector>
#include <openssl/sha.h>

class password_finder
{
public:
    void find_password(const char*);
private:
    std::string find_password2(const unsigned char*, char);
    std::atomic<bool> done;
};

std::string password_finder::find_password2(const unsigned char* digest, char ch)
{
    const int n = 26;
    char passwd[6] = { 0 };
    passwd[0] = ch;
    unsigned char digest2[SHA256_DIGEST_LENGTH];
    std::string result;
    for (int i = 0; i < n && !done; ++i)
    {
        passwd[1] = 'a' + i;
        for (int j = 0; j < n && !done; ++j)
        {
            passwd[2] = 'a' + j;
            for (int k = 0; k < n && !done; ++k)
            {
                passwd[3] = 'a' + k;
                for (int l = 0; l < n && !done; ++l)
                {
                    passwd[4] = 'a' + l;
                    SHA256((const unsigned char*)passwd, 5, digest2);
                    if (memcmp(digest, digest2, SHA256_DIGEST_LENGTH) == 0)
                    {
                        done = true;
                        result = passwd;
                        return result;
                    }
                }
            }
        }
    }
    return result;
}

void password_finder::find_password(const char* hash)
{
    done = false;
    std::string result;
    unsigned char digest[SHA256_DIGEST_LENGTH];
    if (strlen(hash) != 2*SHA256_DIGEST_LENGTH)
    {
        std::cerr << "Invalid SHA-256 hash\n";
        return;
    }
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
    {
        unsigned int x;
        if (sscanf(hash + 2*i, "%2x", &x) != 1)
        {
            std::cerr << "Cannot parse SHA-256 hash\n";
            return;
        }
        digest[i] = x;
    }
    std::vector<std::future<std::string>> results;
    const int n = 26;
    for (int i = 0; i < n; ++i)
    {
        char c = 'a' + i;
        results.push_back(std::async(std::launch::async,
                [&,c]() { return find_password2(digest, c); }));
    }
    for (int i = 0; i < n; ++i)
    {
        std::string str = results[i].get();
        if (!str.empty())
        {
            result = str;
            break;
        }
    }
    std::cout << "password: " << result << ", hash: " << hash << '\n';
}

int main()
{
    password_finder pf;
    pf.find_password("1115dd800feaacefdf481f1f9070374a2a81e27880f187396db67958b207cbad");
    pf.find_password("3a7bd3e2360a3d29eea436fcfb7e44c735d117c42d1c1835420b6b9942dd4f1b");
    pf.find_password("74e1bb62f8dabb8125a58852b63bdf6eaef667cb56ac7f7cdba6d7305c50a22f");
    return 0;
}
