#include <cassert>
#include <string>
#include <algorithm>
#include <vector>
#include <iostream>
#include <boost/lambda/lambda.hpp>


template <unsigned int x, unsigned int y>
struct static_gcd {
      static const unsigned int value = static_gcd<y, x%y>::value;
};

template<unsigned int x>
struct static_gcd<x,0> {
    static const unsigned int value = x;
};

class A {};
int main()
{
    using namespace std;
    using boost::lambda::_1;
    vector<int> v;
    v.push_back(3);
    v.push_back(4);
    std::for_each(v.begin(), v.end(), _1 += 6);
    cout << static_gcd<0,6>::value << endl;
}
