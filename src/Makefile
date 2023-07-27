CXX = g++
CFLAGS = -std=c++17 -Wall -Wextra -Werror
BUILD_DIR     	:= ../build
.PHONY: all clean ant gauss winograd

all: clean winograd

clean:
	rm -f gauss.out winograd.out ant.out
	rm -rf $(BUILD_DIR)

ant: clean
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target ANTS
	../build/ANTS

gauss: clean
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target GAUSS
	../build/GAUSS

winograd: clean
	cmake -S . -B $(BUILD_DIR)
	cmake --build $(BUILD_DIR) --target WINOGRAD
	../build/WINOGRAD

style:
	clang-format -n winograd/model/*.cc winograd/model/*.h winograd/view/*.cc winograd/view/*.h lib/matrix.h lib/common_view.h gauss/model/*.cc gauss/model/*.h gauss/view/*.cc gauss/view/*.h Ants/*.cc Ants/*.h

style-fix:
	clang-format -i winograd/model/*.cc winograd/model/*.h winograd/view/*.cc winograd/view/*.h lib/matrix.h lib/common_view.h gauss/model/*.cc gauss/model/*.h gauss/view/*.cc gauss/view/*.h Ants/*.cc Ants/*.h

cppcheck:
	cppcheck --language=c++ --std=c++17 --suppress=missingIncludeSystem --suppress=unusedFunction .

