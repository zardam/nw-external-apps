SFLAGS += -DSIMULATOR

LDFLAGS += $(EXTAPP_PATH)/libgiac_simulator.a -lpthread -ldl -lpng -lmpfi -lmpfr -lgmp

app_external_src += $(addprefix $(EXTAPP_PATH)/,\
	main.cpp \
)