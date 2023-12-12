linux:
	g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g

linux-mingw:
	i686-w64-mingw32-c++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g -static-libgcc -static-libstdc++ -Wl,-Bstatic -lstdc++ -lpthread -Wl,-Bdynamic -DWIN32



# linux-mingw:
# 	i686-w64-mingw32-g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g -static


windows-cygwin:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g

windows-cygwin-release:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -O2


# windows-mingw:
