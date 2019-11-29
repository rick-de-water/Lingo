#define CATCH_CONFIG_RUNNER
#include <catch/catch.hpp>

int main(int argc, char* argv[])
{
	Catch::Session session;
	int result = session.run(argc, argv);

	return result;
}