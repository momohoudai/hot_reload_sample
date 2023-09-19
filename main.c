//
// TLDR VERSION
//   - Install gcc.
//   - Compile code to hot reload:
//     $ gcc -shared hot.c -o -hot.so
//
//   - Compile code to run the program:
//     $ gcc main.c -o main.exe
//   
//   - Run the program and watch it print "HOT" repeatedly:
//     $ ./main.exe
//
//   - Open another terminal, open "hot.c" and change "HOT" to "COLD"
//   - Compile code to hot reload again:
//     $ gcc -shared hot.c -o -hot.so
//   
//   - ????????????????
//   - PROFIT! (Watch the program magically start to print "COLD" repeatedly!)
//
// DESCRIPTION
//   This is just an example code to showcase how hot-reloading works.
//   There is a surprising lack on information on how to implement it.
//
//   In general, there are many ways to do it, but you have 2 files:
//   > The main file which runs in a loop.
//   > The shared library file (DLL or SO) that contains code you want to reload.
//
//   Then you just have to reload the shared library file pretty much whenever
//   you decide you need to within the loop, depending on how your app works and
//   depending on what platform your app is on. 
//
//   For example, if you are running a game loop in 60FPS or whatever in Windows, 
//   you can use Windows API to check if the DLL file has been modified
//   and THEN reload the DLL. Okay, it's not THAT straightforward because you will 
//   have to  
//
//   This example is a simple console app that is specific to linux, which will run
//   in an infinite loop, reloading "hot.so" and calling the "print" function in it
//   each second. While it is running in one terminal, go to another terminal, 
//   change the printf() in "hot.c" and compile it to as a shared linux library 
//   object "hot.so" with the following command:
//
//     gcc -shared hot.c -o hot.so
//   
//   You should see it print different things ;) 
//
//   Exercise to the reader: Instead of loading the SO file each second, make it so that
//   it only loads the SO file when the SO file is modified!
//
// HOW TO BUILD WITH GCC
//   gcc main.c -o main.exe
//
// HOW TO RUN
//   ./

#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>


int main() {
  void *handle;
  void (*print)(void);
  char *error;

  while(1) {
    handle = dlopen ("./hot.so", RTLD_LAZY);
    if (!handle) {
      fprintf (stderr, "%s\n", dlerror());
      exit(1);
    }
    dlerror();    /* Clear any existing error */
    print = dlsym(handle, "print");
    if ((error = dlerror()) != NULL)  {
      fprintf (stderr, "%s\n", error);
      exit(1);
    }

    // Call the function
    (*print)();
    dlclose(handle);
    sleep(1);
  }

  return 0;

}
