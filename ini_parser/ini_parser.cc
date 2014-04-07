#include "ini_parser.h"
#include <vector>
#include <string>
#include <fstream>
#include <streambuf>

namespace qh
{
	using std::vector;
	using std::string;
	using std::ifstream;
	INIParser::INIParser() : no_found_str(new string()) {};

	INIParser::~INIParser() { delete no_found_str;};

	bool INIParser::Parse(const char* ini_data, size_t ini_data_len, 
						  const std::string& line_seperator, 
						  const std::string& key_value_seperator) {
		vector<string> line_data;
		line_data.push_back("[default_system]");
		const string ini_data_str(ini_data);
		if (ini_data_str.empty()) {
			return 0;
		}
		GetINIDataLine(ini_data_str, line_data, line_seperator);
		if (line_data.empty()) {
			return 0;
		}
		bool result = GetINIDataStruct(line_data, key_value_seperator);		
		return result;
	}


	bool INIParser::Parse(const std::string& ini_file_path) {
		ifstream in(ini_file_path.c_str(), std::ios::in);
		if (!in) {
			return 0;
		}
		std::istreambuf_iterator<char> beg(in), end;
		string strdata(beg, end);
		in.close();		
		return Parse(strdata.c_str(), strdata.size());
	}

	const string& INIParser::Get(const std::string& section, const std::string& key, bool* found) {
		INI_DATA_STRUCT::iterator iter = ini_data_struct_.find(section);
		if (iter == ini_data_struct_.end()) {
			*found = false;
			return *(no_found_str);
		}
		map<string, string>::iterator iter_key = iter->second.find(key);
		if (iter_key == iter->second.end()) {
			*found = false;
			return *(no_found_str);
		}
		*found = true;
		return iter_key->second;
	}

	const string& INIParser::Get(const std::string& key, bool* found) {
		return Get("default_system", key, found);
	}
	
	void INIParser::GetINIDataLine(const string& ini_data_str, 
								   vector<string>& line_data, 
								   const std::string& line_seperator) {
		int first_noseperator_position = ini_data_str.find_first_not_of(line_seperator);
		int line_seperator_position = 0;
		//以line_seperator为分行符，提取每行放入line_data中。
		while (line_seperator_position != string::npos) {
			line_seperator_position = ini_data_str.find(line_seperator, first_noseperator_position);
			int size = line_seperator_position - first_noseperator_position;
			if (size > 0) {
				line_data.push_back(ini_data_str.substr(first_noseperator_position, size));
			}
			else if(line_seperator_position == -1 && first_noseperator_position != ini_data_str.size()){
				line_data.push_back(ini_data_str.substr(first_noseperator_position));
			}
			first_noseperator_position = line_seperator_position + line_seperator.size();
		}
	}

	bool INIParser::GetINIDataStruct(const vector<string>& line_data, const std::string& key_value_seperator) {
		string section;
		map<string, string> key_value;
		for (vector<string>::const_iterator iter = line_data.begin(); iter != line_data.end(); ++iter) {
			//注释部分
			string::size_type upper_boundary = iter->find(';');
			if (upper_boundary == 0) continue;		
			//section部分
			if ((*iter)[0] == '[') {
				if (!section.empty()) {
					ini_data_struct_.insert(make_pair(section, key_value));
					section.clear();
					key_value.clear();
				}
				int right_bracket_position = iter->substr(0, upper_boundary).find(']');
				section = iter->substr(1, right_bracket_position-1);
				if (ini_data_struct_.count(section) != 0) {
					return 0;
				}				
				continue;
			}
			//key&value部分
			if (!section.empty()) {
				int equals_position = iter->substr(0, upper_boundary).find(key_value_seperator.c_str());
				string key = iter->substr(0, equals_position);
				string value = iter->substr(equals_position+1, upper_boundary-equals_position-1);
				if (key_value.count(key) != 0) {
					return 0;
				}
				key_value.insert(make_pair(key, value));
			}
		}
		ini_data_struct_.insert(make_pair(section, key_value));
		return 1;
	}
}