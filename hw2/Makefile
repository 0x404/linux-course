# 编译器指令定义
CC := g++
CCFLAGS :=
CCOBJFLAGS := $(CCFLAGS) -c

# 本次作业目录
# bin为可执行文件目录，本次作业shell程序放在bin目录中
# obj为对象文件目录，src为源代码文件目录
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src

# 目标文件，为`bin/zsh`
TARGET_NAME := zsh
TARGET := $(BIN_PATH)/$(TARGET_NAME)

# 本次作业源文件以及对象文件
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# makefile默认规则
default: makedir all

# 目标文件zsh的生成依赖于obj目录下的指定.o文件
$(TARGET): $(OBJ)
	$(CC) $(CCFLAGS) -o $@ $(OBJ)

# obj目录下的文件生成
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CC) $(CCOBJFLAGS) -o $@ $<


.PHONY: makedir
makedir:
	mkdir -p $(BIN_PATH) $(OBJ_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	rm -f obj/*
	rm -f bin/*