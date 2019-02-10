# Newbies guide to setting up Google Test
This guide is written primarily for developers that want to set up the Google Test framework in order to start writing and running C/C++ projects in the CLI. However, these steps may be relevant for other development environments as well. The focus in this guide is how to get Google Test up and running and not writing tests themselves - there are plenty of other guides for that. This guide also assumes you have some knowledge in C and Makefiles.

### Example
The example I will be using in this guide will be a function that will add 1 to an integer if it is 0 or positive (`x >= 0`), or subtract 1 if it is negative (`x < 0`). Sample code can be found in the directory `myfunction`.

**Note**: The function that we will be testing needs to be in its own source file that **does not** have a `main()` function, otherwise you will run into compilation errors.

Here is the starting directory structure for the example:
```
myfunction/
├── myfunction.c
└── myfunction.h
```

##### `myfunction.h`:
```
#ifndef __MY_FUNCTION_H__
#define __MY_FUNCTION_H__

int myfunction(int x);

#endif
```

##### `myfunction.c`:
```
#include "myfunction.h"

int myfunction(int x) {
    return x >= 0 ? ++x : --x;
}
```

## Setting up Google Test
### Download and unzip source code
First, we need to get the source code for Google Test.
1. Download the source code from [here](https://github.com/google/googletest/releases). At the time of writing, I used v1.8.1.
2. Extract contents of zip and move to preferred directory. For this guide, the directory will be `/usr/src/googletest`. **Note:** I also renamed the root folder from `googletest-release-*` to simply `googletest`.
    * Remember to replace this path in future steps with your own directory if you choose a different one.
    * The contents of the zip file include source code for both googlemock and googletest, we are only focusing on googletest.

## Writing the tests
Next, let's write our unit tests.
1. Create a source file to write your tests, in this example, it will be `myfunction_unittest.cc`.
2. In this file, you must include your header file and the `gtest/gtest.h` header.
3. Write unit tests using the `TEST(TestSuiteName, TestName)` macro.
4. You can find out more about different assertions and what they mean in the [Google Test primer](https://github.com/google/googletest/blob/master/googletest/docs/primer.md).

##### `myfunction_unittest.cc`:
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

## Build and run tests
Now, we need to configure our Makefile, then build and run our tests.
1. Copy the Makefile from `/usr/src/googletest/googletest/make/Makefile` to the root of C project you wish to write unit tests for. For this example, this would be the `myfunction` directory. **The following steps will be made to the newly copied Makefile.**
2. Change the `GTEST_DIR = ..` to point to your google test directory (line 17).
    * In this case, `GTEST_DIR = /usr/src/googletest/googletest`.
3. Change the `USER_DIR = ../samples` to point to your project directory (line 20).
    * In this case, `USER_DIR = /path/to/myfunction`.
4. Change the `TESTS = sample1_unittest` to point the name of your unit test binary that will be created (line 32).
    * For this guide, it will be `myfunction_unittest`, so this line should be changed to `TESTS = myfunction_unittest`.
    * **Note:** You can add more than one unit test binary, just add them separated by a space and make sure you have the proper make rules to make them (covered below).
5. Copy the make rules for the `sample1` source code (line 74 - 82).
6. Change the newly copied make rules to point to your source files. I simply looked for all instances of `sample1` and replaced it with `myfunction` and ensured the file extensions matched (which is important if you are using `.cpp` or something else).
    * **Note:** If you choose to write your own `main()` function in the unit test file, you will also need to change the link from `gtest_main.a` to `gtest.a` or you will get a compilation error due to multiple definitions of `main()`.
7. Run `make` on the command line to generate the unit test binary.
    * In this case, `myfunction_unittest` will be created.
8. Run `./myfunction_unittest` to see the results of the unit tests!

##### Make rules for myfunction:
```
myfunction.o : $(USER_DIR)/myfunction.c $(USER_DIR)/myfunction.h $(GTEST_HEADERS)
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/myfunction.c

myfunction_unittest.o : $(USER_DIR)/myfunction_unittest.cc \
                     $(USER_DIR)/myfunction.h $(GTEST_HEADERS)
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $(USER_DIR)/myfunction_unittest.cc

myfunction_unittest : myfunction.o myfunction_unittest.o gtest_main.a
    $(CXX) $(CPPFLAGS) $(CXXFLAGS) -lpthread $^ -o $@
```

##### Output from `myfunction_unittest`
```
[==========] Running 2 tests from 1 test case.
[----------] Global test environment set-up.
[----------] 2 tests from MyFunction
[ RUN      ] MyFunction.PositiveNum
[       OK ] MyFunction.PositiveNum (0 ms)
[ RUN      ] MyFunction.NegativeNum
[       OK ] MyFunction.NegativeNum (0 ms)
[----------] 2 tests from MyFunction (0 ms total)

[----------] Global test environment tear-down
[==========] 2 tests from 1 test case ran. (1 ms total)
[  PASSED  ] 2 tests.
```

## Conclusion
Hopefully, this guide has helped you hit the ground running with Google Test and you can start developing C/C++ using Test Driven Development. Again, this is not meant to be an advanced guide - there are plenty of documentation out there on how to _use_ Google Test, but few on actually setting it up for the first time. The final directory structure can be found in `myfunction`. Happy testing!
