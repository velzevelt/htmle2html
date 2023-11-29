linux:
	gcc ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g


windows-cygwin:
	gcc -I ./src/asprintf ./src/htmle2html.c -o ./bin/htmle2html -Wall -Wextra -g -DWIN32


# windows-mingw:
