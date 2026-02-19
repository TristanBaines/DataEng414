/*
 * Author     :  (DSP Group, E&E Eng, US)
 * Created on :
 * Copyright  : University of Stellenbosch, all rights retained
 */

// patrec headers
#include "prlite_logging.hpp"  // initLogging
#include "prlite_testing.hpp"

// emdw headers
#include "emdw.hpp"
#include "discretetable.hpp"

// standard headers
#include <iostream>  // cout, endl, flush, cin, cerr
#include <cctype>  // toupper
#include <string>  // string
#include <memory>
#include <set>
#include <map>
#include <algorithm>
#include <limits>
#include <random>

using namespace std;
using namespace emdw;

int main(int, char *argv[]) {

  // NOTE: this activates logging and unit tests // commenting out for cleaner output
  // initLogging(argv[0]);
  // prlite::TestCase::runAllTests();

  try {

    //*********************************************************
    // Some random generator seeding. Just keep this as is
    //*********************************************************

    unsigned seedVal = emdw::randomEngine.getSeedVal();
    cout <<  seedVal << endl;
    emdw::randomEngine.setSeedVal(seedVal);

    //*********************************************************
    // Predefine some types and constants
    //*********************************************************

    typedef int T;                  // The type of the values that the RVs can take on
    typedef DiscreteTable<T> DT;    // DT now is a short-hand for DiscreteTable<int>
    double defProb = 0.0; // default probability - Any unspecified probs will default to this.
    
    rcptr< vector<T> > binDom ( // Lists the values that a particular RV can take on
        new vector<T>{0,1});

    rcptr< vector<T> > triDom (
        new vector<T>{0,1,2}
    );

    // Defining Random Variables
    enum{X, Y, Z}; // ID's: 0, 1, 2

    // QUESTION 1a
    // Creating P(X) table the pointer way
    rcptr<Factor> ptrX =  
      uniqptr<DT>(                 
        new DT(                    
          {X},
          {binDom},
          defProb,
          {
            {{0}, 0.5},
            {{1}, 0.5},
          } ));

    std::cout << "The Marginal Distribution of X (P(X)) \n";
    std::cout << *ptrX << std::endl;
    
    // Creating P(Y) table the pointer way
    rcptr<Factor> ptrY = 
      uniqptr<DT>(
        new DT(                   
          {Y},
          {binDom},
          defProb,
          {
            {{0}, 0.5},
            {{1}, 0.5},
          } ));

    std::cout << "The Marginal Distribution of Y (P(Y)) \n";
    std::cout << *ptrY << std::endl; 

    // Creating P(Z|X,Y) table the pointer way
    rcptr<Factor> ptrZgXY =  
      uniqptr<DT>(                 
        new DT(                    
          {X, Y, Z},
          {binDom, binDom, triDom},
          defProb,
          {
            {{0, 0, 0}, 1},
            {{0, 0, 1}, 0},
            {{0, 0, 2}, 0},
            {{0, 1, 0}, 0},
            {{0, 1, 1}, 1},
            {{0, 1, 2}, 0},
            {{1, 0, 0}, 0},
            {{1, 0, 1}, 1},
            {{1, 0, 2}, 0},
            {{1, 1, 0}, 0},
            {{1, 1, 1}, 0},
            {{1, 1, 2}, 1}
          } ));

    std::cout << "The Conditional Distribution of Z|X,Y (P(Z|X,Y)) \n";
    std::cout << *ptrZgXY << std::endl; 

    // QUESTION 1c
    // Multiplying P(Z|X,Y) with P(X) and P(Y) to obtain P(Z,X,Y)
    rcptr<Factor> ptrXYZ = 
      ptrZgXY ->absorb(ptrX)
              ->absorb(ptrY)
              ->normalize();

    std::cout << "The Joint Distribution of X,Y,Z (P(X,Y,Z)) \n";
    std::cout << "Obtained by Multiplying P(Z|X,Y) with P(X) and P(Y) \n";
    std::cout << *ptrXYZ << std::endl;
    
    // QUESTION 1d
    // Marginalizing out Y, so keeping X and Z
    rcptr<Factor> ptrXZ = 
      ptrXYZ -> marginalize({X,Z});

    std::cout << "Joint Distribution of X and Z (P(X,Z)) \n";
    std::cout << "Marginalizing out Y \n";
    std::cout << *ptrXZ << std::endl;

    // QUESTION 1e
    // Marginalizing out X, so keeping Z
    rcptr<Factor> ptrZ =
      ptrXZ -> marginalize({Z});

    std::cout << "Marginal Distribution of Z (P(Z)) \n";
    std::cout << "Marginalizing out X \n";
    std::cout << *ptrZ << std::endl;
          
    // QUESTION 1f
    // Determining the Conditional Distribution P(Z|X=1)

    rcptr<Factor> ptrZgX1 =
      ptrXZ -> observeAndReduce({X},{1})
              -> normalize();

    std::cout << "Conditional Distribution of Z given that X = 1 (P(Z|X=1)) \n";
    std::cout << *ptrZgX1 << std::endl;

    // QUESTION 1g
    // Determining the Conditional Distribution P(X|Z=2)

    rcptr<Factor> ptrXgZ2 =
      ptrXZ -> observeAndReduce({Z},{2})
            ->normalize();

    std::cout << "Conditional Distribution of X given that Z = 2 (P(X|Z=2)) \n";
    std::cout << *ptrXgZ2 << std::endl;

    return 0;
  } // try

  catch (char msg[]) {
    cerr << msg << endl;
  } // catch

  catch (char const* msg) {
    cerr << msg << endl;
  } // catch

  catch (const string& msg) {
    cerr << msg << endl;
    throw;
  } // catch

  catch (const exception& e) {
    cerr << "Unhandled exception: " << e.what() << endl;
    throw e;
  } // catch

  catch(...) {
    cerr << "An unknown exception / error occurred\n";
    throw;
  } // catch

} // main