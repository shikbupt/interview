#ifndef QIHOO_INI_PARSER_H_
#define QIHOO_INI_PARSER_H_

#include <string>
#include <map>
#include <vector>


namespace qh
{
	using std::map;
	using std::string;
	using std::vector;
    class INIParser
    {
    public:
        INIParser();
        ~INIParser();

        //! \brief ����һ�������ϵ�INI�ļ�
        //! \param[in] - const std::string & ini_file_path
        //! \return - bool
        bool Parse(const std::string& ini_file_path);

        //! \brief ����һ������INI��ʽ���ڴ����ݡ�
        //!   ���磺ini_data="a:1||b:2||c:3"
        //!         ����<code>Parse(ini_data, ini_data_len, "||", ":")</code>���ɽ�����������ݡ�
        //!         �������֮�� 
        //!         Get("a")=="1" && Get("b")=="2" && Get("c")=="3"
        //! \param[in] - const char * ini_data
        //! \param[in] - size_t ini_data
        //! \param[in] - const std::string & line_seperator
        //! \param[in] - const std::string & key_value_seperator
        //! \return - bool
        bool Parse(const char* ini_data, size_t ini_data_len, const std::string& line_seperator = "\n", const std::string& key_value_seperator = "=");

        //! \brief ��Ĭ��section�в���ĳ��key���������ҵ���value������Ҳ���������һ���մ�
        //! \param[in] - const std::string & key
        //! \param[in] - bool * found - ���������true�����ҵ����key
        //! \return - const std::string& - ���صľ���key��Ӧ��value
        const std::string& Get(const std::string& key, bool* found);

        const std::string& Get(const std::string& section, const std::string& key, bool* found);

    private:

		//! \brief ��ini_data����ȡÿһ�з���ini_data��
		void GetINIDataLine(const string& ini_data, vector<string>& line_data, const std::string& line_seperator);

		//! \brief ����ini_data_struct_�ṹ
		bool GetINIDataStruct(const vector<string>& line_data, const std::string& key_value_seperator);

		typedef map<string, map<string, string> > INI_DATA_STRUCT;
		INI_DATA_STRUCT ini_data_struct_;
		string* no_found_str;
    };
} 

#endif

