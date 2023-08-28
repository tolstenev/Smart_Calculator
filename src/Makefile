CC			:= gcc
CPP_FLAGS	:= -std=c++17 -pedantic  -lstdc++
MODEL_FILE	:= ./src/model/main_model.cc
TEST_MODEL	:= ./tests/test_model.cc
TEST_EXEC	:= test.out
TEST_FLAGS	:= -lgtest

test:
	$(CC) $(CPP_FLAGS) $(MODEL_FILE) $(TEST_MODEL) -o $(TEST_EXEC) $(TEST_FLAGS)
	./$(TEST_EXEC)
