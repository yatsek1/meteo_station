@echo off
echo Running cppcheck on Core/...

cppcheck --enable=all --inconclusive --std=c++17 --force ^
  -ICore\Inc ^
  Core\Src > cppcheck_report.txt 2>&1

echo Done! Report saved to cppcheck_report.txt
pause
