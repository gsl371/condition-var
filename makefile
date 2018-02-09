# $@表示目标，$^表示所有依赖，$<表示依赖中的第一个
CPPFLAGS = -I CTP_API
CXXFLAGS = -g -Wall -std=c++11 `mysql_config --cflags` 
#LDFLAGS =-L./CTP_API   
CC = g++  
src=$(wildcard *.cpp) 
obj=$(patsubst %.cpp,%.o,$(src))
obj2=$(wildcard ./CTP_API/*.so)
target = tm 
$(target):$(obj)
	$(CC) -o $@ $(obj) $(obj2) `mysql_config --libs`  -Wl,-rpath=. 
%o:%c
	$(CC) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@ 

.PHONY:clean
clean:
	-rm -f *.o
	-rm -f $(target)
	-rm -f *.csv
	-rm -f *.con
	@echo "Well done!"
test:
	@echo $(src)
	@echo $(obj)
