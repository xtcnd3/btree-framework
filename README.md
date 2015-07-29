btree-framework

Author: Andreas Steffens
Version: 0.01 (alpha)
License: GPL v2


Introduction

The btree-framework is aimed at programmers that are looking for associative data container classes capable of handling data sets at a very large scale. Said data containers are coming with an interface similar to the existing STL container classes map, multimap, set and multiset. Furthermore, iterators, with a random access qualifier enabled, are supported, which ought to allow for a quick interchange between this framework and the STL container. Also, an array type, that isn't suffering from any slow down when data is inserted or removed at the beginning of the list, is available. This array type has an interface similar to the STL class vector.

Contents

1. Where to start
2. How to build this framework
2.1. Linux
2.2. Windows
2.3. build flags
2.4. What has to be included
3. Feedback
3.1. What increases the chances that I will respond to your email
3.1.1. A proper subject, which tells the context of what I am going to find in your email's body.
3.1.2. An email body that gets to the point of what you want to say.
3.2. What decreases the chances that I will respond to your email
3.2.1. An empty or useless subject.
3.2.2. Email content that is confusing or off topic.
3.2.3. Content that has been covered by the documentation or is too obvious to warrant a reply.
3.2.4. Improper english.


1. Where to start

The folder ./doc contains an open office document file called "btree programmer guide.odt". That documentation explains this b-tree framework in detail, but only a sub set of sections is required to understand to get started. See below:

Quick Start Guide
Programmer Guide
	Class Hierarchy
		Data Classes (CBTreeArray and CBTreeKeySort)
			CBTreeArray
			CBTreeKeySort
	Interface Classes
		Data Interface Classes (CBTree*DataIf)
	Template Parameters
		Data Type (_t_data)
		Address Type (_t_sizetype)
		Node Iterator Type (_t_nodeiter)
		Sub-Node Iterator Type (_t_subnodeiter)
		Data Layer (_t_datalayer)
			Data Layer Properties (_t_datalayerproperties)
		Key Type (_t_key)
	Application Classes
		Application Specific Key Sort Classes
			comp ()
				Default comp () addendum
			extract_key ()
				extract_key () addendum




2. How to build this framework under linux or windows


2.1. Linux

For this framework to compile, a compiler capable to support the C++11 dialect is required.
This release was tested with: GCC (C++) 5.1.1 20150618 (Fedora 22)

To build the b-tree regression test system go to ./src/btreetest
and run "make debug".

List of make file rulese:
all             - runs regression, valgrind_report, coverage_report, performance, release
debug           - builds regression test binary with debug flags
release         - builds regression test binary with release flags
coverage        - builds regression test binary with debug and coverage flags
regression      - runs regression tests using debug binary
performance     - runs performance tests using release binary (this is not ready yet)
coverage_report - runs regression tests using coverage binary and creates result displayed as HTML
valgrind_report - runs regression tests using valgrind with debug binary
clean           - removes any build file (may those be intermediate or final)
clean_coverage  - removes any coverage or coverage report file (may those be intermediate or final)

To compile the b-tree framework into another project the files in the following locations are relevant:
./src/btreetest/btree_framework
./src/btreetest/filemem
./src/btreetest/compatibility

These folders then are then copied or sym. linked to your target or common code location.
The additional include paths your compiler needs to be aware of are:
/relative/or/absolute/path/to/your/common/location/btree_framework
/relative/or/absolute/path/to/your/common/location/btree_framework/stl_compatible
/relative/or/absolute/path/to/your/common/location/filemem
/relative/or/absolute/path/to/your/common/location/compatibility

The build LINUX needs to be set and optionally the build flags _DEBUG and BTREE_ITERATOR_REGISTRATION (see below).


2.2. Windows

For this framework to compile, at least Visual Sudio 2013 Express or a compiler that supports the C++11 dialect is required.

To build the b-tree regression test system:
1. Create a new project
2. Copy files from this release to your project location
3. The project needs to be aware of the following include locations:
/relative/or/absolute/path/to/your/project/location/avp_path_find
/relative/or/absolute/path/to/your/project/location/btree_framework
/relative/or/absolute/path/to/your/project/location/btree_framework/stl_compatible
/relative/or/absolute/path/to/your/project/location/filemem
/relative/or/absolute/path/to/your/project/location/compatibility
4. add the build flag BTREE_ITERATOR_REGISTRATION to the any build

To compile the b-tree framework into another project the files in the following locations are relevant:
./src/btreetest/btree_framework
./src/btreetest/filemem
./src/btreetest/compatibility

These folders then are then copied to your target or common code location.
The additional include paths that need to be in your project settings are:
/relative/or/absolute/path/to/your/common/or/project/location/btree_framework
/relative/or/absolute/path/to/your/common/or/project/location/btree_framework/stl_compatible
/relative/or/absolute/path/to/your/common/or/project/location/filemem


2.3. build flags

BTREE_ITERATOR_REGISTRATION:

If your code makes sure, that any b-tree framework iterator instance is destroyed before its respective associated constainer instance is destroyed, then the build flag BTREE_ITERATOR_REGISTRATION can be omitted, otherwise you would be facing access violations at run time. If your code doesn't make sure, iterators are destroyed before their associated container or you are in doubt, then enable the build flag BTREE_ITERATOR_REGISTRATION.

_DEBUG:

If the code is compiled to generate a debug binary, then this flag ought to enabled. It switches additional code for higher level debug functionality as well as performance counter code on. Furthermore, additional safe guard assertions are enabled to sanity check input parameters on lower level methods. In short: It enables code not required by a release build.

LINUX:

This build flag needs to be enabled if the code is compiled under linux, to enabled the respective linux include path and files as well as different API calls to the system.


2.4. What has to be included

Depending on what data layer will be in use, at least one of the follwoing files needs to included:
btreeioram.h	- contains everything required for the RAM data layer to work
btreeiofile.h	- contains everything required for the file data layer to work

The above files also include their respective data layer property classes.

If an STL container interchangeable class is required for the project, then see the table below:

include file		btree framework container		STL container

btreemultimap.h		CBTreeMultiMap					multimap
btreemap.h			CBTreeMap						map
btreemultiset.h		CBTreeMultiSet					multiset
btreeset.h			CBTreeSet						set

Depending on what other container type is required, at least one of the following files needs to be include:
btreekeysort.h	- includes ordered associative container: CBTreeKeySort
btreearray.h	- includes array type: CBTreeArray


3. Feedback

Contact:
btreeframework@gmail.com


3.1. What increases the chances that I will respond to your email


3.1.1. A proper subject, which tells the context of what I am going to find in your email's body.

The subject ought to tell me briefly what the email is about. For instance:
"BUG: CBTree_classname::method_name - undefined behaviour in corner case"

The keyword at the beginning as well as the follwing text allow me to quickly assess what I am dealing with.
Also, the keyword allows to have filter rules in place. The following keywords are currently being filtered for:

BUG: anything code related that is not behaving as documented
CODE: anything code related that ought to be implemented a different way
TEST: anything related to existing or missing regression tests or code coverage
DOC: anything document related, such as: Is content missing? Is something not explained well enough?
COMMENT: anything code related in terms of commenting, such as: an existing comment is misleading or some particular code needs commenting
QUESTION: any questing in terms of this project, which has not been answered anywhere else
ENHANCEMENT: anything related to this project that is an enhancement for everyone, such as: release procedures, file formats etc.
OTHER: anything related to this project that has not been covered by any of the above keywords

If none of the keywords has been used, then your email is not lost, but won't be addressed for a very long time. Potentially for a period of time long enough, that the content becomes irrelevant. 

Neither the subject nor the email's body need to tell me that you are talking about the b-tree framework. If you send an email to the address above, then I am assuming that already.


3.1.2. An email body that gets to the point of what you want to say.

The rule of thumb to that end is: The more afford you have made on your side, the more I am inclined to believe you are actually interested in what you are doing. I mean: if you don't care, why should I? By afford, I don't mean that you have to write a tone of text. What I am asking for is a clear train of though, since it won't be you who has to understand your email, that will be me. Explain why you think something needs to be changed, enhanced, fixed, tested or is broken.
In terms of bugs, make sure it isn't a misinterpretation of the documentation or that you are looking at the wrong API function, because between different container types they can look very similar. Double check your finds and document them very well.
Also, criticism is good, but everyone can criticise. At the very least, suggest how to do things better.


3.2. What decreases the chances that I will respond to your email


3.2.1. An empty or useless subject.

An empty subject or useless a subject like this: "OTHER: problem", just show me that not much afford went into writing this email. How I feel about that has been explained above. A useless subject also includes misleading ones. If I see the keyword "BUG" in the subject, then I am expecting to see email content that is talking about code that is not behaving as documented.


3.2.2. Email content that is confusing or off topic.

If I find your email confusing, misleading or off topic, then chances are that you are trying to talk about two or more subjects at once. If you want to talk about two different subjects, write two emails. It also enables me to answer more to the point for each of your emails.
Also, in terms of off topic: Insults won't get you anywhere.


3.2.3. Content that has been covered by the documentation or is too obvious to warrant a reply.

If something has been covered by the documentation, then you will probably get a very short answer where to find it or none at all, depending on my work load. If I see a question like: "Hey, what's a b-tree?", you can be rest assured that I will remain silent.


3.2.4. Improper english.

First of all, I am not an english native speaker and at this point you probably have already spotted a number of grammar and spelling mistakes. Those are not what I mean by "Improper english". What I mean is content like this: "Yo, c my hax yo!"
It doesn't inspire any confidence and will just make me move on to the next email.
Also, what I don't like is when someone writes the letter "u" instead of the word "you" or similar shortcuts. This allows for room to doubt your motivation and once I have that room it is likely that I will divert my interest to something else, like the next email. Just don't allow me to have that room...
Shortcuts, which are within context, such as: "Ctrl+C", are fine though.
