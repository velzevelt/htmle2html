linux:
	g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g

linux-release:
	g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -O3

linux-mingw:
	i686-w64-mingw32-c++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -g -static -DWIN32

linux-mingw-release:
	i686-w64-mingw32-c++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -DWIN32 -O3 -static


windows-cygwin:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g

windows-cygwin-release:
	gcc -I ./src/ ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -O2


windows-mingw-release:
	mingw32-g++ -I ./src/ ./src/htmle2html.cpp -o ./bin/htmle2html -Wall -Wextra -O3 -static -DWIN32
