compile:
	c++ `Magick++-config --cxxflags --cppflags --ldflags --libs` main.cpp


execute:
	./a.out


a: compile execute

