#include <stdio.h>
#include <stdlib.h>

#include "app.hpp"
#include "shader.hpp"

int main(int argc, char *argv[])
{
	App *a = App::get_instance();
	a->run();
}

