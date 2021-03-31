#pragma once

#ifndef BOOST_SPIRIT_THREADSAFE
#define BOOST_SPIRIT_THREADSAFE
#endif

#include <boost/property_tree/ptree.hpp>
#include <string>
#include <time.h>

#include <iostream>
using namespace std;


#define _SUPPORT_WIDE_OBJECT

class Json
{
public:
	Json();
	Json(const string &buf, bool wide = true);
	~Json();

	void put(const string &name, const string &data);	//

	void put(const string &name, int data){				//
		try{
			if (pt) {
				pt->put(name, data);
			}
			else {
#ifdef _SUPPORT_WIDE_OBJECT
				if (wpt) { //2019-12-25
					wpt->put(std::wstring(name.begin(), name.end()), data);
				}
#endif
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "put name:" << name << " int data:" << data << " Json exception:" << e.what());
		}
		catch (...) {
			//LogWarn(0, "put name:" << name << " int data:" << data << " Json exception.");
		}
	}

	void put(const string &name, unsigned long data){	//
		try{
			if (pt) {
				pt->put(name, data);
			}
			else {
#ifdef _SUPPORT_WIDE_OBJECT
				if (wpt) { //2019-12-25
					wpt->put(std::wstring(name.begin(), name.end()), data);
				}
#endif
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "put name:" << name << " ulong data:" << data << " Json exception:" << e.what());
		}
		catch (...) {
			//LogWarn(0, "put name:" << name << " ulong data:" << data << " Json exception.");
		}
	}

	void put(const string &name, time_t data){			//
		try {
			if (pt) {
				pt->put(name, data);
			}
			else {
#ifdef _SUPPORT_WIDE_OBJECT
				if (wpt) { //2019-12-25
					wpt->put(std::wstring(name.begin(), name.end()), data);
				}
#endif
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "put name:" << name << " time_t data:" << data << " Json exception:" << e.what());
		}
		catch (...) {
			//LogWarn(0, "put name:" << name << " time_t data:" << data << " Json exception.");
		}
	}

	int getInt(const string &name) const {		//
		try {
			if (pt) {
				return pt->get<int>(name);
			}
			else {
#ifdef _SUPPORT_WIDE_OBJECT
				if (wpt) { //2019-12-25
					return wpt->get<int>(std::wstring(name.begin(), name.end()));
				}
#endif
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "getInt get name" << name << " Json exception:" << e.what());
			return 0;
		}
		catch (...) {
			//LogWarn(0, "getInt get name" << name << " Json exception.");
			return 0;
		}
		return 0;
	}





	void putObject(const string &name, const boost::property_tree::ptree &object){	
		try{
			if (pt){
				boost::property_tree::ptree &pt2 = pt->get_child(name);
				pt2 = object;
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "putObject get_child name:" << name << " Json exception:" << e.what());

			try {
				if (pt) { //2019-12-25
					pt->push_back(std::make_pair(name, object));
				}
			}
			catch (std::exception& e) {
				//LogWarn(0, "putObject push_back name" << name << " Json exception:" << e.what());
			}
			catch (...) {
				//LogWarn(0, "putObject push_back name" << name << " Json exception.");
			}
		}
		catch (...) {
			//LogWarn(0, "putObject get_child name:" << name << " Json exception.");

			try{
				if(pt){ //2019-12-25
					pt->push_back(std::make_pair(name, object));
				}
			}
			catch (std::exception& e) {
				//LogWarn(0, "putObject push_back name" << name << " Json exception:" << e.what());
			}
			catch(...){
				//LogWarn(0, "putObject push_back name" << name << " Json exception.");
			}
		}
	}

	boost::property_tree::ptree getObject(const string &name) const {				
		boost::property_tree::ptree obj;
		try {
			if (pt) {
				return pt->get_child(name);
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "getObject get_child name:" << name << " Json exception:" << e.what());
			return obj;
		}
		catch (...) {
			//LogWarn(0, "getObject get_child name:" << name << " Json exception.");
			return obj;
		}
		return obj;
	}

	const boost::property_tree::ptree& getObject() const {							
		return *pt;
	}

	boost::property_tree::ptree *getObjectPtr(const string &name) const {			
		boost::property_tree::ptree *obj = 0;
		try {
			if (pt) {
				return &(pt->get_child(name));
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "getObjectPtr get_child name:" << name << " Json exception:" << e.what());
			return obj;
		}
		catch (...) {
			//LogWarn(0, "getObjectPtr get_child name:" << name << " Json exception.");
			return obj;
		}
		return obj;
	}





#ifdef _SUPPORT_WIDE_OBJECT
	void putObject(const string &name, const boost::property_tree::wptree &object){	
		try{
			if (!pt){
				if (wpt) { //2019-12-25
					wpt->push_back(std::make_pair(std::wstring(name.begin(), name.end()), object));
				}
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "putObject push_back name:" << name << " wptree Json exception:" << e.what());
		}
		catch (...){
			//LogWarn(0, "putObject push_back name:" << name << " wptree Json exception.");
		}
	}
#endif

#ifdef _SUPPORT_WIDE_OBJECT
	boost::property_tree::wptree getWObject(const string &name) const{				
		boost::property_tree::wptree obj;
		try{
			if (!pt){
				if (wpt) { //2019-12-25
					return wpt->get_child(std::wstring(name.begin(), name.end()));
				}
			}
		}
		catch (std::exception& e) {
			//LogWarn(0, "getWObject get_child name:" << name << " Json exception:" << e.what());
			return obj;
		}
		catch (...){
			//LogWarn(0, "getWObject get_child name:" << name << " Json exception.");
			return obj;
		}
		return obj;
	}
#endif	

#ifdef _SUPPORT_WIDE_OBJECT
	const boost::property_tree::wptree& getWObject() const{							
		return *wpt;
	}
#endif
	

	string getString(const string &name) const;	//
	string toString();							//


	static string encode(const string &str);
	static string decode(const string &str);
	static string encode2(const string &str);
	static string decode2(const string &str);
	static string rid(const string &str, char c);
	static string getString(const boost::property_tree::wptree &pt, const string &name);
	static int	  getInt(const boost::property_tree::wptree &pt, const string &name);
	static string toString(const boost::property_tree::ptree &pt);
	static string toString(const boost::property_tree::wptree &pt);

private:
	boost::property_tree::ptree *pt;

#ifdef _SUPPORT_WIDE_OBJECT
	boost::property_tree::wptree *wpt;
#endif
};

