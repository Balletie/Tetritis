#include "window.h"

int main() {
	if (!window::open()) return EXIT_FAILURE;
	if (!window::loop()) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
