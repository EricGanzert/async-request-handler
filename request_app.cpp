#include "request.h"

#include <iostream>

using namespace std;

int main()
{
    RequestHandler rh;

    cout << "making a blocking request" << endl;
    auto result = rh.doBlockingAction();
    cout << "blocking request returned" << endl << endl;

    cout << "making an async request" << endl;
    auto fut = rh.doAsyncAction();
    cout << "async request returned" << endl;
    result = fut.get();
    cout << "result obtained for async request" << endl;
}