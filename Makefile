SOURCE_DIR=.
BUILD_DIR=bin
DCLIB=../dclib/bin

all: arkanoid.bin




$(BUILD_DIR)/dlib.o: dlib.s $(BUILD_DIR)
	ca65 -t none dlib.s -o $(BUILD_DIR)/dlib.o

$(BUILD_DIR)/dlib.lib: $(BUILD_DIR)/dlib.o $(BUILD_DIR)
	ar65 r $(BUILD_DIR)/dlib.lib $(BUILD_DIR)/dlib.o

$(BUILD_DIR)/main.s: $(SOURCE_DIR)/main.c bin $(BUILD_DIR)
	cc65 $(SOURCE_DIR)/main.c -t none --cpu 6502 -o $(BUILD_DIR)/main.s

$(BUILD_DIR)/main.o: $(BUILD_DIR)/main.s $(BUILD_DIR)
	ca65 -t none $(BUILD_DIR)/main.s -o $(BUILD_DIR)/main.o


$(BUILD_DIR)/:
	mkdir -p $(BUILD_DIR)

	
arkanoid.bin: $(BUILD_DIR)/ $(BUILD_DIR)/main.o $(BUILD_DIR)/dlib.lib $(DCLIB)/durango.lib $(DCLIB)/psv.lib $(DCLIB)/system.lib
	ld65 -C durango16k.cfg $(BUILD_DIR)/main.o $(BUILD_DIR)/dlib.lib $(DCLIB)/durango.lib $(DCLIB)/psv.lib $(DCLIB)/system.lib -o arkanoid.bin	

clean:
	rm -Rf $(BUILD_DIR) arkanoid.bin
