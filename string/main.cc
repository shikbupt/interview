#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "qh_string.h"
using qh::string;

void test1(){
	char* s = "1234";
	string s1;
	assert(s1.size() == 0);
	assert(s1.c_str() == NULL);

	string s2(s);
	assert(s2.size() == 4);
	assert(!strcmp(s2.c_str(), s));

	string s3(s2);
	assert(s3.size() == s2.size());
	assert(!strcmp(s3.c_str(), s2.c_str()));

	string s4(s, 2);
	assert(s4.size() == 2);
	assert(!strcmp(s4.c_str(), "12"));

	s4 = s2;
	assert(s4.size() == s2.size());
	assert(!strcmp(s4.c_str(), s2.c_str()));

	string s5(s, 15);
	assert(s5.size() == 4);
	assert(!strcmp(s5.c_str(), "1234"));
}

void test2(){
	char* s = "";

	string s2(s);
	assert(s2.size() == 0);
	assert(!strcmp(s2.c_str(), s));

	string s3(s2);
	assert(s3.size() == s2.size());
	assert(!strcmp(s3.c_str(), s2.c_str()));

	string s4(s, 2);
	assert(s4.size() == 0);
	assert(!strcmp(s4.c_str(), s));

	s4 = s2;
	assert(s4.size() == s2.size());
	assert(!strcmp(s4.c_str(), s2.c_str()));
}

void test3(){
	char* s = NULL;

	string s2(s);
	assert(s2.size() == 0);
	assert(s2.c_str() == NULL);

	string s3(s2);
	assert(s3.size() == s2.size());
	assert(s3.c_str() == s2.c_str());

	string s4(s, 2);
	assert(s4.size() == 0);
	assert(s4.c_str() == s);

	s4 = s2;
	assert(s4.size() == s2.size());
	assert(s4.c_str() == s2.c_str());
}

void test4(){
	char*s = "1234";
	string s1(s);

	for (int i = 0; i < s1.size(); ++i) {
		assert(s1[i] == s[i]);
		s1[i] = '9';
	}
	assert(!strcmp(s1.c_str(), "9999"));
}
int main(int argc, char* argv[])
{
    //TODO 在这里添加单元测试，越多越好，代码路径覆盖率越全越好
    //TODO 单元测试写法请参考INIParser那个项目，不要写一堆printf，要用assert进行断言判断。
	test1();
	test2();
	test3();
	test4();
#ifdef WIN32
    system("pause");
#endif

	return 0;
}

