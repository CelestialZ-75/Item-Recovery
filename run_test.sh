#!/bin/bash

# 测试脚本
echo "========== 开始测试 =========="
echo ""

# 清理之前的输出
rm -f test_output.txt

# 运行程序并捕获输出
echo "运行程序..."
./output/main < test_input.txt > test_output.txt 2>&1

echo "测试完成！输出已保存到 test_output.txt"
echo ""
echo "========== 测试输出预览 =========="
cat test_output.txt

