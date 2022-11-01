CC = pgcc
CFLAGS = -O3 -Wall -std=gnu99

UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
  CFLAGS += -lm
endif

.PHONY: clean

with_libpng:
	$(CC)  *.c -o raytracer $(CFLAGS) -lpng -DPNG
with_csfml:: 
	$(CC)  *.c -o raytracer $(CFLAGS) -lcsfml-graphics -lcsfml-window -lcsfml-system
clean:
	rm -f *.o raytracer raytracer_acc
acc:
	$(CC)  -ta=tesla:managed -acc *.c -o raytracer_acc $(CFLAGS) -lpng -DPNG -Minfo=all
acc_test:
	$(CC)  -ta=tesla:nordc -acc vector.c -c $(CFLAGS) -lpng -DPNG -Minfo=all
	ar -cvq vector.a vector.o  
	$(CC)  -ta=tesla:managed -L. -acc array.c color.c geometry.c main.c mesh.c ray.c scene.c camera.c csfml_render.c light.c material.c png_render.c raytracer.c surface.c -o raytracer_acc $(CFLAGS) -lvector -lpng -DPNG -Minfo=all