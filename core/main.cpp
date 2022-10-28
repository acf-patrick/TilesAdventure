#include "game/game.h"

int main( int argc, char* argv[] )
{
	Game((argc > 1) ? argv[1] : "config.yml").run();
	return 0;
}