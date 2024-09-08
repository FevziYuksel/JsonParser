#pragma once
#include <map>
#include <unordered_map>
#include <set>
#include <vector>
#include <iostream>

typedef int DEBUG_LEVEL;
int d_level = 0;

template<typename Key, typename Value>
std::ostream& operator<<(std::ostream& os, const std::pair<const Key, Value>& p)
{
	os << p.first << " : " << p.second;
	return os;
}

template<typename Key, typename Value >
std::ostream& operator<<(std::ostream& os, const std::map<Key, Value>& m)
{
	os << "[ "; // 1

	for (const auto& [k, v] : m)
	{
		os << k << " : " << v << ", "; // 2
	}

	os << "]"; // 3

	return os;
}

template<typename Key, typename Value >
std::ostream& operator<<(std::ostream& os, const std::unordered_map<Key, Value>& m)
{
	os << "[ "; // 1

	for (const auto& [k, v] : m)
	{
		os << k << " : " << v << ", "; // 2
	}

	os << "]"; // 3

	return os;
}


template<typename T, template<typename ELEM, typename ALLOC = std::allocator<ELEM>> class Container>
std::ostream& operator<< (std::ostream& os, const Container<T>& container)
{
	typename Container<T>::const_iterator it = container.begin();

	os << "["; // 1

	while (it != container.end())
	{
		os << *it++ << ", "; // 2
	}

	os << "]"; // 3

	return os;
}

template<typename Container>
void log(DEBUG_LEVEL level, const Container& c) {
	if (level >= d_level)
	{
		for (typename Container::const_iterator it = c.begin(); it != c.end(); ++it)
		{
			std::cout << *it << '\n';
		}
	}
}


// OPTIONAL Adding your own types

class MyClass
{
	int i;
	std::string s;
public:
	MyClass(int i, std::string s) : i(i), s(s) {}

	// declare the operator<<() overload as a friend to grant access
	// to private members
	friend std::ostream& operator<<(std::ostream& os, const MyClass& mc);

};

// define the operator<<() for MyClass as a global function (required)
std::ostream& operator<<(std::ostream& os, const MyClass& mc)
{
	os << "{MyClass: " << mc.s << ": " << mc.i << "}";
	return os;
}


// End of OPTIONAL

int test()
{
	std::set<int> s;
	s.insert(6);
	s.insert(3);
	s.insert(4);

	std::map<int, int> m;// {{1, 2}, {3, 4}};
	m[1] = 2;
	m[3] = 4;

	std::vector<int> v;
	v.push_back(4);
	v.push_back(3);
	v.push_back(2);
	v.push_back(1);

	std::cout << "\nset:\n";
	log(1, s);

	std::cout << "\nmap:\n";
	log(1, m);

	std::cout << "\nvector:\n";
	log(1, v);

	std::cout << "\nvector of MyClass:\n";
	std::vector<MyClass> vmc;
	vmc.push_back(MyClass(1, "hello"));
	vmc.push_back(MyClass(2, "world"));

	log(1, vmc);

	return 0;
}