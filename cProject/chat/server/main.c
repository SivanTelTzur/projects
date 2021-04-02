#include "serverMng.h"
#include <stdio.h>

int main()
{
    ServerMng* App;
    App = ServerMngCreate(100);
    ServerMngRun(App);
    return 0;
}