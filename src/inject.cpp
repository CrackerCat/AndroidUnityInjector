#include "dobby.h"
#include "main.h"
#include <stdio.h>

int main() {
    auto version = DobbyGetVersion();
    printf("Dobby version: %s\n", version);
    return 0;
}