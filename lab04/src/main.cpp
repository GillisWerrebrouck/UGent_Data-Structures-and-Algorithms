#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <vector>
#include <set>
#include <queue>
#include <chrono>
#include <random>
#include <algorithm>
#include <string>
#include <limits.h>

#include "hashfunctions.h"

/*
Reads all the files in "folder". Assumes that they are named 0.py, 1.py, ...
Returns a vector with for each file a set of of tokens in that file. 
Tokens are unique words or bigrams (two sequential words).
*/
std::vector<std::set<std::string>> readDatafiles(const char *folder)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<std::string>> contents;

    int i = 0;
    std::ifstream in(folder + std::to_string(i) + ".py");

    while (in)
    {
        std::string word;
        std::string previous = "";
        contents.push_back(std::set<std::string>());

        while (in)
        {
            in >> word;
            contents[i].insert(word);

            previous = previous + " " + word;
            contents[i].insert(previous);

            previous = word;
        }

        i++;
        in = std::ifstream(folder + std::to_string(i) + ".py");
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Reading the data files took " << duration.count() << " ms" << std::endl;

    return contents;
}

// Replaces each unique word with a unique id (0, 1, 2, ...).
std::vector<std::set<int>> replaceWithUniqueId(const std::vector<std::set<std::string>> &input)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<int>> contents(input.size());

    std::vector<std::string> unique_input;

    // replace each n-gram by a unique id and keep track of unique ids by using a vector which keeps track of unique n-grams
    for(int i = 0; i < input.size(); i++) {
        for(std::string s : input[i]) {
            // check if the word has been used as an id before
            std::vector<std::string>::iterator it = find(unique_input.begin(), unique_input.end(), s);

            // add the n-gram to the vector if the n-gram has not been seen before
            // the id for an n-gram is its index in the vector
            int index;
            if(it == unique_input.end()) {
                unique_input.push_back(s);
                index = unique_input.size() - 1;
            } else {
                index = std::distance(unique_input.begin(), it);
            }

            contents[i].insert(index);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Replacing words with IDs took " << duration.count() << " ms" << std::endl;
    return contents;
}

// Replaces each word with its hash code.
template <unsigned int (*hashfunction)(const std::string &)>
std::vector<std::set<int>> replaceWithHash(const std::vector<std::set<std::string>> &input)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::set<int>> contents(input.size());

    for(int i = 0; i < input.size(); i++) {
        for(std::string s : input[i]) {
            int hashcode = hashfunction(s);
            contents[i].insert(hashcode);
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Replacing words with hash values took " << duration.count() << " ms" << std::endl;
    return contents;
}

// Print_collisions: boolean value that decides if the collisions need to be printed.
template <unsigned int (*hashfunction)(const std::string &)>
void findCollisions(const std::vector<std::set<std::string>> &input, bool print_collisions)
{
    std::vector<std::string> unique_input;

    // get all unique n-grams
    for(int i = 0; i < input.size(); i++) {
        for(std::string s : input[i]) {
            std::vector<std::string>::iterator it = find(unique_input.begin(), unique_input.end(), s);
            if(it == unique_input.end()) {
                unique_input.push_back(s);
            }
        }
    }

    int same_collision_count = 0;

    // find collisions
    for(int i = 0; i < unique_input.size(); i++) {
        int collision_count = 0;
        for(int o = i+1; o < unique_input.size(); o++) {
            int hashcode1 = hashfunction(unique_input[i]);
            int hashcode2 = hashfunction(unique_input[o]);
            if(hashcode1 == hashcode2) {
                if(print_collisions) {
                    std::cout << "Collision found: \"" << unique_input[i] << "\" and \"" << unique_input[o] << "\" both map to " << hashcode1 << std::endl;
                }
                collision_count++;
            }
        }
        if(0 < collision_count) {
            same_collision_count++;
        }
    }
    std::cout << "Collision count: " << same_collision_count << std::endl;
}

// Get the intersecting set between two sets.
template <typename T>
std::set<T> get_intersection(std::set<T> set1, std::set<T> set2)
{ 
    std::set<T> intersection;
    std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), inserter(intersection, intersection.begin()));
    return intersection;
} 

// Calculate all jaccard indices (priority queue) for all documents (sets).
template <typename T>
std::vector<std::priority_queue<std::pair<double, int>>> jaccard(const std::vector<std::set<T>> &contents)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::priority_queue<std::pair<double, int>>> similarities(contents.size());

    // compare each two documents and use the intersection to calculate the jaccard index
    for(int i = 0; i < contents.size(); i++) {
        for(int o = i+1; o < contents.size(); o++) {
            int size1 = contents[i].size();
            int size2 = contents[o].size();

            std::set<T> intersection = get_intersection(contents[i], contents[o]);
            int union_size = size1 + size2 - intersection.size();
            double jaccard_index = intersection.size() / (double)union_size;

            similarities[i].push(std::make_pair(jaccard_index, o));
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Calculating the Jaccard index took " << duration.count() << " ms" << std::endl;
    return similarities;
}

// Show for each document the most similar other documents. Each document only needs to occur once in the list.
// Ignore the document if the similarity score is lower than "threshold" 
void showSummary(std::vector<std::priority_queue<std::pair<double, int>>> &similarities, double threshold)
{
    std::set<int> seen_docs;
    std::cout << std::left << std::setw(8) << "doc" << "Similar documents" << std::endl << "-------------------------------";

    for(int i = 0; i < similarities.size(); i++) {
        std::priority_queue<std::pair<double, int>> p = similarities[i];
        bool is_doc_nr_printed = false;
                
        if(p.empty()) continue;
        std::pair<double, int> s = p.top();
        p.pop();

        // print the similarities that haven't been printed before
        while (!p.empty() && s.first >= threshold) {
            if(seen_docs.find(s.second) == seen_docs.end()) {
                if(!is_doc_nr_printed) {
                    std::cout << std::endl << std::setw(8) << i;
                    is_doc_nr_printed = true;
                }
                std::cout << s.second << "(" << s.first << ") ";
                seen_docs.insert(s.second);
            }
            s = p.top();
            p.pop();
        }
    }

    std::cout << std::endl;
}

// credits: https://www.geeksforgeeks.org/program-to-find-the-next-prime-number/
bool is_prime(int n)
{
    // Corner cases
    if (n <= 1) return false;
    if (n <= 3) return true;

    // This is checked so that we can skip 
    // middle five numbers in below loop
    if (n%2 == 0 || n%3 == 0) return false;

    for (int i = 5; i*i <= n; i = i+6)
        if (n%i == 0 || n%(i+2) == 0)
            return false;

    return true;
}

int get_next_prime(int n)
{
    auto start = std::chrono::high_resolution_clock::now();

    bool found = false;
    while(!found)
        found = is_prime(++n);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Computing the next prime took " << duration.count() << " ms" << std::endl;

    return n;
}

// Compute an estimated jaccard index for each two documents using minhashing.
std::vector<std::priority_queue<std::pair<double, int>>> minhashing(const std::vector<std::set<int>> &contents, int n)
{
    auto start = std::chrono::high_resolution_clock::now();

    std::vector<std::priority_queue<std::pair<double, int>>> similarities(contents.size());

    // find the maximum hash value over all sets
    int max = 0;
    for(int i = 0; i < contents.size(); i++) {
        int x = *(std::max_element(contents[i].begin(), contents[i].end()));
        if(x > max) max = x;
    }

    // generate a and b values for each k
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, max-1);
    std::vector<std::pair<int, int>> rands(n);
    for(int k = 0; k < n; k++) {
        int a = dist(rng);
        int b = dist(rng);
        rands[k] = std::make_pair(a, b);
    }

    // c is the next prime number after the max hash value
    int c = get_next_prime(max);

    // compute all k min hashes of all documents
    std::vector<std::set<int>> min_hash_sets(contents.size());
    for(int i = 0; i < contents.size(); i++) {
        std::set<int> min_hashes;
        for(int k = 0; k < n; k++) {
            int a = rands[k].first;
            int b = rands[k].second;

            int min_hash = INT_MAX;
            for(int x : contents[i]) {
                int hash = (a*x + b) % c;
                if(min_hash > hash) min_hash = hash;
            }
            min_hashes.insert(min_hash);
        }
        min_hash_sets[i] = min_hashes;
    }

    // compare all documents their min hashes and use the intersection to estimate the jaccard index
    for(int i = 0; i < contents.size(); i++) {
        for(int o = i+1; o < contents.size(); o++) {
            std::set<int> intersection = get_intersection(min_hash_sets[i], min_hash_sets[o]);
            double estimated_jaccard_index = intersection.size() / (double)n;
            similarities[i].push(std::make_pair(estimated_jaccard_index, o));
        }
    }

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);
    std::cout << "Minhashing took " << duration.count() << " ms" << std::endl;
    return similarities;
}

int main()
{
    // Read the source code as sets of strings
    std::vector<std::set<std::string>> contents = readDatafiles("src/quiz/");
    std::cout << contents.size() << " files"<< std::endl;

    // Replace strings with integeres
    // std::vector<std::set<int>> contentsId = replaceWithUniqueId(contents);
    std::vector<std::set<int>> contentsId = replaceWithHash<good_hash>(contents);

    // Check if a certain hash function results in collisions
    // Set 2nd param true to print the detected collisions
    findCollisions<jenkins_one_at_a_time_hash>(contents, false);

    // Calculate the Jaccard similarity either on sets of strings or on sets of ints
    // std::vector<std::priority_queue6<std::pair<double, int>>> similarities = jaccard<std::string>(contents);
    // std::vector<std::priority_queue<std::pair<double, int>>> similarities = jaccard<int>(contentsId);

    // Estimate the Jaccard similarity using minhashing
    std::vector<std::priority_queue<std::pair<double, int>>> similarities = minhashing(contentsId, 50);

    // Show the results
    showSummary(similarities, 0.75);

    return 0;
}
