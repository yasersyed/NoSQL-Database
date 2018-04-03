///////////////////////////////////////////////////////////////////
// TestExecutive.cpp - Contains requirement functions		     //
//	For project#1												 //
// Ver 1.0                                                       //
// Application: TestExecutive for Project#1                      //
// Platform:    MacBook Pro, Win 10 Education, Visual Studio 2015//
// Author:      Syed Yaser Ahmed				                 //
//              sysyed@syr.edu		                             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This Cpp contains requirement execution given for Project#1
* Database.exe performs a serious of operations which include - creating database, saving elements,
* Editing Database, Removing elements, Peristance, Restore
* And queries to be performed on database
*
* Main function has been split into smaller portions as per requirements.
*
*
* Required Files:
* ---------------
*	- Elements.h, DBQuery.h, XMLclass.h
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

#define _CRT_SECURE_NO_WARNINGS

#include "Elements.h"
#include "XMLclass.h"
#include "DBQuery.h"
#include "../XMLDocument/XMLDocument.h"
#include "../XMLDocument/XMLElement.h"
#include "../Properties/Properties.h"
#include "../Convert/Convert.h"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <fstream>
#include <chrono>
#include <thread>
#include <future>

using StrData = std::string;
using Data = std::string;
using VData = Widget;
using Key = NoSqlDb<StrData>::Key;
using Keys = NoSqlDb<StrData>::Keys;

std::string getLocaltime() //gets current time as string
{
	std::time_t _tm = std::time(NULL);
	struct tm * curtime = std::localtime(&_tm);
	char buffer[32];
	std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", curtime); // convert time_t to string for storage
	return buffer;
}
void requirement2b() //Demonstrates using Widget instead of string in Data section of a Key/Value
{
	std::cout << "\n Reqirement 2b, Instance of template demonstrated with Widget \n";
	Widget wdgt;
	WidgetWrapper wwdgt(wdgt);
	wdgt.wint = 1;
	wdgt.wstr = "Sample widget string data";
	WidgetWrapper::Xml xmlW = wwdgt.savew();
	std::cout << "\n  XML saved from WidgetWrapper:" << xmlW<<"\n";
	WidgetWrapper newWwdgt;
	newWwdgt.restorew(xmlW);
	std::cout << "\n  id of restored WidgetWrapper = " << newWwdgt.id();
	std::cout << "\n  WidgetWrapper state:";
	std::cout << "\n    wint = " << static_cast<Widget>(newWwdgt).wint;
	std::cout << "\n    wstr = " << static_cast<Widget>(newWwdgt).wstr <<"\n";
	std::string buffer = getLocaltime();
	NoSqlDb<VData> vd;
	metadata<VData> velem1;
	velem1.name = "Widget";
	velem1.category = "WW";
	velem1.description = "Testing Data with Widget datatype";
	velem1.timeDate = buffer;
	velem1.data = wdgt;
	velem1.children = { "Widget2","Widget3" };
	vd.save(velem1.name, velem1);
	Keys vk = vd.keys();
	for (Key v1 : vk){
		std::cout << "\n Printing Database \n\n\tName: " << vd.value(v1).name << "\n";
		std::cout << "\tCategory: " << vd.value(v1).category << "\n";
		std::cout << "\tDescription: " << vd.value(v1).description << "\n";
		std::cout << "\tTimeDate: " << vd.value(v1).timeDate << "\n";
		std::cout << "\tData:\n";
		Widget wp = vd.value(v1).data;
		std::cout << "\t\tInteger Data: "<< wp.wint << "\n";
		std::cout << "\t\tString Data: "<<wp.wstr << "\n";
		std::cout << "\tChildren: " << "\n";
		std::vector<std::string> newV = vd.value(v1).children;
		for (std::vector<std::string>::const_iterator i = newV.begin(); i != newV.end(); ++i)
			std::cout <<"\t\t"<< *i << "\n";
	}
	std::cout << "\n  Requirement 3-a: Addition of key/value database pairs \n";
}
void requirement5(NoSqlDb<StrData> db,int timeMS) //Demonstrates persisting database in an XML file
{
	std::cout << "\n  Persisting database to XML file \n";
	XMLclass xc;
	try {
		std::string xml = xc.toXml(db);
		std::ofstream file("output.xml");
		file << xml;
		file.close();
	}
	catch (std::exception ex){
		std::cout << ex.what();
	}
	std::this_thread::sleep_for(std::chrono::milliseconds(timeMS));
}
void requirement5b(NoSqlDb<StrData> db)
{
	std::cout << "\n Requirement 5-a: Saved Database in output.xml \n"; 
	XMLclass xc;
	std::cout << "\n  Requirement 5-b: Restoring database from XML file \n";
	std::ifstream ifs("output.xml");
	std::string xmlString((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	std::cout << "Retreiving data from xml \n";
	NoSqlDb<Data> db1 = xc.fromXML(db, xmlString);
	db.display(db1);
}
void requirementQueries(NoSqlDb<StrData> db)
{
	DBQueries<StrData> dbq;
	std::cout << "\n  Requirement 7: Queries for returning keys	\n";
	std::cout << "7-a Finding value of a specified key \n";
	std::string result = dbq.keyvalue(db, "Convert");
	std::cout << "Result : " << result << "\n";
	std::cout << "7-b Finding Child of a specific key \n";
	NoSqlDb<Data> db7b = dbq.keychild(db, "Properties");
	std::cout << "7-c Finding set of all keys matching specified pattern \n";
	NoSqlDb<Data> db7c = dbq.keyregex(db, "(.*)(ert)(.*)");
	std::cout << "7-d Finding keys that contain specified string in item \n";
	NoSqlDb<Data> db7d = dbq.keyname(db, "ert");
	std::cout << "7-e Finding keys that contain specified string in category \n";
	NoSqlDb<Data> db7e = dbq.keycateg(db, "si");
	std::cout << "7-f Finding keys that contain specified string in data \n";
	NoSqlDb<Data> db7f = dbq.keydata(db, ".h");
	std::cout << "7-g Finding keys that contain specified string in data\n";
	NoSqlDb<Data> db7g = dbq.keydate(db, "02:02:2017 00:00:00", ""); //Second parameter will be current time
	std::cout << "\n 8 Querying with keys returned in previous queries \n";
	NoSqlDb<Data> db8 = dbq.keycateg(db7g, "Ass");
	std::cout << "\n 9 Union of keys returned in previous queries \n";
	NoSqlDb<Data> db9 = dbq.keyunion(db8, db7g);
	db.display(db9);
	std::cout << "\n Bonus - Previous Queries types with Regular Expression \n";
	NoSqlDb<Data> bonusDB = dbq.BonusQuery(db, "category", "([a-zD-P])+"); //Parameters are Database, metadata to search, pattern
}
NoSqlDb<StrData> makeElement(NoSqlDb<StrData> db,std::string n,std::string ca,std::string des,Data d,std::string td,std::vector<std::string> ch)
{
	metadata<std::string> elem;
	elem.name = n;
	elem.category = ca;
	elem.description = des;
	elem.data = d;
	elem.timeDate = td;
	elem.children = ch;
	db.save(elem.name, elem);
	return db;
}
int main()
{
	NoSqlDb<StrData> db;
	metadata<StrData> elem1,elem2,elem3,elem4,elem5,element;
	std::cout << "\n Requirement 2: Template class providing key/value db with metadata as value \n ";
	std::string buffer = getLocaltime();
	elem1.name = "Convert";elem1.category = "Serializer";elem1.description = "(de)serializes objects of type T (from)to strings";
	elem1.data = "Convert.h";
	elem1.timeDate = buffer;
	elem1.children = {"Database"};
	elem2.name = "Database";elem2.category = "Executive";
	elem2.description = "Requirements are defined in this executive";
	elem2.data = "Elements.h ";elem2.timeDate = buffer;
	elem3.name = "Properties";elem3.category = "Assignment";
	elem3.description = "getting and setting properties in metadata";elem3.data = "Properties.h";
	elem3.timeDate = buffer;elem3.children = {"Database"};
	elem4.name = "Utilities";elem4.category = "Processing";
	elem4.description = "lambdas that provide mildly useful local processing";
	elem4.data = "Utilities.h";elem4.timeDate = buffer;
	elem5.name = "XMLDocument";elem5.category = "XML Processing";
	elem5.description = "Program-friendly wrapper around an Abstract Syntax Tree (AST)";
	elem5.data = "XMLDocument.h";elem5.timeDate = buffer;elem5.children = {"Database"};
	requirement2b();
	db.save(elem1.name, elem1);
	db.save(elem2.name, elem2);
	db.save(elem3.name, elem3);
	db.save(elem4.name, elem4);
	db.save(elem5.name, elem5);
	db.display(db);
	std::cout << "\n  Requirement 3-b: Deletion of key/value database pairs \n  Deleting Key/Value Pair - XMLDocument \n";
	db.remove("Database");
	db.display(db);
	std::cout << "\n  Requirement 4: Editing description data \n";
	element.name = "Convert";element.category = "Serializer";
	element.description = "(de)serializes objects of type T (from)to strings";
	element.data = "test.h";element.timeDate = buffer;
	db.editDB1(element.name,element);
	db.display(db);
	/*std::cout<<db.value("Convert").show();*/
	requirement5(db,0);
	requirement5b(db);
	std::cout << "\n Requirement 6 - Demonstration of Timed Interval for persist \n Data will be persisted every 2 seconds \n";
	int t = 0; bool check = true;
	while (check) {
		auto f = std::async(&requirement5, db, 2000);
		if (t == 4)
			check = false;
		t++;
	}
	requirementQueries(db);//Requirement 6-9
	system("PAUSE");
	return 0;
}