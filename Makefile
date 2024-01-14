# 编译器
CXX = g++

# 编译选项
CXXFLAGS = -std=c++17 -Wall -g

# 源文件目录
SRC_DIR = code
INCLUDE_DIR = code/include

# 所有的 CPP 文件
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# 对象文件
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SRC_FILES))

# 最终目标
TARGET = my_webserver

# 默认目标
all: $(TARGET)

# 生成目标文件
%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# 生成最终目标
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $^ -o $@

# 清理生成的文件
clean:
	rm -f $(OBJ_FILES) $(TARGET)
