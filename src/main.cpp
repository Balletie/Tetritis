#include "window.h"
#include <stdlib.h>

int main() {
	srand(time(NULL));
	if (!window::open()) return EXIT_FAILURE;
	if (!window::loop()) return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
