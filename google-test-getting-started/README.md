# Newbies guide to setting up Google Test
This guide is primarily for developers wanting to set up the Google Test framework for writing and running C/C++ projects in the CLI since that was my specific use case. However, hopefully the steps are relevant for other development environments as well. This guide also assumes you have some knowledge in Makefiles.

### Example
The example I will be using to unit test will be a function that will add 1 to an integer if it is 0 or positive (`x >= 0`), or subtract 1 if it is negative (`x < 0`). Sample code can be found in the directory `myfunction/`.

Here is the starting directory structure for the example:
```
myfunction/
├── myfunction.c
└── myfunction.h
```

`myfunction.h`:
```
#ifndef __MY_FUNCTION_H__
#define __MY_FUNCTION_H__

int myfunction(int x);

#endif
```

`myfunction.c`:
```
#include "myfunction.h"

int myfunction(int x) {
    return x >= 0 ? ++x : --x;
}
```

## Setting up Google Test
### Download and unzip source code
First, we need to get the source code for Google Test.
1. Download source code from [here](https://github.com/google/googletest/releases). I used v1.8.1 at the time of writing.
2. Extract contents of zip and move to preferred directory. For this guide, the directory will be `/usr/src/googletest`. Remember to replace this in future steps with your own directory if you choose a different one.
    * The contents of the zip file include source code for both googlemock and googletest, we are only focusing on googletest.

### Writing the tests
Next, let's write our unit tests.
1. Create a source file to write your unit tests, `myfunction_unittest.cc` in this example.
2. In this file, you must include your header file and the `gtest/gtest.h` header.
3. Write unit tests using `TEST(TestSuiteName, TestName)`.
4. Different assertions and what they mean can be found in the Google Test [primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md).

`myfunction_unittest.cc`:
```
#include "myfunction.h"
#include "gtest/gtest.h"

TEST(MyFunction, PositiveNum) {
    int expected = 7;

    ASSERT_EQ(expected, myfunction(6));
}

TEST(MyFunction, NegativeNum) {
    int expected = -7;

    ASSERT_EQ(expected, myfunction(-6));
}
```

### Build and run tests
Now, we need to configure our Makefile, then build and run our tests.
1. Copy the Makefile from `/usr/src/googletest/googletest/make/Makefile` to the root of C project you wish to write unit tests for. For this example, this would be the `myfunction/` directory. **The following steps will be made to the newly copied Makefile.**
2. Change the `GTEST_DIR = ..` to point to your google test directory. In this case, `GTEST_DIR = /usr/src/googletest`.
3. Change the `USER_DIR = ../samples` to point to your project directory. In this case, `USER_DIR = /path/to/myfunction`.
4. Change the `TESTS = sample1_unittest` to point the name of your unit test binary (not yet created). For this guide, I will choose `myfunction_unittest`, so this line should be changed to `TESTS = myfunction_unittest`.
    * Note: You can add more than one unit test binary, just add them space-separated and make sure you have the proper make rules to make them (covered below).
5. Copy the make rules for the `sample1` source code (found starting at line 74).
6. Change the newly copied make rules to point to your source files. I simply looked for all instances of `sample1` and replaced it with `myfunction` and ensured the file extensions matched (important if you are using `.cpp` or something else).
    * **Note:** If you choose to write your own `main()` function in the unit test file, then you will also need to change the link from `gtest_main.a` to `gtest.a` or you will get a compilation error due to multiple definitions of `main()`!
7. Run `make` on the command line to generate the unit test binary. In this case, `myfunction_unittest` will be created.
8. Run `myfunction_unittest` to see the results of the unit tests!

Example make rules for myfunction:
```
myfunction.o : $(USER_DIR)/myfunction.c $(USER_DIR)/myfunction.h $(GTEST_HEADERS)
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/myfunction.c

myfunction_unittest.o : $(USER_DIR)/myfunction_unittest.cc \
                     $(USER_DIR)/myfunction.h $(GTEST_HEADERS)
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/myfunction_unittest.cc

myfunction_unittest : myfunction.o myfunction_unittest.o gtest_main.a
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
```

### Conclusion
Hopefully, this guide has helped you hit the ground running with Google Test and you can start developing C/C++ using Test Driven Development. Again, this is not meant to be an advanced guide - there are plenty of documentation out there on how to _use_ Google Tests, but few on actually setting it up for the first time.

This is what the example directory looks like in the end (minus binary files):
```
myfunction/
├── Makefile
├── myfunction.c
├── myfunction_unittest.cc
└── myfunction.h
```
