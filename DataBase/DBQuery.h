///////////////////////////////////////////////////////////////////
// DBQuery.h - Contains Query functions for testcases            //
// Ver 1.0                                                       //
// Application: TestExecutive for Project#1                      //
// Platform:    MacBook Pro, Win 10 Education, Visual Studio 2015//
// Author:      Syed Yaser Ahmed				                 //
//              sysyed@syr.edu		                             //
///////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This header contains query functions to be performed on NoSql database
*
*
* The functions defined in this header are used as queries by the TestExecutive.cpp
*   std::string keyvalue(NoSqlDb<Data> db, std::string k) // Return Data of a given Key
*   NoSqlDb<Data> keychild(NoSqlDb<Data> db, std::string k) //Return children of a given key
*   NoSqlDb<Data> keyname(NoSqlDb<Data> db, std::string k) //Return All keys that contain a specified string in their item name
*   NoSqlDb<Data> keydate(NoSqlDb<Data> db, std::string date1, std::string date2) //All keys that contain values written within a specified time-date interval.
*   NoSqlDb<Data> keycateg(NoSqlDb<Data> db, std::string k) // All keys that contain a specified string in their category name
*   NoSqlDb<Data> keydata(NoSqlDb<Data> db, std::string k) // All keys that contain a specified string in their template data section when that makes sense.
*   void keyregex(NoSqlDb<Data> db, std::string express) // Support regular expression matching for the query types described above.
*   NoSqlDb<Data> keyunion(NoSqlDb<Data> db8, NoSqlDb<Data> db7g) //Unions two sets of keys
*
* Required Files:
* ---------------
*   - Properties.h, Properties.cpp
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
#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <unordered_map>
#include <iomanip>
#include <sstream>
#include <regex>
#include "../Properties/Properties.h"

template <typename Data>
class DBQueries {
public:
	std::string keyvalue(NoSqlDb<Data> db, std::string k) {
		std::cout << "\n Showing Values of Key " << k << " \n";
		NoSqlDb<Data> newdb;
		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			std::string check = e.name;
			if (check.find(k) != std::string::npos)
			{
				newdb.save(e.name, e);
			}
		}
		return db.value(k).show();
	}
	NoSqlDb<Data> keychild(NoSqlDb<Data> db, Key k)
	{
		std::cout << "\n Showing Children of Key " << k << " \n";
		NoSqlDb<Data> newdb;
		metadata<Data> e;
		e = db.value(k).returns();
		std::vector<std::string> ch = e.children;
		if (ch.size() == 0)
			std::cout << "\n No Child present for this Key \n";
		for (std::vector<std::string>::const_iterator i = ch.begin(); i != ch.end(); ++i)
		{
			std::cout << "\n Child: " << *i << "\n \n";
		}
		newdb.save(e.name, e);
		return newdb;
	}
	NoSqlDb<Data> findkey(NoSqlDb<Data> db, std::string k)
	{
		NoSqlDb<Data> credb;
		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			if (n1.find(k) != std::string::npos)
			{
				e = db.value(n1).returns();
				newdb.save(e.name, e);
				std::cout << db.value(n1).show();
			}
		}
		return credb;
	}
	NoSqlDb<Data> keyname(NoSqlDb<Data> db, std::string k)
	{
		std::cout << "\n Finding Keys with the string " << k << " in their Item Name \n";
		NoSqlDb<Data> newdb;
		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			std::string check = e.name;
			if (check.find(k) != std::string::npos)
			{
				newdb.save(e.name, e);
				std::cout << newdb.value(n1).show();
			}
		}
		return newdb;
	}
	NoSqlDb<Data> keydate(NoSqlDb<Data> db, std::string date1, std::string date2)
	{
		std::cout << "\n Finding Keys between " << date1 << " and " << date2<<"\n";
		NoSqlDb<Data> newdb;
		if (date2=="")
		{
			std::time_t _tm = std::time(NULL);
			struct tm * curtime = std::localtime(&_tm);
			char buffer[32];
			std::strftime(buffer, 32, "%d:%m:%Y %H:%M:%S", curtime);
			date2 = buffer[32];
		}

		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			std::string check = e.timeDate;
			if (check > date1 && check < date2)
			{
				newdb.save(e.name, e);
				std::cout << db.value(n1).show();
			}
		}
		return newdb;
	}
	NoSqlDb<Data> keycateg(NoSqlDb<Data> db, std::string k)
	{
		std::cout << "\n Finding Keys with the string " << k << " in their Category \n";
		NoSqlDb<Data> newdb;
		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			std::string check = e.category;
			if (check.find(k) != std::string::npos)
			{
				newdb.save(e.name, e);
				std::cout << newdb.value(n1).show();
			}
		}
		return newdb;
	}
	NoSqlDb<Data> keydata(NoSqlDb<Data> db, std::string k)
	{
		std::cout << "\n Finding Keys with the string " << k << " in their Data \n";
		NoSqlDb<Data> newdb;
		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			std::string check = e.data;
			if (check.find(k) != std::string::npos)
			{
				newdb.save(e.name, e);
				std::cout << newdb.value(n1).show();
			}
		}
		return newdb;
	}
	metadata<Data> editDB(metadata<Data> db, std::string meta, std::string nvalue)
	{
		if (meta == "data") {
			db.data = nvalue;
		}
		else if (meta == "description") {
			db.description = nvalue;
		}
		else
			std::cout << "Requirement is to only edit data and description";

		return db;
	}
	NoSqlDb<Data> keyregex(NoSqlDb<Data> db, std::string express)
	{
		NoSqlDb<Data> credb;
		std::regex pattern(express);
		Keys n = db.keys();
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			std::string pat = e.name;
			if (std::regex_match(pat, std::regex(express))) {
				std::cout << db.value(n1).show();
				credb.save(e.name, e);
			}
		}
		return credb;
	}
	NoSqlDb<Data> keyunion(NoSqlDb<Data> db8, NoSqlDb<Data> db7g)
	{
		NoSqlDb<Data> db9;
		Keys uk = db8.keys();
		for (Key k : uk)
		{
			db9.save(db8.value(k).name, db8.value(k));
		}
		for (Key k : db7g.keys())
		{
			db9.save(db7g.value(k).name, db7g.value(k));
		}
		return db9;
	}
	NoSqlDb<Data> BonusQuery(NoSqlDb<Data> db, std::string meta, std::string express)
	{
		std::cout << "\n Searching for " << express << " in " << meta << " \n";
		std::regex pattern(express);
		NoSqlDb<Data> bdb;
		Keys n = db.keys();
		std::string pat;
		for (Key n1 : n)
		{
			metadata<Data> e;
			e = db.value(n1).returns();
			if (meta == "name")
				pat = e.name;
			else if (meta == "category")
				pat = e.category;
			else if (meta == "description")
				pat = e.description;
			else if (meta == "timeDate")
				pat = e.timeDate;
			else
				pat = e.data;

				if (std::regex_match(pat, std::regex(express)))
				{
					std::cout << db.value(n1).show();
					bdb.save(e.name,e);
				}
		}
		return bdb;
	}
};