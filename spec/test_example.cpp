#include <test>

#include <example.h>


TEST_CASE( "Example test", "[test]" ) {
    GIVEN("truthy") {
        REQUIRE( IsTruthy() == true );
    }

    GIVEN("falsy") {
        REQUIRE( IsFalsy() == false );
    }
}
