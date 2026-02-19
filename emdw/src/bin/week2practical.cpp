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

  // NOTE: this activates logging and unit tests
  // initLogging(argv[0]);
  // prlite::TestCase::runAllTests();  
  
  
  try {

    unsigned seedVal = emdw::randomEngine.getSeedVal();
    cout <<  seedVal << endl;
    emdw::randomEngine.setSeedVal(seedVal);

    typedef int T; // The type of the values that the RVs can take on
    typedef DiscreteTable<T> DT; // DT now is a short-hand for DiscreteTable<int>
    double defProb = 0.0; // Any unspecified probs will default to this.
    
    rcptr< vector<T> > binDom ( // Lists the values that a particular RV can take on
        new vector<T>{0,1});

    rcptr< vector<T> > triDom (
        new vector<T>{0,1,2});

    enum{I, C, M}; // ID's: 0, 1, 2

    // QUESTION 1.1 a
    // Constructing Factors and Calculating Joint Distribution P(M,I,C)

    rcptr<Factor> ptrI =
        uniqptr<DT>(
            new DT(
                {I},
                {triDom},
                defProb,
                {
                    {{0}, 0.33},
                    {{1}, 0.33},
                    {{2}, 0.33},
                } ));
    
    std::cout << "The Marginal Distribution of I, P(I) \n";
    std::cout << *ptrI << std::endl;
    
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