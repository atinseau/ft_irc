#include <string.h>
#include <iostream>

void write() {
	static char buffer[512];
	strcpy(buffer + strlen(buffer), "hello");

	std::cout << buffer << std::endl;
}

int main(void) {

	write();
	write();
	write();

	return (0);
}