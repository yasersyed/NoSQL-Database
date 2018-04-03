///////////////////////////////////////////////////////////////////
// Elements.h - Contains classes for MetaData, NoSqlDb			 //
// Widgets and Widget Persistance Interface						 //
// Ver 1.0                                                       //
// Application: TestExecutive for Project#1                      //
// Platform:    MacBook Pro, Win 10 Education, Visual Studio 2015//
// Author:      Syed Yaser Ahmed				                 //
//              sysyed@syr.edu		                             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This header contains classes for creating Database Elements and Databases themselves.
* metadata<Data> class contains definition for the datatype and functions for displaying metadata of any key
* NoSqlDB<Data> class contains definitions for storing database and deleting keys
* Widget class contains declarations for Properties which can be used instead of Data property in metadata
*
*
* The functions defined in this header are used for creating elements and storing in database
* by the TestExecutive.cpp
*	class metadata<Data> functions
*  ---------------------------------
*   std::string metadata<Data>::show() //To display an element in Database
*   metadata<Data> metadata<Data>::returns() // To get metadata and data from an element used by DBQuery.h
*
*	class NoSqlDb<Data> functions
*   ------------------------------
*   typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys() //To get all keys from a database
*   bool NoSqlDb<Data>::save(Key key, metadata<Data> elem) //Saving an element with its key in unordered map
*   void NoSqlDb<Data>::display(NoSqlDb<Data> db) //Displaying entire database
*   bool NoSqlDb<Data>::editDB1(Key key, metadata<Data> db) //For editing an element
*   void NoSqlDb<Data>::remove(Key key) //To remove a key, value pair from database
*   metadata<Data> NoSqlDb<Data>::value(Key key) //To get the elements of any given key
*   size_t NoSqlDb<Data>::count() //Used to get size of database
*
*	class WidgetWrapper functions
* --------------------------------
*   inline WidgetWrapper::Xml WidgetWrapper::savew() //For persisting widget in an xml string
*   inline void WidgetWrapper::restorew(const Xml& xml) //For restoring database from xml string
*
*
* Required Files:
* ---------------
*   - Convert.h, Convert.cpp
*	- Properties.h, Properties.cpp
*	- XMLDocument.h, XMLDocument.cpp
*   - XMLElement.h, XMLElement.cpp
*
* Build Process:
* --------------
*   devenv Database.sln /debug rebuild
*
* Maintenance History:
* --------------------
* Ver 1.0 : 07 Feb 2017
* - first release
*
*/

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include "../Convert/Convert.h"
#include "../Properties/Properties.h"
#include "../XMLDocument/XMLDocument.h"
#include "../XMLDocument/XMLElement.h"

using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;

template <typename Data>
class metadata {
public:
	using Name = std::string;
	using Category = std::string;
	using TimeDate = std::string;
	using Description = std::string;
	using Children = std::vector<std::string>;
	Property<Name> name;            // metadata
	Property<Category> category;    // metadata
	Property<TimeDate> timeDate;    // metadata
	Property<Description> description; //metadata
	Property<Data> data;            // data
	Property<Children> children;
	std::string show();
	metadata metadata<Data>::returns();
};
template <typename Data>
std::string metadata<Data>::show()
{
	std::ostringstream out;
	out.setf(std::ios::adjustfield, std::ios::left);
	out << "\n    " << std::setw(8) << "name" << " : " << name;
	out << "\n    " << std::setw(8) << "category" << " : " << category;
	out << "\n    " << std::setw(8) << "timeDate" << " : " << timeDate;
	out << "\n    " << std::setw(8) << "description" << " : " << description;
	out << "\n    " << std::setw(8) << "data" << " : " << data;
	out << "\n    " << std::setw(8) << "Children" << " : ";
	std::vector<std::string> newV = children;
	for (std::vector<std::string>::const_iterator i = newV.begin(); i != newV.end(); ++i)
		out << *i << ' ';
	out << "\n";
	return out.str();
}
template <typename Data>
metadata<Data> metadata<Data>::returns() {
	metadata<Data> m;
	m.name = name;
	m.category = category;
	m.description = description;
	m.data = data;
	m.timeDate = timeDate;
	m.children = children;
	return m;
}
template<typename Data>
class NoSqlDb
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;

	Keys keys();
	bool save(Key key, metadata<Data> elem);
	bool editDB1(Key key, metadata<Data> elem);
	void display(NoSqlDb<Data>);
	void remove(Key key);
	metadata<Data> value(Key key);
	size_t count();
private:
	using Item = std::pair<Key, metadata<Data>>;
	std::unordered_map<Key, metadata<Data>> store;
};
template<typename Data>
typename NoSqlDb<Data>::Keys NoSqlDb<Data>::keys()
{
	Keys keys;
	for (Item item : store)
	{
		keys.push_back(item.first);
	}
	return keys;
}

template<typename Data>
bool NoSqlDb<Data>::save(Key key, metadata<Data> elem)
{
	if (store.find(key) != store.end())
		return false;
	store[key] = elem;
	return true;
}
template<typename Data>
void NoSqlDb<Data>::display(NoSqlDb<Data> db)
{
	Keys ki = db.keys();
	for (Key k : ki)
	{
		std::cout << db.value(k).show();
	}
}


template<typename Data>
bool NoSqlDb<Data>::editDB1(Key key, metadata<Data> db)
{
	if (store.find(key) != store.end())
	{
		store[key] = db;
		return true;
	}
	else
		return false;
}
template<typename Data>
void NoSqlDb<Data>::remove(Key key)
{
	try {
		store.erase(key);//Add delete children to this
	}
	catch(std::exception ex)
	{
		std::cout << "Key not found";
	}
}

template<typename Data>
metadata<Data> NoSqlDb<Data>::value(Key key)
{
	if (store.find(key) != store.end())
		return store[key];
	return metadata<Data>();
}

template<typename Data>
size_t NoSqlDb<Data>::count()
{
	return store.size();
}
/////////////////////////////////////////////////////////////////////
// IPersist interface
// - declares methods Persistance Wrappers must use
//
template<typename T>
struct IPersist
{
	using Xml = std::string;

	virtual ~IPersist() {}
	virtual IPersist<T>& operator=(const T& t) = 0;
	virtual operator T() = 0;
	std::string id() { return typeid(*this).name(); }
	virtual Xml savew() = 0;
	virtual void restorew(const Xml& xml) = 0;
};
class Widget
{
public:
	Property<int> wint;
	Property<std::string> wstr;
};
class WidgetWrapper : public IPersist<Widget>
{
public:
	WidgetWrapper() {};
	WidgetWrapper(Widget widget) : widget_(widget) {}
	WidgetWrapper& operator=(const Widget& widget);
	operator Widget();
	Xml savew();
	void restorew(const Xml& xml);
private:
	Widget widget_;
};
//----< assignment operator overload >-------------------------------

WidgetWrapper& WidgetWrapper::operator=(const Widget& widget)
{
	widget_ = widget;
	return *this;
}
inline WidgetWrapper::operator Widget()
{
	return widget_;
}
inline WidgetWrapper::Xml WidgetWrapper::savew()
{
	XmlProcessing::XmlDocument doc;

	// make and add root element to doc
	SPtr root = XmlProcessing::makeTaggedElement("WidgetWrapper");
	doc.docElement() = root;

	// make Property<int> wint element and add to root
	int wintValue = static_cast<Widget>(*this).wint;
	std::string wintStr = Convert<int>::toString(wintValue);
	SPtr pWintElem = XmlProcessing::makeTaggedElement("wint");
	pWintElem->addChild(XmlProcessing::makeTextElement(wintStr));
	root->addChild(pWintElem);

	// make Property<std::string> wstr element and add to root
	std::string wStr = static_cast<Widget>(*this).wstr;
	SPtr pWstrElem = XmlProcessing::makeTaggedElement("wstr");
	pWstrElem->addChild(XmlProcessing::makeTextElement(wStr));
	root->addChild(pWstrElem);
	return doc.toString();
}
//---< restores instance state from XML string >---------------------

inline void WidgetWrapper::restorew(const Xml& xml)
{
	XmlProcessing::XmlDocument doc(xml);
	std::vector<SPtr> elems = doc.element("wint").select();
	std::string temp = elems[0]->children()[0]->value();
	widget_.wint = Convert<int>::fromString(temp);
	elems = doc.element("wstr").select();
	widget_.wstr = elems[0]->children()[0]->value();
}