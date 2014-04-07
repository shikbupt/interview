#include "qh_string.h"

#include <string.h>

namespace qh
{
    // TODO 将在这里写实现代码

    string::string()
        : data_(NULL), len_(0), no_null_data_(NULL)
    {
    }

    string::string( const char* s )
    {
		if (s == NULL) {
			data_ = NULL;
			len_ = 0;
			no_null_data_= NULL;
		} 
		else {
			len_ = strlen(s);
			no_null_data_ = NULL;			
			data_ = new char[5];
			strcpy(data_, s);

		}		
    }

    string::string( const char* s, size_t len )
    {
		if (s == NULL) {
			data_ = NULL;
			len_ = 0;
			no_null_data_= NULL;
		}
		else{
			no_null_data_ = NULL;
			len_ = strlen(s)<len ? strlen(s) : len;
			data_ = new char[len_+1];  
			strncpy(data_, s, len_);
			data_[len_] = '\0';
	
		}		
    }

    string::string( const string& rhs )
    {
		no_null_data_ = NULL;
		len_ = rhs.len_;
		if(!rhs.data_) {
			data_ = NULL;
		}
		else
		{
			data_ = new char[len_+1];  
			strncpy(data_, rhs.data_, len_);
			data_[len_] = '\0';
		}
    }

    string& string::operator=( const string& rhs )
    {
		if (this != &rhs) {
			if (data_ != NULL) {
				delete[] data_;
			}
			if (no_null_data_ != NULL) {
				delete[] no_null_data_;
			}
			if (!rhs.data_) {
				len_ = 0;
				data_ = NULL;
				no_null_data_ = NULL;
			} 
			else {
				len_ = rhs.len_;
				data_ = new char[len_+1];
				no_null_data_ = NULL;
				strncpy(data_, rhs.data_, len_);
				data_[len_] = '\0';
			}
		}
        return *this;
    }

    string::~string()
    {
		if (data_ != NULL) {
			delete[] data_;
		}
		if (no_null_data_ != NULL) {
			delete[] no_null_data_;
		}
    }

    size_t string::size() const
    {
        return len_;
    }

    const char* string::data() const
    {
		no_null_data_ = new char[len_];
		strncpy(no_null_data_, data_, len_);
        return no_null_data_;
    }

    const char* string::c_str() const
    {
        return data_;
    }

    char& string::operator[]( size_t index )
    {
		if (index>=0  && index<=strlen(data_)) {
			return data_[index];
		}
    }
}
