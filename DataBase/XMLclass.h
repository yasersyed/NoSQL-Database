///////////////////////////////////////////////////////////////////
// XMLclass.h - Contains methods persisting database in XML file //
// and restoring database from XML file							 //
// Ver 1.0                                                       //
// Application: TestExecutive for Project#1                      //
// Platform:    MacBook Pro, Win 10 Education, Visual Studio 2015//
// Author:      Syed Yaser Ahmed				                 //
//              sysyed@syr.edu		                             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This header contains class for containing functions used in persistance and restore.
* It also contains functions for trimming out whitespaces from tagged elements
* Used by TestExecutive for storing database
*
*	XMLclass functions
*  ---------------------------------
*   std::string toXml(NoSqlDb<StrData>& x) //To persist database in xml string, which is then stored in a file
*   NoSqlDb<StrData> fromXML(NoSqlDb<StrData> db1, std::string& xml) //Getting database elements restored from XML file
*   static inline std::string &ltrim(std::string &s) // Left trim a string
*   static inline std::string &rtrim(std::string &s) // Right trim a string
*   static inline std::string &trim(std::string &s)  // Trim on both sides
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
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <string>
#include "../XMLDocument/XMLDocument.h"
#include "../XMLDocument/XMLElement.h"
#include "../Properties/Properties.h"
#include "../Convert/Convert.h"

//using namespace XmlProcessing;
using SPtr = std::shared_ptr<XmlProcessing::AbstractXmlElement>;
using StrData = std::string;
using Data = std::string;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;
// trim from start
static inline std::string &ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
		std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

// trim from end
static inline std::string &rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(),
		std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

// trim from both ends
static inline std::string &trim(std::string &s) {
	try { return ltrim(rtrim(s)); }
	catch (...) { return std::string("trimming error"); }
}
class XMLclass{
public:
	std::string toXml(NoSqlDb<StrData>& x)
	{
		Keys k = x.keys();
		std::string xml;
		XmlProcessing::XmlDocument doc;
		int childcount = 1;
		SPtr pRoot = XmlProcessing::makeTaggedElement("Database");
		doc.docElement() = pRoot;
		for (Key ki : k) {
			metadata<Data> d;
			d = x.value(ki).returns();
			SPtr pRoot1 = XmlProcessing::makeTaggedElement("Element");
			pRoot->addChild(pRoot1);
			SPtr pNameElem = XmlProcessing::makeTaggedElement("Key");
			pRoot1->addChild(pNameElem);
			SPtr pTextElem = XmlProcessing::makeTextElement(ki);
			pNameElem->addChild(pTextElem);
			pNameElem = XmlProcessing::makeTaggedElement("Item");
			pRoot1->addChild(pNameElem);
			pTextElem = XmlProcessing::makeTextElement(d.name);
			pNameElem->addChild(pTextElem);
			pNameElem = XmlProcessing::makeTaggedElement("Category");
			pRoot1->addChild(pNameElem);
			pTextElem = XmlProcessing::makeTextElement(d.category);
			pNameElem->addChild(pTextElem);
			pNameElem = XmlProcessing::makeTaggedElement("TimeDate");
			pRoot1->addChild(pNameElem);
			pTextElem = XmlProcessing::makeTextElement(d.timeDate);
			pNameElem->addChild(pTextElem);
			pNameElem = XmlProcessing::makeTaggedElement("Description");
			pRoot1->addChild(pNameElem);
			pTextElem = XmlProcessing::makeTextElement(d.description);
			pNameElem->addChild(pTextElem);
			pNameElem = XmlProcessing::makeTaggedElement("Data");
			pRoot1->addChild(pNameElem);
			pTextElem = XmlProcessing::makeTextElement(d.data);
			pNameElem->addChild(pTextElem);
			std::vector<std::string> newv = d.children;
			for (std::string ch : newv)
			{
				pNameElem = XmlProcessing::makeTaggedElement("child");
				pRoot1->addChild(pNameElem);
				pTextElem = XmlProcessing::makeTextElement(ch);
				pNameElem->addChild(pTextElem);
			}
			xml = doc.toString();
		}
		return xml;
	}
	NoSqlDb<StrData> fromXML(NoSqlDb<StrData> db1, std::string& xml)
	{
		try {
			XmlProcessing::XmlDocument doc(xml);
			std::vector<SPtr> desc = doc.element("Element").select();
			for (size_t i = 0; i < desc.size(); i++)
			{
				metadata<Data> db;
				std::vector<SPtr> desc = doc.element("Item").select();
				std::string name_value = desc[i]->children()[0]->value();
				db.name = ltrim(rtrim(name_value));
				desc = doc.element("Category").select();
				std::string cateString = desc[i]->children()[0]->value();
				db.category = ltrim(rtrim(cateString));
				desc = doc.element("TimeDate").select();
				std::string timeString = desc[i]->children()[0]->value();
				db.timeDate = ltrim(rtrim(timeString));
				desc = doc.element("Description").select();
				std::string descString = desc[i]->children()[0]->value();
				db.description = ltrim(rtrim(descString));
				desc = doc.element("Data").select();
				std::string dataString = desc[i]->children()[0]->value();
				db.data = ltrim(rtrim(dataString));
				desc = doc.element("child").select();
				std::vector<std::string> childString;
				for (size_t j = 0; j < desc.size(); j++)
					childString.push_back(ltrim(rtrim(desc[0]->children()[0]->value())));
				db.children = childString;
				db1.save(db.name, db);
				return db1;
			}
		}
		catch (std::exception& ex)
		{
			std::cout << ex.what();
		}
		return db1;
	}
};