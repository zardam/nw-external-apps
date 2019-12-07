API=../../api
CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
AR=arm-none-eabi-ar
CFLAGS=-DNDEBUG -ggdb3 -I$(API) -Os -mcpu=cortex-m7 -mthumb -mfpu=fpv5-sp-d16 -mfloat-abi=hard -fno-common -fdata-sections -ffunction-sections -fno-exceptions -fno-strict-aliasing -fno-common -fno-threadsafe-statics
LDFLAGS=-Wl,-L$(API) -Wl,--gc-sections -Wl,--entry=entrypoint --specs=nano.specs -nostartfiles -Wl,-Ur -lc -lstdc++ -lm -lapi -lnosys

%.o: %.cpp
	$(CC) $(CFLAGS) -c $<

app.elf: main.o libgiac.a libmpfi.a libmpfr.a libgmp.a
	$(CC) $^ -o $@ $(CFLAGS) $(LDFLAGS)

clean:
	rm -f *.elf *.o
