#include "ini_parser.h"

#include <assert.h>

void test1()
{
    const char* ini_text= "a=1\nb=2\n"; 
    qh::INIParser parser;
    if (!parser.Parse(ini_text, strlen(ini_text), "\n", "=")) {
        assert(false);
    }
	bool found;
    const std::string& a = parser.Get("a", &found);
    assert(a == "1");

    std::string b = parser.Get("b", &found);
    assert(b == "2");

    const std::string& c = parser.Get("c", &found);
    assert(c == "");
}

void test2()
{
    const char* ini_text= "a=1||b=2||c=3"; 
    qh::INIParser parser;
    if (!parser.Parse(ini_text, strlen(ini_text), "||", "=")) {
        assert(false);
    }
	bool found;
    const std::string& a = parser.Get("a", &found);
    assert(a == "1");

    std::string b = parser.Get("b", &found);
    assert(b == "2");

    const std::string& c = parser.Get("c", &found);
    assert(c == "3");
}

void test3()
{
    const char* ini_text= "||||a:1||b:2||||c:3||||||"; 
    qh::INIParser parser;
    if (!parser.Parse(ini_text, strlen(ini_text), "||", ":")) {
        assert(false);
    }
	bool found;
    const std::string& a = parser.Get("a", &found);
    assert(a == "1");

    std::string b = parser.Get("b", &found);
    assert(b == "2");

    const std::string& c = parser.Get("c", &found);
    assert(c == "3");
}

void test4()
{
	const char* ini_text= "[section1]\na=1\nb=2\n[section2];comment\nc=3\nd=4"; 
	qh::INIParser parser;
	if (!parser.Parse(ini_text, strlen(ini_text), "\n", "=")) {
		assert(false);
	}
	bool found;
	const std::string& a = parser.Get("section1", "a", &found);
	assert(a == "1");

	std::string b = parser.Get("section1", "b", &found);
	assert(b == "2");

	const std::string& c = parser.Get("section2","c", &found);
	assert(c == "3");

	const std::string& d = parser.Get("section2","d", &found);
	assert(d == "4");
}

void test5()
{
	qh::INIParser parser;
	if (!parser.Parse("test.ini")) {
		assert(false);
	}
	bool found;
	const std::string& a = parser.Get("section1", "a", &found);
	assert(a == "1");

	std::string b = parser.Get("section1", "b", &found);
	assert(b == "2");

	const std::string& c = parser.Get("section2","c", &found);
	assert(c == "3");

	const std::string& d = parser.Get("section2","d", &found);
	assert(d == "4");
}

int main(int argc, char* argv[])
{
    //TODO 在这里添加单元测试，越多越好，代码路径覆盖率越全越好

    test1();
    test2();
    test3();
	test4();
	test5();

    return 0;
}


