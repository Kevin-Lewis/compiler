#include <iostream>

extern "C" int string_len(const char* str);

int main() {
 	char c[] = "test";
 	std::cout << string_len(c) << std::endl;
}