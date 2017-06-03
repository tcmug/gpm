
#include <wchar.h>
#include <stdlib.h>


typedef struct _package {
    const wchar_t *name;
} package;


int main(int argc, char *args[]) {

    package pack = {
        L"hello world"
    };

    wprintf(pack.name);
    wprintf(L"\n");

    return 0;
}
