cmake_minimum_required (VERSION 3.1)

project (btree_framework)

set		(	sources
			./abstract_data_classes/btreeassociativebase.cpp
			./abstract_data_classes/btreeassociative.cpp
			./base_class_stack/btreebasedefaults.cpp
			./base_class_stack/btreebaseif.cpp
			./base_class_stack/btreedefaults.cpp
			./base_class_stack/btreeif.cpp
			./common/btreecommon.cpp
			./io/data_layer/base/btreeio.cpp
			./io/data_layer/target/btreeiofile.cpp
			./io/data_layer/target/btreeioram.cpp
			./io/data_layer/type/btreeioblock.cpp
			./io/data_layer/type/btreeiolinear.cpp
			./io/properties/base/btreeioprop.cpp
			./io/properties/btreefileioprop.cpp
			./io/properties/btreeramioprop.cpp
			./iterators/btreeiter.cpp
			./specific_data_classes/btreearray.cpp
			./specific_data_classes/btreekeysort.cpp
			./specific_data_classes/btreemap.cpp
			./specific_data_classes/btreemultimap.cpp
			./specific_data_classes/btreemultiset.cpp
			./specific_data_classes/btreeset.cpp
			./super_classes/btreeaux.cpp
		)

set		(	headers
			./abstract_data_classes/btreeassociativebase.h
			./abstract_data_classes/btreeassociative.h
			./base_class_stack/btreebasedefaults.h
			./base_class_stack/btreebaseif.h
			./base_class_stack/btreedefaults.h
			./base_class_stack/btreeif.h
			./common/btreecommon.h
			./io/data_layer/base/btreeio.h
			./io/data_layer/target/btreeiofile.h
			./io/data_layer/target/btreeioram.h
			./io/data_layer/type/btreeioblock.h
			./io/data_layer/type/btreeiolinear.h
			./io/properties/base/btreeioprop.h
			./io/properties/btreefileioprop.h
			./io/properties/btreeramioprop.h
			./iterators/btreeiter.h
			./specific_data_classes/btreearray.h
			./specific_data_classes/btreekeysort.h
			./specific_data_classes/btreemap.h
			./specific_data_classes/btreemultimap.h
			./specific_data_classes/btreemultiset.h
			./specific_data_classes/btreeset.h
			./super_classes/btreeaux.h
		)

set		(	srcs
			${sources}
			${headers}
		)

include_directories	(
			${BTREE_FRAMEWORK_INCLUDE_DIRS}
		)

source_group ("sources\\common" REGULAR_EXPRESSION \./common/.*\.cpp)
source_group ("sources\\super classes" REGULAR_EXPRESSION \./super_classes/.*\.cpp)
source_group ("sources\\abstract data classes" REGULAR_EXPRESSION \./abstract_data_classes/.*\.cpp)
source_group ("sources\\base class stack" REGULAR_EXPRESSION \./base_class_stack/.*\.cpp)
source_group ("sources\\specific data classes" REGULAR_EXPRESSION \./specific_data_classes/.*\.cpp)
source_group ("sources\\iterators" REGULAR_EXPRESSION \./iterators/.*\.cpp)
source_group ("sources\\io\\data layer\\base" REGULAR_EXPRESSION \./io/data_layer/base/.*\.cpp)
source_group ("sources\\io\\data layer\\target" REGULAR_EXPRESSION \./io/data_layer/target/.*\.cpp)
source_group ("sources\\io\\data layer\\type" REGULAR_EXPRESSION \./io/data_layer/type/.*\.cpp)
source_group ("sources\\io\\properties\\base" REGULAR_EXPRESSION \./io/properties/base/.*\.cpp)
source_group ("sources\\io\\properties" REGULAR_EXPRESSION \./io/properties/.*\.cpp)


source_group ("headers\\common" REGULAR_EXPRESSION \./common/.*\.h)
source_group ("headers\\super classes" REGULAR_EXPRESSION \./super_classes/.*\.h)
source_group ("headers\\abstract data classes" REGULAR_EXPRESSION \./abstract_data_classes/.*\.h)
source_group ("headers\\base class stack" REGULAR_EXPRESSION \./base_class_stack/.*\.h)
source_group ("headers\\specific data classes" REGULAR_EXPRESSION \./specific_data_classes/.*\.h)
source_group ("headers\\iterators" REGULAR_EXPRESSION \./iterators/.*\.h)
source_group ("headers\\io\\data layer\\base" REGULAR_EXPRESSION \./io/data_layer/base/.*\.h)
source_group ("headers\\io\\data layer\\target" REGULAR_EXPRESSION \./io/data_layer/target/.*\.h)
source_group ("headers\\io\\data layer\\type" REGULAR_EXPRESSION \./io/data_layer/type/.*\.h)
source_group ("headers\\io\\properties\\base" REGULAR_EXPRESSION \./io/properties/base/.*\.h)
source_group ("headers\\io\\properties" REGULAR_EXPRESSION \./io/properties/.*\.h)

if (MSVC)

	# MSVC linker warns about object files not being in the library build
	# this can be safely ignored, since due to the fact how c++ template based source files
	# have to be included and that when a source which was previously included has to be
	# processed, contributes nothing to the final build --> the result is a warning
	# 
	# However, all files have to stay in the file lists, in case the build or included order changes!
	list (APPEND CMAKE_STATIC_LINKER_FLAGS " /IGNORE:4221")
	string (REGEX REPLACE "%3B" " " "CMAKE_STATIC_LINKER_FLAGS" ${CMAKE_STATIC_LINKER_FLAGS})
	string (REGEX REPLACE ";" " " "CMAKE_STATIC_LINKER_FLAGS" ${CMAKE_STATIC_LINKER_FLAGS})

#	get_cmake_property (_varNames VARIABLES)
#	foreach (_varName ${_varNames})
#		message (STATUS "${_varName}=${${_varName}}")
#	endforeach ()
endif ()

add_library (${PROJECT_NAME} ${srcs})

