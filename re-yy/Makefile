#
# Makefile
# hzshang, 2019-02-24 10:13
#
#CFLAGS=-g -D YYDEBUG
all: flag.tab.o lex.yy.o util.o main.o
	@echo "Makefile needs your attention"
	gcc $^ -o $@

flag.tab.c: flag.y
	bison -d -x $^
	xsltproc `bison --print-datadir`/xslt/xml2xhtml.xsl flag.xml > debug.html

flag.tab.o: flag.tab.c
	gcc $(CFLAGS) -c $^ -o $@

lex.yy.c: flag.lex
	lex $^
lex.yy.o: lex.yy.c
	gcc $(CFLAGS) -c $^ -o $@
util.o: util.c
	gcc $(CFLAGS) -c $^ -o $@ -O3
clean:
	rm -f flag.tab.* debug.html flag.xml all lex.yy.* *.o

