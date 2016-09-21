#
## @author: mishavetl
#

BUILD_DIR?=build
TESTS_EXE?=husky_tests
TESTS_EXE_Lists?=husky_list_tests

WIN32_BUILD_DIR?=win32build
WIN64_BUILD_DIR?=win64build

all: configure
	cd ${BUILD_DIR}; make

configure:
	mkdir -p ${BUILD_DIR}
	cd ${BUILD_DIR}; cmake ${CMAKE_ARGS} -DCMAKE_BUILD_TYPE=Debug ..

test: configure all
	${BUILD_DIR}/${TESTS_EXE}
	${BUILD_DIR}/${TESTS_EXE_Lists}

## Windows 32 bit.

win32_configure:
	mkdir -p ${WIN32_BUILD_DIR}
	cd ${WIN32_BUILD_DIR}; cmake ${CMAKE_ARGS} -DCMAKE_TOOLCHAIN_FILE=toolchains/Windows32bit.cmake -DCMAKE_BUILD_TYPE=Debug ..

win32: win32_configure
	cd ${WIN32_BUILD_DIR}; make

## Windows 64 bit.

win64_configure:
	mkdir -p ${WIN64_BUILD_DIR}
	cd ${WIN64_BUILD_DIR}; cmake ${CMAKE_ARGS} -DCMAKE_TOOLCHAIN_FILE=toolchains/Windows64bit.cmake -DCMAKE_BUILD_TYPE=Debug ..


win64: win64_configure
	cd ${WIN64_BUILD_DIR}; make
