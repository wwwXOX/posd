#ifndef UTLIST_H
#define UTLIST_H

//#include <string>
//using std::string;

#include "list.h"
#include "struct.h"
#include "atom.h"
#include "number.h"
#include "variable.h"

// When create a new list without any item
// Then #symbol() of the list should return "[]"
TEST ( List , constructor ) {
  	List l;
	ASSERT_EQ ( "[]" , l.symbol() );
}

// Given there are two perfect numbers: 8128, 496
// When create a new list with the perfect Number
// Then #symbol() of the list should return "[8128]"
TEST ( List , Numbers ) {
	Number n1 ( 8128 ) , n2 ( 496 );
	std :: vector < Term * > args = { &n1 , &n2 };
  	List l ( args );
	ASSERT_EQ ( "[8128, 496]" , l.symbol() );
}

// Given there are two atoms: "terence_tao", "alan_mathison_turing"
// When create a new list with the Atoms
// Then #symbol() of the list should return "[terence_tao, alan_mathison_turing]"
TEST ( List , Atoms ) {
	Atom a1 ( "terence_tao" ) , a2 ( "alan_mathison_turing" );
	std :: vector < Term * > args = { &a1 , &a2 };
  	List l ( args );
	ASSERT_EQ ( "[terence_tao, alan_mathison_turing]" , l.symbol() );
}

// Given there are two variables: X, Y
// When create a new list with the variables
// Then #symbol() of the list should return "[X, Y]"
TEST ( List , Vars ) {
	Variable X ( "X" ) , Y ( "Y" );
	std :: vector < Term * > args = { &X , &Y };
  	List l ( args );
	ASSERT_EQ ( "[X, Y]" , l.symbol() );
}

// ?- tom = [496, X, terence_tao].
// false.
TEST ( List , matchToAtomShouldFail ) {
	Atom tom ( "tom" ) , a ( "terence_tao" );
	Number n ( 496 );
	Variable X ( "X" );
	std :: vector < Term * > args = { &n , &X , &a };
  	List l ( args );
	ASSERT_FALSE ( tom.match ( l ) );	
}

// ?- 8128 = [496, X, terence_tao].
// false.
TEST ( List , matchToNumberShouldFail ) {
	Number n1 ( 8128 ) , n2 ( 496 );
	Variable X( "X" );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args = { &n2 , &X , &a };
  	List l ( args );
	ASSERT_FALSE ( n1.match ( l ) );
}

// ?- s(X) = [496, X, terence_tao].
// false.
TEST ( List , matchToStructShouldFail ) {
	Variable X ( "X" );
	std :: vector < Term * > v = { &X };
	Struct hobby ( Atom ( "s" ) , v );
	Number n ( 496 );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args = { &n , &X , &a };
  	List l ( args );
	ASSERT_FALSE ( hobby.match ( l ) );
}

// ?- Y = [496, X, terence_tao].
// Y = [496, X, terence_tao].
TEST ( List , matchToVarShouldSucceed ) {
	Variable Y ( "Y" ) , X ( "X" );
	Number n ( 496 );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args = { &n , &X , &a };
  	List l ( args );
	ASSERT_TRUE ( Y.match ( l ) );
}

// ?- X = [496, X, terence_tao].
// false.
TEST ( List , matchToVarOccuredInListShouldFail ) {
	Variable X ( "X" );
	Number n ( 496 );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args = { &n , &X , &a };
  	List l ( args );
	ASSERT_FALSE ( X.match ( l ) );
}

// ?- [496, X, terence_tao] = [496, X, terence_tao].
// true.
TEST ( List , matchToSameListShouldSucceed ) {
	Variable X ( "X" );
	Number n ( 496 );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args = { &n , &X , &a };
  	List l1 ( args );
	List l2 ( args );
	ASSERT_TRUE ( l1.match ( l2 ) );
}

// ?- [496, X, terence_tao] = [496, Y, terence_tao].
// true.
TEST ( List , matchToSameListWithDiffVarNameShouldSucceed ) {
	Variable X ( "X" ) , Y ( "Y" );
	Number n ( 496 );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args1 = { &n , &X , &a };
	std :: vector < Term * > args2 = { &n , &Y , &a };
  	List l1 ( args1 );
	List l2 ( args2 );
	ASSERT_TRUE ( l1.match ( l2 ) );	
}

// ?- [496, X, terence_tao] = [496, 8128, terence_tao].
// X = 8128.
TEST ( List , matchToVarToAtominListShouldSucceed ) {
	Variable X ( "X" );
	Number n1 ( 496 ) , n2 ( 8128 );
	Atom a ( "terence_tao" );
	std :: vector < Term * > args1 = { &n1 , &X , &a };
	std :: vector < Term * > args2 = { &n1 , &n2 , &a };
  	List l1 ( args1 );
	List l2 ( args2 );
	l1.match ( l2 );
	ASSERT_EQ ( "8128" , X.value() );	
}

// ?- Y = [496, X, terence_tao], X = alan_mathison_turing.
// Y = [496, alan_mathison_turing, terence_tao],
// X = alan_mathison_turing.
TEST ( List , matchVarinListToAtomShouldSucceed ) {
	Variable X ( "X" ) , Y ( "Y" );
	Number n ( 496 );
	Atom a1 ( "terence_tao" ) , a2 ( "alan_mathison_turing" );
	std :: vector < Term * > args = { &n , &X , &a1 };
  	List l ( args );
	Y.match ( l );
	X.match ( a2 );
	ASSERT_EQ ( "[496, alan_mathison_turing, terence_tao]" , Y.value() );	
	ASSERT_EQ ( "alan_mathison_turing" , X.value() );
}

// Example: 
// ?- [first, second, third] = [H|T].
// H = first, T = [second, third].
TEST ( List, headAndTailMatching1 ) {
	Atom f ( "first" ) , s ( "second" ) , t ( "third" );
  	std :: vector < Term * > args = { &f , &s , &t };
  	List l ( args );
  	EXPECT_EQ ( string ( "first" ) , l.head() -> value() );
  	EXPECT_EQ ( string ( "[second, third]" ) , l.tail() -> value() );
}

// Example:
// ?- [first, second, third] = [first, H|T].
// H = second, T = [third].
TEST ( List , headAndTailMatching2 ) {
	Atom f ( "first" ) , s ( "second" ) , t ( "third" );
  	std :: vector < Term * > args = { &f , &s , &t };
  	List l ( args );
  	EXPECT_EQ ( string("second") , l.tail() -> head() -> value() );
  	EXPECT_EQ ( string("[third]") , l.tail() -> tail() -> value() );
}

// ?- [[first], second, third] = [H|T].
// H = [first], T = [second, third].
TEST ( List , headAndTailMatching3 ) {
	Atom f ( "[first]" ) , s ( "second" ) , t ( "third" );
  	std :: vector < Term * > args = { &f , &s , &t };
  	List l ( args );
  	ASSERT_EQ ( string("[first]") , l.head() -> value() );
  	ASSERT_EQ ( string("[second, third]") , l.tail() -> value() );
}

// ?- [first, second, third] = [first, second, H|T].
// H = third, T = [].
TEST ( List , headAndTailMatching4 ) {
	Atom f ( "first" ) , s ( "second" ) , t ( "third" );
  	std :: vector < Term * > args = { &f , &s , &t };
  	List l ( args );
  	ASSERT_EQ ( string("third") , l.tail() -> tail() -> head() -> value() );
  	ASSERT_EQ ( string("[]") , l.tail() -> tail() -> tail() -> value() );
}
 
// Given there is a empty list
// When client still want to get the head of list
// Then it should throw a string: "Accessing head in an empty list" as an exception.
TEST ( List , emptyExecptionOfHead ) {
  	List l;
	try {
		l.head();
	}catch ( std :: string e ){
  		ASSERT_EQ ( "Accessing head in an empty list" , e );
	}
}

// Given there is a empty list
// When client still want to get the tail of list
// Then it should throw a string: "Accessing tail in an empty list" as an exception.
TEST ( List , emptyExecptionOfTail ) {
	List l;
	try {
		l.tail();
	}catch ( std :: string e ){
  		ASSERT_EQ ( "Accessing tail in an empty list" , e );
	}
}
#endif
