#
#	End edit at 4/7/12
#

find . -name "*.h" -o -name "*.c" -o -name "*.cc" -o -name "*.cpp" > cscope.files

cscope -bkq -i cscope.files

ctags -R *	# C project

#ctags -R --c++-kinds=+p --fields=+iaS --extra=+q .  # C++ project

