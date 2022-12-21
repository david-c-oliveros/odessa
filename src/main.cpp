#include "App.h"


int main()
{
    App app = App();

    if (app.Construct())
        app.Start();

    return 0;
}
