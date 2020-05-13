# 最終ターゲット
mycalc:	y.tab.o lex.yy.o myFunction.o
	g++ -o mycalc y.tab.o lex.yy.o myFunction.o

# 構文解析処理
y.tab.o: calculator.y
	bison -dy calculator.y	# -dy : yacc互換
	g++ -c y.tab.c

# 字句解析処理
lex.yy.o: calculator.l
	flex -l calculator.l
	g++ -c lex.yy.c

myFunction.o: myFunction.c
	g++ -c myFunction.c


# clean:; rm mycalc y.tab.c y.tab.h lex.yy.cc *.o
clean:; rm mycalc y.tab.c y.tab.h lex.yy.c *.o