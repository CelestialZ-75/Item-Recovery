#
# 'make'        build executable file 'main'
# 'make clean'  removes all .o and executable files
#

# define the Cpp compiler to use
CXX = g++

# define any compile-time flags
# 添加 Qt 编译标志
CXXFLAGS	:= -std=c++17 -Wall -Wextra -g
# 检测 Qt 路径 (macOS Homebrew)
QT_PREFIX := $(shell brew --prefix qt 2>/dev/null || echo /opt/homebrew/opt/qt)
QT_FLAGS := -I$(QT_PREFIX)/include -I$(QT_PREFIX)/include/QtCore -I$(QT_PREFIX)/include/QtWidgets -I$(QT_PREFIX)/include/QtGui

# define library paths in addition to /usr/lib
LFLAGS = -L$(QT_PREFIX)/lib

# Qt 库链接 (macOS)
QT_LIBS := -F$(QT_PREFIX)/lib -framework QtCore -framework QtWidgets -framework QtGui

# 查找 MOC 工具
MOC := $(shell find /opt/homebrew -name "moc" -type f 2>/dev/null | head -1)
ifeq ($(MOC),)
  # 尝试其他可能的位置
  MOC := $(shell which moc-qt5 2>/dev/null || which moc 2>/dev/null || echo "")
endif

# 如果使用 pkg-config (Linux)
# QT_FLAGS := $(shell pkg-config --cflags Qt5Widgets Qt5Core Qt5Gui 2>/dev/null)
# QT_LIBS := $(shell pkg-config --libs Qt5Widgets Qt5Core Qt5Gui 2>/dev/null)

LDFLAGS += -static -static-libgcc -static-libstdc++

# define output directory
OUTPUT	:= output

# define source directory
SRC		:= src

# define include directory
INCLUDE	:= include

# define lib directory
LIB		:= lib

ifeq ($(OS),Windows_NT)
MAIN	:= main.exe
SOURCEDIRS	:= $(SRC)
INCLUDEDIRS	:= $(INCLUDE)
LIBDIRS		:= $(LIB)
FIXPATH = $(subst /,\,$1)
RM			:= del /q /f
MD	:= mkdir
else
MAIN	:= main
SOURCEDIRS	:= $(shell find $(SRC) -type d)
INCLUDEDIRS	:= $(shell find $(INCLUDE) -type d)
LIBDIRS		:= $(shell find $(LIB) -type d)
FIXPATH = $1
RM = rm -f
MD	:= mkdir -p
endif

# define any directories containing header files other than /usr/include
INCLUDES	:= $(patsubst %,-I%, $(INCLUDEDIRS:%/=%))

# define the C libs
LIBS		:= $(patsubst %,-L%, $(LIBDIRS:%/=%))

# Qt 库链接
# QT_LIBS     := -F/opt/homebrew/opt/qt/lib -framework QtCore -framework QtWidgets -framework QtGui

# MOC 生成的文件（先定义，用于排除）
MOC_HEADERS := $(INCLUDE)/mainwindow.h
MOC_SOURCES := $(patsubst $(INCLUDE)/%.h,$(SRC)/moc_%.cpp,$(MOC_HEADERS))

# define the C source files
# 排除命令行版本的 main.cpp 和 MOC 生成的文件，使用 GUI 版本的 main_gui.cpp
SOURCES		:= $(filter-out $(SRC)/main.cpp $(MOC_SOURCES), $(wildcard $(patsubst %,%/*.cpp, $(SOURCEDIRS))))

# define the C object files
OBJECTS		:= $(SOURCES:.cpp=.o) $(MOC_SOURCES:.cpp=.o)

# define the dependency output files
DEPS		:= $(OBJECTS:.o=.d)

#
# The following part of the makefile is generic; it can be used to
# build any executable just by changing the definitions above and by
# deleting dependencies appended to the file from 'make depend'
#

OUTPUTMAIN	:= $(call FIXPATH,$(OUTPUT)/$(MAIN))

all: $(OUTPUT) $(MAIN)
	@echo Executing 'all' complete!

$(OUTPUT):
	$(MD) $(OUTPUT)

# MOC 规则：从 .h 文件生成 moc_*.cpp 文件
$(SRC)/moc_%.cpp: $(INCLUDE)/%.h
ifneq ($(MOC),)
	$(MOC) $(QT_FLAGS) $(INCLUDES) $< -o $@
else
	@echo "Warning: MOC not found, skipping $@"
	@touch $@
endif

$(MAIN): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QT_FLAGS) -o $(OUTPUTMAIN) $(OBJECTS) $(LFLAGS) $(LIBS) $(QT_LIBS)

# include all .d files
-include $(DEPS)

# this is a suffix replacement rule for building .o's and .d's from .c's
.cpp.o:
	$(CXX) $(CXXFLAGS) $(INCLUDES) $(QT_FLAGS) -c -MMD $<  -o $@

.PHONY: clean
clean:
	$(RM) $(OUTPUTMAIN)
	$(RM) $(call FIXPATH,$(OBJECTS))
	$(RM) $(call FIXPATH,$(DEPS))
	$(RM) $(call FIXPATH,$(MOC_SOURCES))
	@echo Cleanup complete!

run: all
ifeq ($(OS),Windows_NT)
	$(OUTPUTMAIN)
else
	./$(OUTPUTMAIN)
endif
	@echo Executing 'run: all' complete!