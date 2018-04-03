#pragma once
#include <iostream>
#include <ostream>
#include <functional>
template <typename T>
class Property
{
public:
	Property() :value(T()) {}
	Property(const T &t) : values(T t) {}
	virtual ~Property() {}
	virtual Property<T>& operator=(const T &t)
	{
		value = setter_(t);
		return *this;
	}
	virtual operator const T() const { return getter_(); }
	void setter(std::function<T(const T&)> sttr) { setter_ = sttr; }
	void getter(std::function<T(void)> gttr) { getter_ = gttr; }
	T getValue() { return value; }
protected:
	T value;

	std::function<T(const T&)> setter_ = [](const T& t) { return t; };  // setter
	std::function<T()> getter_ = [=]() { return value; };
};
std::ostream& operator<<(std::ostream& out, Property<std::string>& p)
{
	std::string temp = p;
	try {
		out << (temp).c_str();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n  exception:\n  " << ex.what() << "\n";
	}
	return out;
}
template<typename T>
class Property<const T>
{
public:
	Property() : value(T()) {}
	Property(const Property<T>& p) : value(p.value) {}
	Property(const T& t) : value(t) {}
	virtual ~Property() {}
	void getter(std::function<T(void)> gttr) { getter_ = gttr; }
	virtual operator const T() const { return getter_(); }    // getter
	T getValue() { return value; }                            // used by custom getters
protected:
	T value;
	std::function<T()> getter_ = [=]() { return value; };     // default gettter
};

/*----< overload insertion operator for std::string properties >---*/

std::ostream& operator<<(std::ostream& out, Property<const std::string>& p)
{
	std::string temp = p;
	try {
		out << (temp).c_str();
	}
	catch (std::exception& ex)
	{
		std::cout << "\n  exception:\n  " << ex.what() << "\n";
	}
	return out;
}
