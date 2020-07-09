API=../../api
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
AR=arm-none-eabi-ar
CFLAGS=-DNDEBUG -ggdb3 -I$(API) -Os -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -fno-common -fdata-sections -ffunction-sections -fno-exceptions -fno-strict-aliasing -fno-common -fno-threadsafe-statics
LDFLAGS=-Wl,-L$(API) -Wl,--gc-sections -Wl,--entry=entrypoint --specs=nano.specs -nostartfiles -Wl,-Ur -lc -lstdc++ -lm -lapi -lnosys

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

app.elf: src/main.o src/lib/libgiac.a src/lib/libmpfi.a src/lib/libmpfr.a src/lib/libgmp.a
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

src/lib/libgmp.a src/lib/libmpfi.a src/lib/libmpfr.a src/include/gmp.h src/include/mpfi.h src/include/mpfr.h:
	./buildDeps

src/lib/libgiac.a: src/lib/libgmp.a src/lib/libmpfi.a src/lib/libmpfr.a src/include/gmp.h src/include/mpfi.h src/include/mpfr.h
	./buildGiac

clean:
	rm -f *.elf src/*.o src/include/*.h src/lib/*.a
