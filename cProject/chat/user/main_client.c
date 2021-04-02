#include "clientMng.h"
#include <stdio.h>
#include <stdlib.h>
int main()
{
    ClientMng* newclientMng = ClientMngCreate();
    ClientMngRun(newclientMng);
    return 0;
}
