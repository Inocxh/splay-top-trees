#include <catch2/catch_test_macros.hpp>

#include <iostream> 

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}
int add(int x, int y) {
    return x + y;
}


TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}
TEST_CASE( "Factorials are computed", "[add]" ) {
    REQUIRE( add(1, 3) == 4 );
    REQUIRE( add(5, 4) == 9 );
}