#include "Json.h"
#include <boost/locale.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "boost/program_options/detail/utf8_codecvt_facet.hpp"

/*#include <iostream>
using namespace std;*/

Json::Json()
{
	pt = 0; //2019-12-25
	pt = new boost::property_tree::ptree();

#ifdef _SUPPORT_WIDE_OBJECT
	wpt = 0;
#endif
}

Json::Json(const string &buf, bool wide)
{
	pt = 0; //2019-12-25

	string buf2 = Json::encode(buf);

#ifdef _SUPPORT_WIDE_OBJECT
	if (wide)
	{
		pt = 0;
		wpt = new boost::property_tree::wptree();

		if (buf2.length() <= 0) return;
		wstringstream ss(std::wstring(buf2.begin(), buf2.end()));
		try{
			boost::property_tree::json_parser::read_json(ss, *wpt);
		}
		catch (boost::property_tree::ptree_error & e) {
			//LogWarn(0, "construction Json buf:" << buf << " ptree_error exception:" << e.what());
			return;
		}
		catch (std::exception& e) {
			//LogWarn(0, "construction Json buf:" << buf << " exception:" << e.what());
			return;
		}
		catch (...) {
			//LogWarn(0, "construction Json buf:" << buf << " exception.");
			return;
		}
	}
	else
	{
		wpt = 0;
		pt = new boost::property_tree::ptree();

		if (buf2.length() <= 0) return;
		stringstream ss(buf2);
		try{
			boost::property_tree::json_parser::read_json(ss, *pt);
		}
		catch (boost::property_tree::ptree_error & e) {
			//LogWarn(0, "construction Json buf:" << buf << " ptree_error exception:" << e.what());
			return;
		}
		catch (std::exception& e) {
			//LogWarn(0, "construction Json buf:" << buf << " exception:" << e.what());
			return;
		}
		catch (...) {
			//LogWarn(0, "construction Json buf:" << buf << " exception.");
			return;
		}
	}
#else
	//wpt = 0;
	pt = new boost::property_tree::ptree();

	if (buf.length() <= 0) return;
	stringstream ss(buf);
	try{
		boost::property_tree::json_parser::read_json(ss, *pt);
	}
	catch (boost::property_tree::ptree_error & e) {
		//LogWarn(0, "construction Json buf:" << buf << " ptree_error exception:" << e.what());
		return;
	}
	catch (std::exception& e) {
		//LogWarn(0, "construction Json buf:" << buf << " exception:" << e.what());
		return;
	}
	catch (...) {
		//LogWarn(0, "construction Json buf:" << buf << " exception.");
		return;
	}
#endif
}

Json::~Json()
{
	try{ //2019-10-24
#ifdef _SUPPORT_WIDE_OBJECT
		if (wpt){
			delete wpt;
			wpt = NULL;
		}
#endif

		if (pt){
			delete pt;
			pt = NULL;
		}
	}
	catch (std::exception& e) {
		//LogWarn(0, "destruct Json exception:" << e.what());
	}
	catch (...) {
		//LogWarn(0, "destruct Json exception.");
	}
}

void Json::put(const string &name, const string &data)
{
	try {
		if (pt) {
			pt->put(name, data);
		}
		else {
#ifdef _SUPPORT_WIDE_OBJECT
			wpt->put(std::wstring(name.begin(), name.end()), std::wstring(data.begin(), data.end()));
#endif
		}
	}
	catch (std::exception& e) {
		//LogWarn(0, "put name:" << name << " string data:" << data << " Json exception:" << e.what());
	}
	catch (...) {
		//LogWarn(0, "put name:" << name << " string data:" << data << " Json exception.");
	}
}

string Json::toString()
{
	string ret;

	try{
		if (pt){
			std::stringstream ss;
			boost::property_tree::json_parser::write_json(ss, *pt, false);
			ret = ss.str();
		}else{
#ifdef _SUPPORT_WIDE_OBJECT
			std::wstringstream ss;
			boost::property_tree::json_parser::write_json(ss, *wpt, false);
			wstring str = ss.str();
			ret = std::string(str.begin(), str.end());
			return ret;
#endif
		}
	}
	catch (std::exception& e) {
		//LogWarn(0, "toString write_json exception:" << e.what());
	}
	catch (...) {
		//LogWarn(0, "toString write_json exception.");
	}
	return ret;
}





string Json::getString(const string &name) const
{
	try{
		if (pt){
			string rt = Json::decode(pt->get<string>(name));
			return rt;
		}else{
#ifdef _SUPPORT_WIDE_OBJECT
			std::wstring ws;
			ws = wpt->get<wstring>(std::wstring(name.begin(), name.end()));
			if (ws.size() != 0){
				string rt = Json::decode(std::string(ws.begin(), ws.end()).c_str());
				return rt;
			}else{
				boost::property_tree::wptree obj;
				obj = wpt->get_child(std::wstring(name.begin(), name.end()));

				if (obj.empty()){
					return "";
				}

				std::wstringstream ss;
				boost::property_tree::json_parser::write_json(ss, obj, false);
				wstring str = ss.str();
				string rt = Json::decode(std::string(str.begin(), str.end()));
				return rt;
			}
#endif
		}
	}
	catch (std::exception& e) {
		////LogWarn(0, "getString name:" << name << " exception:" << e.what());
		return "";
	}
	catch (...){
		////LogWarn(0, "getString name:" << name << " exception.");
		return "";
	}

	return "";
}










string Json::encode(const string &s)
{
	string s1;
	int size = (int)s.size();
	for (int i = 0; i < size; i++) {
		unsigned char t = (unsigned char)s[i];
		if (t > 127) {
			char tmp[10];
			sprintf(tmp, "%%%02x", t);
			s1.append(tmp);
		}
		else {
			s1.append(1, s[i]);
		}
	}
	return s1;
}

string Json::encode2(const string &s)
{
	string s1;
	int size = (int)s.size();
	int first = 0;
	for (int i = 0; i < size; i++) {
		unsigned char t = (unsigned char)s[i];
		if (t > 127 && first == 0) {
			char tmp[10];
			sprintf(tmp, "\\u%02x", t);
			s1.append(tmp);
			first = 1;
		}else if (first == 1){
			char tmp[10];
			sprintf(tmp, "%02x", t);
			s1.append(tmp);
			first = 0;
		} else {
			s1.append(1, s[i]);
		}
	}
	return s1;
}

#define char2hex(c) ((c>='0' && c<='9')?(c - '0'):((c>='a' && c<='z')?(c - 'a' + 10):(c - 'A' + 10)))
string Json::decode(const string &s)
{
	string s1;

	int size = (int)s.size();
	for (int i = 0; i < size; i++){
		if (s[i] == '%' && (i + 1) < size && s[i + 1] == '%'){
			s1.append(1, s[i]);
			i++;
		}else if (s[i] == '%' && (i + 2) < size){
			char tmp;
			tmp = (char2hex(s[i + 1]) << 4);
			tmp += char2hex(s[i + 2]);
			s1.append(1, tmp);
			i += 2;
		}else{
			s1.append(1, s[i]);
		}
	}

	return s1;
}

string Json::decode2(const string &s)
{
	string s1;
	int size = (int)s.size();
	for (int i = 0; i < size; i++) {
		if (s[i] == '\\' && (i + 5) < size && s[i + 1] == 'u') {
			char tmp;
			tmp = (char2hex(s[i + 2]) << 4);
			tmp += char2hex(s[i + 3]);
			s1.append(1, tmp);

			tmp = (char2hex(s[i + 4]) << 4);
			tmp += char2hex(s[i + 5]);
			s1.append(1, tmp);
			i += 5;
		} else {
			s1.append(1, s[i]);
		}
	}
	return s1;
}

string Json::rid(const string &s, char c)
{
	string s1;
	int size = (int)s.size();
	for (int i = 0; i < size; i++){
		if (s[i] != c && s[i] != ' ' && s[i] != '\t'){
			s1.append(1, s[i]);
		}
	}
	return s1;
}

string Json::getString(const boost::property_tree::wptree &pt, const string &name)
{
	try {
		std::wstring ws;
		ws = pt.get<wstring>(std::wstring(name.begin(), name.end()));
		if (ws.size() != 0) {
			return std::string(ws.begin(), ws.end()).c_str();
		}
		else {
			boost::property_tree::wptree obj;
			obj = pt.get_child(std::wstring(name.begin(), name.end()));
			if (obj.empty()) {
				return "";
			}
			std::wstringstream ss;
			boost::property_tree::json_parser::write_json(ss, obj, false);
			wstring str = ss.str();
			return std::string(str.begin(), str.end());
		}
	}
	catch (std::exception& e) {
		////LogWarn(0, "staic getString name:" << name << " exception:" << e.what());
		return "";
	}
	catch (...) {
		////LogWarn(0, "staic getString name:" << name << " exception.");
		return "";
	}

	return "";
}

int Json::getInt(const boost::property_tree::wptree &pt, const string &name) {
	try {
		return pt.get<int>(std::wstring(name.begin(), name.end()));
	}
	catch (std::exception& e) {
		////LogWarn(0, "staic getInt name:" << name << " exception:" << e.what());
		return 0;
	}
	catch (...) {
		////LogWarn(0, "staic getInt name:" << name << " exception.");
		return 0;
	}
	return 0;
}

string Json::toString(const boost::property_tree::ptree &pt)
{
#if 0
	string ret;

	try {
		std::stringstream ss;
		boost::property_tree::json_parser::write_json(ss, pt, false);
		ret = ss.str();
	}
	catch (std::exception& e) {
		//LogWarn(0, "toString write_json exception:" << e.what());
	}
	catch (...) {
		//LogWarn(0, "toString write_json exception.");
	}
	return ret;
#else
	Json json;
	json.putObject("name", pt);
	string rowsString = json.toString();
	size_t pos = rowsString.find("\"name\":");
	if (pos != string::npos) {
		rowsString = rowsString.substr(pos + 7);
	}
	pos = rowsString.find_last_of("}");
	if (pos != string::npos) {
		rowsString = rowsString.substr(0, pos);
	}
	return rowsString;
#endif
}

string Json::toString(const boost::property_tree::wptree &pt)
{
#if 0
	string ret;

	try {
		std::wstringstream ss;
		boost::property_tree::json_parser::write_json(ss, pt, false);
		wstring str = ss.str();
		return std::string(str.begin(), str.end());
	}
	catch (std::exception& e) {
		//LogWarn(0, "write_json exception:" << e.what());
	}
	catch (...) {
		//LogWarn(0, "write_json exception.");
	}
	return ret;
#else
	Json json("", true);
	json.putObject("name", pt);
	string rowsString = json.toString();
	size_t pos = rowsString.find("\"name\":");
	if (pos != string::npos) {
		rowsString = rowsString.substr(pos + 7);
	}
	pos = rowsString.find_last_of("}");
	if (pos != string::npos) {
		rowsString = rowsString.substr(0, pos);
	}
	return rowsString;
#endif
}
