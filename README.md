[btree-framework](https://github.com/andreas-steffens/btree-framework)
=================

The btree-framework provides a number of associative and linear containers, each being based on a btree structure. The intention is to deliver increased performance, since other implementations use linear arrays, link lists and red-black as well as binary trees. Any of the above methods develop poor performance when it comes to large data amounts, while btrees can scale quite well to that end. Furthermore, the provided containers come with an API interface compatible to existing STL containers, which enables a quick switch between container types within a project. Any btree container type can be abstracted, as oppose to their existing STL equivalents, allowing applications to create their own container type hierarchy. Also, the sorting criteria is a virtual method and not a template parameter, which makes it possible for iterators to support so called "scary assignments", as described by Bjarne Sproustrup’s paper [“Minimizing Dependencies within Generic Classes for Faster and Smaller Programs”](http://www.stroustrup.com/SCARY.pdf). For increased scalability, the size&#95;type is selected via a template parameter and unlike other implementations, is not bound to what size&#95;t is set to.

And finally, the project supports cmake, which is explained in the "How To Build" section below.

Note: Please understand that this is not a stand-alone project and needs to be build as a sub-module or library in a project, which employs the btree-framework. If you want to see a project where this framework is being actively used, then please go to the [btree-frame<b>-testbench</b> project](https://github.com/andreas-steffens/btree-framework-testbench) and follow the how-to-build instructions there.
<br>
<br>
## Documentation

The documentation of this project comes in two flavours:
- [Documentation as Open Document Text file (.ODT)](https://github.com/andreas-steffens/btree-framework-testbench/blob/master/doc/btree%20programmer%20guide.odt?raw=true)
- [Documentation as Portable Document Format file (.PDF)](https://github.com/andreas-steffens/btree-framework-testbench/blob/master/doc/btree%20programmer%20guide.pdf?raw=true)

The documentation is pretty large and might be a bit overwhelming. To address that problem, please find the section "1. Where to start" of the [README](https://github.com/andreas-steffens/btree-framework/blob/master/README) file. This section tells what sections need to be initially read in order to understand how this framework is working and supposed to be used.
<br>
<br>
## How To Build

This project supports cmake, which ought to make it very easy to include the resulting btree-framework library in any project that uses cmake. 
<br>
<br>
How to integrate the stand alone btree-framework library under windows and linux?
--
```
cd .../to/your/projects/location/or/common/code/location
git clone https://github.com/andreas-steffens/btree-framework
```
<br>
<br>
If your project doesn't use cmake, then please refer to section "2.2.2. How to make the framework part of an exsting project which uses visual studio" of the [README](https://github.com/andreas-steffens/btree-framework/blob/master/README) file. Under linux you will have to add this project as a library or as source manually to your make files. Please read section "2.1.2. How to make the framework part of an exsting project which uses a makefile" of the [README](https://github.com/andreas-steffens/btree-framework/blob/master/README) file, to see how to do that.
<br>
<br>
If your project uses cmake, then the appropriate CMakeLists.txt(s) need(s) to be modified. By default, the directory in which the btree framework is cloned into, is called btree-framework, if you have used the above commands. Since it is also possible to have this project as a sub-module in another repository, it can be checked out to a folder with a different name. Hence, in the below description the btree framework main directory is displayed as this placeholder: `<btree-framework>`.

In your main CMakeLists.txt the btree framework path needs to be registered as follow:

```
list (APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/<btree-framework>)
```

If need be, the following optional build flag(s) can be added by the command(s) below:

```
# for windows use the line below
add_definitions ("/DBTREE_ITERATOR_REGISTRATION")
```
```
# for linux use the line below
add_definitions ("-DBTREE_ITERATOR_REGISTRATION")
```

To find out what that flag does refer to section "2.3. build flags" of the [README](https://github.com/andreas-steffens/btree-framework/blob/master/README) file. Furthermore, you need to make sure that `add_definitions` is coming <b>before</b> the subdirectory registration (`add_subdirectory`).

The project needs to be aware of the btree framework's subdirectory (see below):

```
add_subdirectory (<btree-framework>)
```
<br>
<br>
Any module that requires the btree framework needs to make sure it is available by adding this command:

```
find_package	(<btree-framework>	REQUIRED)
```

The include paths need to be appended using the provided variable ${BTREE_FRAMEWORK_INCLUDE_DIRS} (see below):

```
include_directories
	(	
		... other includes ...
		${BTREE_FRAMEWORK_INCLUDE_DIRS}
	)
```

For the correct build order to be generated by cmake, the framework must be flagged as a dependency:

```
add_dependencies (${PROJECT_NAME} btree_framework)
```

And finally cmake must be aware what to link it against:

```
target_link_libraries (${PROJECT_NAME} btree_framework)
```
<br>
<br>
How to build the test bench under windows for x86 and x86&#95;64 platforms?
--
Via cygwin command execute the following commands:

```
cd .../to/your/projects/location
git clone --recursive https://github.com/andreas-steffens/btree-framework-testbench
cd btree-framework-testbench/src/btreetest
```

Here you can find a number of batch files called msvce201x.x86(&#95;64).bat. These batch files can be executed via command line or the windows explorer. Each of those batch files creates a directory named "cmake" and generates all cmake files as well as the respective visual studio express solution files, set up for the selected platform and visual studio express version, in there. Then it tries to open whatever visual studio express application can be found under the registry key HKEY&#95;CLASSES&#95;ROOT\Applications\WDExpress.exe\shell\open\command. Please see below what versions of visual studio express and target platforms are currently supported:


```
msvce2013.x86.bat    - Visual Studio Express 2013 for platform Win32
msvce2013.x86_64.bat - Visual Studio Express 2013 for platform x64
msvce2015.x86.bat    - Visual Studio Express 2015 for platform Win32
msvce2015.x86_64.bat - Visual Studio Express 2015 for platform x64
```

If the project needs re-opening at a later point, simply use the same batch file again. In the event that you accidentally select the wrong batch file, cmake will refuse to create new project files and aborts with an error message. As a result your existing project files remain unharmed.
In case you want build the project say for a different platform, then you'll need to close the visual studio instance that has the btree-framework test bench project open. After that, you need to delete or rename the afore mentioned cmake directory and execute a different batch file that generates the project files for the desired platform. However, any modifications that have been made to the project will be lost!
<br>
<br>
How to build the test bench under linux?
--
Via command execute the following commands:

```
cd .../to/your/projects/location
git clone --recursive https://github.com/andreas-steffens/btree-framework-testbench
cd btree-framework-testbench/src/btreetest
make debug        # this builds the binary with debug settings
make regression   # this builds the binary with debug settings and runs all regression tests
```

In order to see all build rules, please see section "2.1.1. How to build the test bench using cmake" of the [README](https://github.com/andreas-steffens/btree-framework/blob/master/README) file.


<br>
<br>
## Version

The current version of the project is: 0.08 (alpha)

Please don't be discouraged by the fact that the version contains the word "alpha". What this means is that the feature set is incomplete, but what has been implemented is stable and thoroughly tested. See below to find out what is missing:

- initializer&#95;list<> support
 
<br>
<br>
## Agenda

For the project to leave its alpha state, all the above features and methods will be implemented. Once, the project is entering the beta phase the following points will be addressed:

- more application based testing, combined with the existing artificial testing approach
- performance tests
- c++11 language feature tests
- performance improvements

<br>
<br>
## Btree-Framework Project on Facebook

If you want to see updates or release on the btree-framework project, then follow this link: [https://www.facebook.com/btreeframework](https://www.facebook.com/btreeframework)<br>
Every release post also contains a brief change log statement, allowing to tell what major changes have been made.
<br>
<br>
## Contact

Please see [README](https://github.com/andreas-steffens/btree-framework/blob/master/README) section "3. Feedback".
<br>
<br>
## License

Licensed under the Lesser GNU Public License, Version 3.0: [http://www.gnu.org/licenses/lgpl-3.0.txt](http://www.gnu.org/licenses/lgpl-3.0.txt)


