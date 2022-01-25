#include <iostream>
#include <iomanip>
#include <fstream>
#include <random>
#include <list>
#include <string>
#include <chrono>
#include "md5.hpp"

struct Md5_Tester_Config
{
    std::string alphabet;
    int rounds;
    int str1_len;
    int str2_len;
    int min_match;
} CONFIG;

struct Result
{
    std::string str1;
    std::string str2;
    std::string hash1;
    std::string hash2;
    int match_length;
};

void write_output(std::list<Result>& results)
{
    std::ofstream outfile("config/output.csv");

    outfile << "matches,string 1,string 2,md5-hash 1,md5-hash 2\n";

    for (Result result : results)
    {
        outfile << result.match_length << ","
                << result.str1 << ","
                << result.str2 << ","
                << result.hash1 << ","
                << result.hash2 << "\n";
    }
    outfile.close();
}

void read_config()
{
    std::ifstream config("md5_tester.config");
    std::array<std::string, 5> conf_lines;

    if (config.is_open())
    {
        std::string line;
        int i = 0;

        while (std::getline(config, line))
        {
            if (line[0] != '#')
            {
                conf_lines[i] = line;
                i++;
            }
        }
    }

    config.close();

    CONFIG.alphabet = conf_lines[0];
    CONFIG.rounds = std::stoi(conf_lines[1]);
    CONFIG.str1_len = std::stoi(conf_lines[2]);
    CONFIG.str2_len = std::stoi(conf_lines[3]);
    CONFIG.min_match = std::stoi(conf_lines[4]);
}

std::string create_random_string(std::mt19937& gen)
{
    std::uniform_int_distribution<> dist(0, CONFIG.alphabet.size());

    std::string randStr;

    for (size_t i = 0; i < CONFIG.str1_len; ++i)
    {
        randStr += CONFIG.alphabet[dist(gen)];
    }

    return randStr;
}

void test_md5(std::list<Result>& results, std::mt19937& gen)
{
    Result res;
    // clock_t rs_s, rs_e, md5_s, md5_e;
    // rs_s = clock();
    res.str1 = create_random_string(gen);
    // rs_e = clock();
    res.str2 = create_random_string(gen);
    // md5_s = clock();
    res.hash1 = md5(res.str1);
    // md5_e = clock();
    res.hash2 = md5(res.str2);
    res.match_length = 0;

    for (int i = 0; i <= res.hash1.size(); i++)
    {
        if (res.hash1[i] != res.hash2[i])
            break;
        ++(res.match_length);
    }

    if ( res.match_length >= CONFIG.min_match)
        results.push_back(res);

    /*
    std::cout << "rand_str: " << rs_e - rs_s << "\u03BCs\n"
              << "md5: " << md5_e - md5_s << "\u03BCs\n";
    */
}

void run_tests(std::mt19937& gen)
{
    std::list<Result> results;

    for (int round = 1; round <= CONFIG.rounds; round++)
    {
        test_md5(results, gen);

        if ((round % (CONFIG.rounds/100)) == 0)
            std::cerr << "[progress: " << round*100/CONFIG.rounds << "%]\n";
    }

    write_output(results);
}

int main()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    clock_t runtime_start, runtime_end;

    read_config();

    runtime_start = clock();
    run_tests(gen);
    runtime_end = clock();

    std::cout << "\nTotal runtime:\t" << std::fixed << std::setprecision(2)
              << (double) (runtime_end - runtime_start) / CLOCKS_PER_SEC << "s (" << runtime_end - runtime_start << "\u03BCs)\n";

    return EXIT_SUCCESS;
}