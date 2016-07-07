#
## @author: mishavetl
#

BUILD_DIR=build
TESTS_EXE=husky_tests

all:
	cd $(BUILD_DIR); make

configure:
	make -p $(BUILD_DIR)
	cd $(BUILD_DIR); cmake -DCMAKE_BUILD_TYPE=Debug ..

test:
	$(BUILD_DIR)/$(TESTS_EXE)
