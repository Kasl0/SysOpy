#include <stdio.h>
#include <dlfcn.h>

int main (int l_parm, char * wparam[]) {

    void *uchwyt = dlopen("./libbibl1.so", RTLD_LAZY);
    if(!uchwyt) {
        printf("blad otw.biblioteki\n");
        return 0;
    }

    void (*f2)(void);
    f2 = dlsym(uchwyt, "wypisz");
    if(dlerror()!=NULL) {
        printf("blad fun 2\n");
        return 0;
    }

    f2();
    dlclose(uchwyt);
    return 0;
}