
#include "proxy_url_extractor.h"
#include <fstream>
#include <vector>
#include "tokener.h"

namespace qh
{

    namespace {

        template< class _StringVector, 
        class StringType,
        class _DelimType> 
            inline void StringSplit(  
            const StringType& str, 
            const _DelimType& delims, 
            unsigned int maxSplits, 
            _StringVector& ret)
        {
            unsigned int numSplits = 0;

            // Use STL methods
            size_t start, pos;
            start = 0;

            do
            {
                pos = str.find_first_of( delims, start );

                if ( pos == start )
                {
                    ret.push_back(StringType());
                    start = pos + 1;
                }
                else if ( pos == StringType::npos || ( maxSplits && numSplits + 1== maxSplits ) )
                {
                    // Copy the rest of the string
                    ret.push_back(StringType());
                    *(ret.rbegin()) = StringType(str.data() + start, str.size() - start);
                    break;
                }
                else
                {
                    // Copy up to delimiter
                    //ret.push_back( str.substr( start, pos - start ) );
                    ret.push_back(StringType());
                    *(ret.rbegin()) = StringType(str.data() + start, pos - start);
                    start = pos + 1;
                }

                ++numSplits;

            }
            while ( pos != StringType::npos );
        }
    }

    ProxyURLExtractor::ProxyURLExtractor()
    {
    }

    bool ProxyURLExtractor::Initialize( const std::string& param_keys_path )
    {
        std::ifstream ifs;
        ifs.open(param_keys_path.data(), std::fstream::in);
        typedef std::vector<std::string> stringvector;
        stringvector keysvect;
        
        while (!ifs.eof()) {
            std::string line;
            getline(ifs, line);
            if (ifs.fail() && !ifs.eof()) {
                fprintf(stderr, "SubUrlExtractor::LoadParamKeysFile readfile_error=[%s] error!!", param_keys_path.data());
                ifs.close();
                return false;
            }
            if (line.empty()) continue;

            keysvect.clear();
            StringSplit(line, ",", static_cast<unsigned int>(-1), keysvect);
            assert(keysvect.size() >= 1);
            keys_set_.insert(keysvect.begin(), keysvect.end());
            keys_set_.erase("");
        }

        ifs.close();

        return true;
    }

    std::string ProxyURLExtractor::Extract( const std::string& raw_url )
    {
        std::string sub_url;
        ProxyURLExtractor::Extract(keys_set_, raw_url, sub_url);
        return sub_url;
    }

	void GetEqualsPositions(const std::string& raw_url, std::vector<int>& positions)
	{
		char* equals = "=";
		for (std::string::size_type index = raw_url.size()-1; index != 0; --index) {
			if (!strncmp(&raw_url[index], equals, 1)) {
				positions.push_back(index);
			}
		}
	}

	int FindForwardMaxAlphaSegment(const std::string& raw_url, int equals_position)
	{
		int max_position = equals_position;
		for (std::string::size_type index = equals_position-1; index != 0; --index) {
			if (isalpha(raw_url[index])) {
				max_position = index;
			}
			else{
				break;
			}
		}
		return max_position;
	}

	void ExtractSubUrl(const std::string& raw_url, int equals_position, std::string& sub_url)
	{
		char* ends = "&";
		for (std::string::size_type index = equals_position+1; index != raw_url.size(); ++index) {
			if (!strncmp(&raw_url[index], ends, 1)) {
				sub_url = raw_url.substr(equals_position+1, index-equals_position-1);
				return;
			}
		}
		sub_url = raw_url.substr(equals_position+1);
	}

    void ProxyURLExtractor::Extract( const KeyItems& keys, const std::string& raw_url, std::string& sub_url )
    {
#if 1
        //TODO 请面试者在这里添加自己的代码实现以完成所需功能
		//先寻找“=”所在的位置，按出现位置向前寻找最大的字母段，将字母段与keys中关键词比较
		//发现关键词再以“=”所在的位置为中心向后查找，直到出现“&”或结尾，此时提取即为suburl。
		if (raw_url.empty() || keys.empty()) {
			//printf("raw_url/keys 无输入");
			return;
		}
		using std::vector;
		vector<int> equals_positions;
		GetEqualsPositions(raw_url, equals_positions);
		if (equals_positions.empty()) {
			return;
		} 		
		for (vector<int>::iterator iter = equals_positions.begin(); 
			iter != equals_positions.end(); ++iter) {
				int alpha_segment_pos = FindForwardMaxAlphaSegment(raw_url, *iter);
				if (keys.count(raw_url.substr(alpha_segment_pos, *iter-alpha_segment_pos)) != 0) {
					ExtractSubUrl(raw_url, *iter, sub_url);
					if (!sub_url.empty()) {
						return;
					}
				}
		}
		
#else
        //这是一份参考实现，但在特殊情况下工作不能符合预期
        Tokener token(raw_url);
        token.skipTo('?');
        token.next(); //skip one char : '?' 
        std::string key;
        while (!token.isEnd()) {
            key = token.nextString('=');
            if (keys.find(key) != keys.end()) {
                const char* curpos = token.getCurReadPos();
                int nreadable = token.getReadableSize();

                /**
                * case 1: 
                *  raw_url="http://www.microsofttranslator.com/bv.aspx?from=&to=zh-chs&a=http://hnujug.com/&xx=yy"
                *  sub_url="http://hnujug.com/"
                */
                sub_url = token.nextString('&');

                if (sub_url.empty() && nreadable > 0) {
                    /**
                    * case 2: 
                    * raw_url="http://www.microsofttranslator.com/bv.aspx?from=&to=zh-chs&a=http://hnujug.com/"
                    * sub_url="http://hnujug.com/"
                    */
                    assert(curpos);
                    sub_url.assign(curpos, nreadable);
                }
            }
            token.skipTo('&');
            token.next();//skip one char : '&'
        }
#endif
    }

    std::string ProxyURLExtractor::Extract( const KeyItems& keys, const std::string& raw_url )
    {
        std::string sub_url;
        ProxyURLExtractor::Extract(keys, raw_url, sub_url);
        return sub_url;
    }
}

