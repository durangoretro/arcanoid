SOURCE_DIR=.
BUILD_DIR=bin

all: arkanoid.bin


$(BUILD_DIR)/crt0.o: crt0.s $(BUILD_DIR)
	ca65 -t none crt0.s -o $(BUILD_DIR)/crt0.o
	
$(BUILD_DIR)/dlib.o: dlib.s $(BUILD_DIR)
	ca65 -t none dlib.s -o $(BUILD_DIR)/dlib.o

$(BUILD_DIR)/sbc.lib: $(BUILD_DIR)/crt0.o $(BUILD_DIR)
	cp /usr/share/cc65/lib/supervision.lib $(BUILD_DIR)/sbc.lib && ar65 a $(BUILD_DIR)/sbc.lib $(BUILD_DIR)/crt0.o


$(BUILD_DIR)/main.s: $(SOURCE_DIR)/main.c bin $(BUILD_DIR)
	cc65 $(SOURCE_DIR)/main.c -t none --cpu 6502 -o $(BUILD_DIR)/main.s

$(BUILD_DIR)/main.o: $(BUILD_DIR)/main.s $(BUILD_DIR)
	ca65 -t none $(BUILD_DIR)/main.s -o $(BUILD_DIR)/main.o


$(BUILD_DIR)/:
	mkdir -p $(BUILD_DIR)

	
arkanoid.bin: $(BUILD_DIR)/ $(BUILD_DIR)/main.o $(BUILD_DIR)/dlib.o $(BUILD_DIR)/sbc.lib
	ld65 -C durango16k.cfg $(BUILD_DIR)/main.o $(BUILD_DIR)/dlib.o $(BUILD_DIR)/sbc.lib -o arkanoid.bin	

clean:
	rm -Rf $(BUILD_DIR) arkanoid.bin
