#include <iostream>
#include <algorithm>
#include <set>
#include <vector>
#include <cmath>
#include <random>
#include <bitset>
#include <chrono>
#include <limits>

#define ui unsigned int

#define MAX_SIZE 12// Change const to solve for S bigger than 3

using namespace std;

random_device rd;
mt19937 gen(rd());


const int n = MAX_SIZE;

uniform_real_distribution<double> dist(0, 0.084);


vector<double> binary_values((1 << MAX_SIZE));

void get_risk_system_solution() {
    // Fill free variables
    binary_values[0] = 0;
    for (int i = 0; i < n; i++) {
        ui val = (1 << i);
        binary_values[val] = dist(gen);
    }

    // Calculate others
    for (int i = 0; i < (1 << n); ++i) {
        bitset<MAX_SIZE> b = i;
        int cnt = b.count();
        if (cnt > 1) {
            binary_values[i] = (1 - cnt) * binary_values[0];
            for (int j = 0; j < n; ++j) {
                if (b[j]) {
                    binary_values[i] += binary_values[(1 << j)];
                }
            }
        }
    }
}

void fetch_subword(set<string> &subword, string s) {
    if (subword.count(s)) {
        return;
    }
    subword.insert(s);
    for (int i = 0; i < s.size(); ++i) {
        if (s[i] == '0' || s[i] == '1') {
            string ns = s;
            ns[i] = '?';
            fetch_subword(subword, ns);
        }
    }
}

double masks_sum(string &pattern, int q) {
    double res = 0;

    vector<int> positions;
    for (int i = 0; i < pattern.size(); ++i) {
        if (pattern[i] == '?')
            positions.emplace_back(i);
    }
    string cur = pattern;
    //cout << "Taken masks:\n";
    for (int i = 0; i < (1 << q); ++i) {
        ui val = i;
        for (int bit = 0; bit < q; ++bit) {
            cur[positions[bit]] = '0' + bool(val & (1 << bit));
        }
        bitset<32> b(cur);
        ui bi = b.to_ulong();
        res += binary_values[bi];
        //cout << cur << " with value " << binary_values[bi] << '\n';
    }
    //cout << "Masks sum: " << res << '\n';
    return res;
}

double calc_word(string word) {

    //cout << "For calc_subword " << word << ":\n";

    double res = 0;
    int q = 0;
    for (int i = 0; i < word.size(); ++i) {
        if (word[i] == '?')
            q++;
    }
    double msum = masks_sum(word, q);
    res = pow(-1, q - 1) * 1.0 / double(1 << q) * msum;

    //cout << "Final result " << pow(-1, q - 1) * 1.0 << " * " << "1/" << (1 << q) << " * " << msum << " = " << res
       //  << "\n\n";

    return res;
}

double calc_subword(set<string> &subword) {
    double res = 0;
    for (auto word: subword) {
        res += calc_word(word);
    }
    return res;
}

int main() {
    freopen("out.txt", "wt", stdout);

    auto start = chrono::high_resolution_clock::now();
    get_risk_system_solution(); // pass 0 to fill with determined values
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = end - start;
    cerr << "Time taken: " << diff.count() << " s\n";

    cout.precision(6);

    for (int i = 0; i < (1 << n); ++i) {
        bitset<MAX_SIZE> bv(i);
        cout << "p[" << i << "] = " << binary_values[i] << '\n';
    }

    start = chrono::high_resolution_clock::now();
    double mx = numeric_limits<double>::max();
    for (ui mask = 0; mask < pow(3, MAX_SIZE); ++mask) {
        set<string> sub_word;
        ui cur = mask, unknown = 0;
        string word;
        while (cur) {
            word += '0' + (cur % 3);
            if (word.back() == '2') {
                word.back() = '?';
                unknown++;
            }
            cur /= 3;
        }
        if (MAX_SIZE - unknown <= 1) {
            continue;
        }
        while (word.size() < MAX_SIZE)
            word += '0';
        reverse(word.begin(), word.end());

        //cout << "Calc for word: " << word << "\n";

        fetch_subword(sub_word, word);

        //cout << "sub_word:\n";

        double res = calc_subword(sub_word);

        cout << "Sum of all sub_word is: " << res << "\n\n\n";
        if(res < mx)
            mx = res;
        //cerr << res << '\n';
    }
    end = chrono::high_resolution_clock::now();
    diff = end - start;
    cerr << "Time taken: " << diff.count() << " s\n";

    cerr << mx;
}
