
#ifndef HASHFUNCTIONS_H
#define HASHFUNCTIONS_H

#include <string>

unsigned int worst_hash(const std::string &word)
{
    return 0;
}

unsigned int bad_hash(const std::string &word)
{
    return word.size();
}

unsigned int other_bad_hash(const std::string &word)
{
    unsigned int result = 0;

    for (const auto &c : word)
    {
        result += c;
    }

    return result;
}

unsigned int horner_hash(const std::string &word)
{
    unsigned int horner = 0;

    for (const auto &c : word)
    {
        horner = (horner * 256 + c);
    }

    return horner;
}

unsigned int good_hash(const std::string &word)
{
    unsigned int horner = 0;

    for (const char c : word)
    {
        horner = (horner * 131 + c);
    }

    return horner;
}

// credits: http://www.cse.yorku.ca/~oz/hash.html
unsigned int djb2_hash(const std::string &word)
{
    unsigned int hash = 5381;
    
    for (const char c : word)
    {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }

    return hash;
}

// credits: https://en.wikipedia.org/wiki/Jenkins_hash_function#SpookyHash
unsigned int jenkins_one_at_a_time_hash(const std::string &word)
{
    uint32_t hash = 0;
    for (const char c : word)
    {
        hash += c;
        hash += hash << 10;
        hash ^= hash >> 6;
    }
    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

unsigned int std_hash(const std::string &word)
{
    std::hash<std::string> hash;
    return hash(word);
}

// credits: https://stackoverflow.com/a/8317622/5005651
unsigned int custom_prime_hash(const std::string &word)
{
    unsigned int a = 54059;
    unsigned int b = 76963;
    unsigned int hash = 37;

    for (const char c : word)
    {
        hash = (hash * a) ^ (b * c);
    }

    return hash;
}

// credits: https://www.partow.net/programming/hashfunctions/#StringHashing
unsigned int RS_hash(const std::string &word)
{
   unsigned int a = 63689;
   unsigned int b = 378551;
   unsigned int hash = 0;

   for (const char c : word)
   {
      hash = hash * a + c;
      a = a * b;
   }

   return hash;
}

#endif