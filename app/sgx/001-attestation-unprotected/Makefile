LIBSPONGENT_DIR      = ../libspongent
LIBSPONGENT          = $(LIBSPONGENT_DIR)/libspongent

CC                   = gcc
AS                   = gcc
LD                   = gcc

CFLAGS              += -fPIC -fno-stack-protector -fno-builtin -fno-jump-tables \
                       -fno-common -Wno-attributes -g -D_GNU_SOURCE -O0
INCLUDE              = -I../common/ -I$(LIBSPONGENT_DIR) 
LDFLAGS	             = -lspongent -L$(LIBSPONGENT)

SOURCES              = $(shell ls *.c ../common/*.c)
OBJECTS              = $(SOURCES:.c=.o)
OUTPUT               = app

.SILENT:
all: libspongent $(OUTPUT)

libspongent:
	$(MAKE) -C $(LIBSPONGENT)
	
run: clean all
	./$(OUTPUT)

$(OUTPUT): $(BUILDDIRS) $(OBJECTS)
	echo "$(INDENT)[LD]" $(OBJECTS) $(LIBS) -o $(OUTPUT) 
	$(LD) $(OBJECTS) $(LDFLAGS) -o $(OUTPUT) 

%.o : %.c
	echo "$(INDENT)[CC] " $<
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@

%.o : %.S
	echo "$(INDENT)[AS] " $<
	$(AS) $(INCLUDE) -c $< -o $@

clean: $(CLEANDIRS)
	echo "$(INDENT)[RM]" $(OBJECTS) $(OUTPUT)
	rm -f $(OBJECTS) $(OUTPUT)
	$(MAKE) -C $(LIBSPONGENT) clean
