CC			:= gcc
CPP_FLAGS	:= -std=c++17 -pedantic  -lstdc++
MAIN		:= ./src/main.cc
VIEW_HDR	:= ./src/view/main_window.h
VIEW_SRC	:= ./src/view/main_window.cc
CONTROL_HDR	:= ./src/controller/main_controller.h
MODEL_HDR	:= ./src/model/main_model.cc
MODEL_SRC	:= ./src/model/main_model.cc
SRCS		:= $(VIEW_HDR) \
			   $(VIEW_SRC) \
			   $(CONTROL_HDR) \
			   $(MODEL_HDR) \
			   $(MODEL_SRC) \
			   $(MAIN)
TEST_MODEL	:= ./tests/test_model.cc
TEST_EXEC	:= test.out
TEST_FLAGS	:= -lgtest

.PHONY: test

test:
	$(CC) $(CPP_FLAGS) $(MODEL_SRC) $(TEST_MODEL) -o $(TEST_EXEC) $(TEST_FLAGS)
	./$(TEST_EXEC)

copy_linter:
	cp ../materials/linters/.clang-format tests/.clang-format
	cp ../materials/linters/.clang-format src/model/.clang-format
	cp ../materials/linters/.clang-format src/controller/.clang-format
	cp ../materials/linters/.clang-format src/view/.clang-format
	cp ../materials/linters/.clang-format src/rcs/.clang-format

lint: copy_linter
	clang-format -i $(SRCS) $(TEST_MODEL)

check: copy_linter
	clang-format -n $(SRCS) $(TEST_MODEL)

clean:
	rm -rf $(TEST_EXEC)
	rm -rf tests/.clang-format
	rm -rf src/model/.clang-format
	rm -rf src/controller/.clang-format
	rm -rf src/view/.clang-format
	rm -rf src/rcs/.clang-format