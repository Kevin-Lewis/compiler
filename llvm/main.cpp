#include <iostream>

extern "C" int string_len(const char* str);
extern "C" int string_cmp(const char* str1, const char* str2);
extern "C" const char* string_chr(const char* str, char c);
extern "C" char* string_cpy(char* dest, const char* src);
extern "C" char* string_cat(char* dest, const char* src);

int main() {
 	char c[] = "test";
 	char c1[] = "test"; char c2[] = "tests";
 	char c3[4]; char c5[8] = "ing";
 	std::cout << "The length of the string 'test' is " << string_len(c) << std::endl;
 	std::cout << "This value should be 0:" << string_cmp(c,c1) << " and this value should not be 0:" << string_cmp(c1,c2) << std::endl;
 	std::cout << "The string '"<< c << "' starting at char 'e' is " << string_chr(c, 'e') << std::endl;
 	std::cout << "This string should say 'test':" << string_cpy(c3,c) << std::endl;
 	std::cout << "This string should say 'testing':" << string_cat(c,c5) << std::endl;

}