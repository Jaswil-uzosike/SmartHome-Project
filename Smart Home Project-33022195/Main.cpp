#include "SmartHome.h"

//Note: Header files aren't commented because I feel they are easier to understand than the cpp files.
//The cpp files are finely commented
int main() {
#ifdef _DEBUG
    onexit(_CrtDumpMemoryLeaks);
#endif

    SmartHome home;
    home.run();
    return 0;
}