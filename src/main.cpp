#include "test.h"
#include "batcher.h"
#include <NTL/ZZ_pXFactoring.h>
#include <NTL/ZZ_pEX.h>

using namespace std;
using namespace NTL;


// Test Flash library
int main()
{
    //TestSort();

    //TestBatching();

    TestBatchedComparison();

    return 0;
}
