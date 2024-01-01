// Approach 2: each thread have its own dictionary, and I merge them at the end

#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <vector>
#include <chrono>
#include <future>

// Variable declaration //
const size_t TOPK = 10;
using Counter = std::map<std::string, std::size_t>;

// fuction forward declaration //
std::string tolower(const std::string &str);
void count_words(std::istream& stream, Counter& counter);
void print_topk(std::ostream& stream, const Counter& counter, const size_t k);

Counter process_file(const char* filename);

// main function //
int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: topk_words [FILES...]\n";
        return EXIT_FAILURE;
    }

    auto start = std::chrono::high_resolution_clock::now();
    Counter freq_dict;

    std::vector<std::future<Counter>> futures;
    for (int i = 1; i < argc; ++i) {
        futures.push_back(std::async(std::launch::async, process_file, argv[i]));
    }

    for (auto& future : futures) {
        Counter thread_dict = future.get();
        // Merge thread dictionaries
        for (const auto& pair : thread_dict) {
            freq_dict[pair.first] += pair.second;
        }
    }

    print_topk(std::cout, freq_dict, TOPK);
    auto end = std::chrono::high_resolution_clock::now();
    auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    std::cout << "Elapsed time is " << elapsed_ms.count() << " us\n";
}

// function implementation // 

Counter process_file(const char* filename) {
    std::ifstream input{filename};
    if (!input.is_open()) {
        std::cerr << "Failed to open file " << filename << '\n';
        return Counter();
    }

    Counter local_dict;
    count_words(input, local_dict);
    return local_dict;
}

std::string tolower(const std::string &str) {
    std::string lower_str;
    std::transform(std::cbegin(str), std::cend(str),
                   std::back_inserter(lower_str),
                   [](unsigned char ch) { return std::tolower(ch); });
    return lower_str;
};

void count_words(std::istream& stream, Counter& counter) {
    std::for_each(std::istream_iterator<std::string>(stream),
                  std::istream_iterator<std::string>(),
                  [&counter](const std::string &s) { ++counter[tolower(s)]; });    
}

void print_topk(std::ostream& stream, const Counter& counter, const size_t k) {
    std::vector<Counter::const_iterator> words;
    words.reserve(counter.size());
    for (auto it = std::cbegin(counter); it != std::cend(counter); ++it) {
        words.push_back(it);
    }

    std::partial_sort(
        std::begin(words), std::begin(words) + k, std::end(words),
        [](auto lhs, auto &rhs) { return lhs->second > rhs->second; });

    std::for_each(
        std::begin(words), std::begin(words) + k,
        [&stream](const Counter::const_iterator &pair) {
            stream << std::setw(4) << pair->second << " " << pair->first
                      << '\n';
        });
}