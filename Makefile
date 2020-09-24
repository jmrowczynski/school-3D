LIBS=-lGL -lglfw -lGLEW
HEADERS=constants.h  lodepng.h  shaderprogram.h OBJ_Loader.h globals.h model.h
FILES=lodepng.cpp  main_file.cpp  shaderprogram.cpp

main_file: $(FILES) $(HEADERS)
	g++ -o main_file $(FILES)  $(LIBS) -I.
