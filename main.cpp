#include <iostream>
#include <vector>
#include <string>
//#include <bits/stdc++.h>
//#include <algorithm> //DOES NOT DEFINE STD::ACCUMULATE
#include <numeric>
//using namespace std;

template< typename T >
concept int_generic = std::is_same_v< T, int > || std::is_same_v< T, short int > ||
                      std::is_same_v< T, long int > || std::is_same_v< T, long long int>;

class BigNumber{
private:
    std::vector<int> digits_;
public:
    BigNumber(){}
    template< int_generic T>
    BigNumber(const T n){
        if(n==0){
            digits_.push_back(0);
        }else{
            T m = n;
            while(m > 0){
                digits_.push_back(static_cast<int>(m % 10));
                m /= 10;
            }
        }
    }
    BigNumber(std::vector<int>& digits) {
        digits_ = digits;
        collapse_zeros();
    }

    BigNumber(const std::string& str){
        for(int i = str.size() - 1; i >=0; i--){
            digits_.push_back(str[i] - '0');
        }
    }

    BigNumber(const BigNumber& other) : digits_(other.digits_) {}

    BigNumber& operator=(const BigNumber& other){
        //void operator=(const BigNumber& other){
        if( this == &other) return *this;
        digits_ = other.digits_;
        return *this;
    }

    template<int_generic T>
    BigNumber& operator=(const T& num){
        //void operator=(const T& num){
        if(num==0){
            digits_ = std::vector<int>{0};
        }else{
            T m = num;
            std::vector<int> newdigits;
            while(m > 0){
                newdigits.push_back(static_cast<int>(m % 10));
                m /= 10;
            }
            digits_ = newdigits;
        }
        return *this;
    }

    //MOVE CTOR:
    BigNumber(BigNumber&& other) noexcept: digits_(std::move(other.digits_)){}

    //MOVE ASSIGNMENT OVERLOADED:
    BigNumber& operator=(BigNumber&& other) noexcept{
        //void operator=(BigNumber&& other) noexcept{
        if( this == &other) return *this;
        digits_ = std::move(other.digits_);
        return *this;
    }

    std::vector<int> get_digits() const { return digits_; }

    void print_digits() const {
        for(auto elem : digits_) std::cout << elem << " ";
        std::cout << "\n";
    }
    void collapse_zeros(){
        if(digits_.size() > 1 && !std::accumulate(digits_.begin(),digits_.end(),0)) digits_ = std::vector<int>{0};
    }

    BigNumber& operator+(const BigNumber& other){
        std::vector<int> other_digits = other.get_digits();
        std::vector<int> res_digits;
        int extra = 0;
        int res = 0;
        auto iter1 = digits_.begin();
        auto iter2 = other_digits.begin();
        while(iter1 < digits_.end() || iter2 < other_digits.end()){
            res = 0;
            if(iter1 < digits_.end()) {
                res += *iter1;
                iter1++;
            }
            if(iter2 < other_digits.end()) {
                res += *iter2;
                iter2++;
            }
            res += extra;
            extra = res / 10;
            res = res % 10;
            res_digits.push_back(res);
            /*iter1++;
        iter2++;
        std::cout << "Next digits:\n" << *(iter1) << "\n";
        std::cout << *(iter2) << "\n";*/
        }
        if(extra) res_digits.push_back(extra);
        return *new BigNumber(res_digits);
    }
    BigNumber& operator*(const BigNumber& other){
        BigNumber bigsum = 0;
        std::vector<int> other_digits = other.get_digits();
        for(int i{};i<other_digits.size();i++){
            std::vector<int> res_digits;
            for(int j=i;j>0;j--) res_digits.push_back(0);
            int extra = 0;
            int res = 0;
            int mult = other_digits[i];
            auto iter = digits_.begin();
            while(iter < digits_.end()){
                res = *iter * mult;
                res += extra;
                extra = res / 10;
                res = res % 10;
                res_digits.push_back(res);
                iter++;
            }
            if(extra) res_digits.push_back(extra);
            BigNumber bignew(res_digits);
                //bignew.print_digits();
            bigsum = bigsum + bignew;
        }
        return *new BigNumber(bigsum);
    }
    template<int_generic U>
    BigNumber& operator* (const U& num){
        return *new BigNumber(*this * BigNumber(num));
    }

    friend std::ostream& operator<<(std::ostream& out, const BigNumber& bn);

};
std::ostream& operator<<(std::ostream& out, const BigNumber& bn){
    std::string newstr;
    for(auto it = bn.digits_.rbegin(); it < bn.digits_.rend();++it){
        newstr += std::to_string(*it);
    }
    out << newstr;
    return out;
}

int main()
{
    auto number1 = BigNumber("114575");
    //auto number1 = BigNumber(114575);
    auto number2 = BigNumber("78524");
    //number1.print_digits();
    //std::cout << number1 << "\n";
    auto result = number1 + number2;
    std::cout << result; // 193099

    std::cout << "\n";
    std::cout << "\nANOTHER CHECK:\n";
    BigNumber result2 = 114575 + 78524;
    result2 = 99999 + 1;
    std::cout << result2; // 100000

    std::cout << "\n";
    std::cout << "\nCHECKING DIFFERENT INTEGER TYPES:\n";
    BigNumber bg(123);
    BigNumber bg2(456LL);
    BigNumber bgsum = bg + bg2;
    std::cout << bgsum << "\n"; //579

    std::cout << "\nCHECKING MULTIPLICATION:\n";
    bg = bg * 10;
    std::cout << bg << "\n"; // 1230

    return 0;
}
