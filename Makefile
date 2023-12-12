linux:
	g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g

linux-mingw:
	i686-w64-mingw32-gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g -DWIN32


windows-cygwin:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g

windows-cygwin-release:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -O2


# windows-mingw:
