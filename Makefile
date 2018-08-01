# programm
SRC_DIR := ./src
SRC_FILES := $(shell find $(SRC_DIR) -type f -name *.cpp ! -path ./src/open_mpi/main.cpp)

INCLUDE_DIR := ./include
INCLUDE_FILES := $(shell find $(INCLUDE_DIR) -type f -name *.h)
OBJS := $(SRC_FILES) $(INCLUDE_FILES)

# open_mpi
OPEN_MPI_SRC_DIR := ./src/processing
OPEN_MPI_INCLUDE_DIR := ./include/processing
OPEN_MPI_SRC_FILES := ./src/open_mpi/main.cpp
OPEN_MPI_SRC_FILES +=  $(shell find $(OPEN_MPI_SRC_DIR) -type f -name *.cpp ! -name main.cpp)

OPEN_MPI_INCLUDE_FILES := $(shell find $(OPEN_MPI_INCLUDE_DIR) -type f -name *.h)
OPEN_MPI_INCLUDE_FILES += $(shell find ./include/cfg -type f -name *.h)

OPEN_MPI_SRC_FILES += $(shell find ./src/cfg -type f -name *.cpp)

OPEN_MPI_OBJS := $(OPEN_MPI_SRC_FILES) $(OPEN_MPI_INCLUDE_FILES)


T_SRC_FILES := $(shell find $(SRC_DIR) -type f -name *.cpp ! -path ./src/open_mpi/main.cpp ! -path ./src/main.cpp)


# test
TEST_SRC_DIR := ./test
TEST_SRC_FILES := $(shell find $(TEST_SRC_DIR) -type f -name *.cpp ! -path ./src/main.cpp ! -path ./src/open_mpi/main.cpp)

TEST_INCLUDE_DIR := ./test
TEST_INCLUDE_FILES := $(shell find $(TEST_INCLUDE_DIR) -type f -name *.h)

TEST_OBJS := $(T_SRC_FILES) $(INCLUDE_FILES) $(TEST_SRC_FILES) $(TEST_INCLUDE_FILES)

INC := -I ./include
TEST_INC := -I ./test/include

CC = g++ -std=c++14
COMPILER_FLAGS = -Wall -g -o3 

LINKER_FLAGS = -lGL -lGLU -lglut -lGLEW -lglfw3 -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor -lboost_system -fopenmp 


MPICC = mpic++ -std=c++14

OBJ_NAME = main
TEST_OBJ_NAME = strategies_test
OPEN_MPI_OBJ_NAME = open_mpi_main

all: $(OBJS)
	$(CC) $(INC) $(OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OBJ_NAME)

test: $(TEST_OBJS)
	$(CC) $(INC) $(TEST_INC) $(TEST_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(TEST_OBJ_NAME)

open_mpi: $(OPEN_MPI_OBJS)
	$(MPICC) $(INC) $(OPEN_MPI_OBJS) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(OPEN_MPI_OBJ_NAME)