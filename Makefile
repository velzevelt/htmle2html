linux:
	g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g

linux-release:
	g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -O2

linux-mingw:
	i686-w64-mingw32-c++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g -static -DWIN32

linux-mingw-release:
	i686-w64-mingw32-c++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -static -DWIN32 -O2


windows-cygwin:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g

windows-cygwin-release:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -O2


# windows-mingw:
