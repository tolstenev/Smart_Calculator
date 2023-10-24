NAME			:= YonnCalc

CC				:= gcc
CPP_FLAGS		:= -std=c++17 -pedantic -lstdc++
MAIN			:= ./src/main.cc
VIEW_HDR		:= ./src/view/main_window.h
VIEW_SRC		:= ./src/view/main_window.cc
CONTROLLER_HDR	:= ./src/controller/main_controller.h
MODEL_HDR		:= ./src/model/main_model.cc
MODEL_SRC		:= ./src/model/main_model.cc
SRCS			:= $(VIEW_HDR)        \
			   		$(VIEW_SRC)       \
			   		$(CONTROLLER_HDR) \
			   		$(MODEL_HDR)      \
			   		$(MODEL_SRC)      \
			   		$(MAIN)
MODEL_TEST_SRC	:= ./tests/main_model_test.cc
MODEL_TEST_EXEC	:= main_model_test.out
GTEST_FLAGS		:= -lgtest
GCOV_FLAGS		:= -fprofile-arcs -ftest-coverage
REPORT      	:= coverage_report
CURRENT_DIR 	:= $(abspath $(CURDIR))
INSTALL_DIR 	:= $(HOME)/Desktop
OS 				:= $(shell uname -s)

.PHONY: install     \
		open        \
		uninstall   \
		dvi         \
		dvi_rus     \
		dist        \
		tests       \
		lint        \
		check       \
		gcov_report \
		clean

install: clean
	mkdir build
	cd build && cmake ../CMakeLists.txt && make
	mv ./build/$(NAME).app "$(INSTALL_DIR)/$(NAME).app"

open:
	open $(INSTALL_DIR)/$(NAME).app

uninstall:
	rm -rf ./build
	rm -rf $(INSTALL_DIR)/$(NAME).app

dvi:
	open ./reference.html

dvi_rus:
	open ./reference_rus.html

dist: clean
	@clear
	mkdir $(NAME)/
	mkdir $(NAME)/src
	cp -r src/ $(NAME)/src
	mkdir $(NAME)/tests
	cp -r tests/ $(NAME)/tests
	cp ../materials/linters/.clang-format $(NAME)
	cp Makefile CMakeLists.txt reference.html reference_rus.html $(NAME)
	tar -cvzf $(NAME).tar.gz $(NAME)
	rm -rf $(NAME)

tests: $(MODEL_SRC) $(MODEL_HDR) $(MODEL_TEST_SRC)
	@clear
	$(CC) $(CPP_FLAGS) $(MODEL_SRC) $(MODEL_TEST_SRC) -o $(MODEL_TEST_EXEC) $(GTEST_FLAGS)
	./$(MODEL_TEST_EXEC)

.clang-format:
	cp ../materials/linters/.clang-format .

lint: .clang-format
	@clear
	clang-format -i $(SRCS) $(MODEL_TEST_SRC)

check: .clang-format
	@clear
	clang-format -n $(SRCS) $(MODEL_TEST_SRC)

gcov_report:
	@clear
	$(CC) $(CPP_FLAGS) $(GTEST_FLAGS) $(GCOV_FLAGS) $(MODEL_TEST_SRC) $(MODEL_SRC) -o $(REPORT).out
	./$(REPORT).out
	lcov -t "$(REPORT)" -o $(REPORT).info -c -d . --ignore-errors mismatch --no-external --exclude '$(CURRENT_DIR)/src/rcs/*'
	genhtml -o $(REPORT) $(REPORT).info
	rm -rf ./*.gcno ./*.gcda ./$(REPORT).*
	open ./$(REPORT)/index.html

clean:
	@clear
	rm -rf build
	rm -rf $(MODEL_TEST_EXEC)
	rm -rf .clang-format
	rm -rf $(REPORT)
	rm -rf $(REPORT).info
	rm -rf *.gcda *.gcno
	rm -rf $(NAME).tar.gz
	rm -rf $(NAME)/