SOURCE_DIR=.
BUILD_DIR=bin
CFG=../dclib/cfg/durango16k.cfg
DCLIB=../dclib/bin
DCINC=../dclib/inc
RESCOMP = ../rescomp/target/rescomp.jar

all: arkanoid.bin

$(BUILD_DIR)/font.h: font_hex.png $(BUILD_DIR)
	java -jar ${RESCOMP} -n font -m FONT -i font_hex.png -h 8 -w 5 -o $(BUILD_DIR)/font.h


$(BUILD_DIR)/dlib.o: dlib.s $(BUILD_DIR)
	ca65 -t none dlib.s -o $(BUILD_DIR)/dlib.o


$(BUILD_DIR)/main.s: $(SOURCE_DIR)/main.c $(BUILD_DIR)/font.h $(BUILD_DIR)
	cc65 -I $(DCINC) $(SOURCE_DIR)/main.c -t none --cpu 6502 -o $(BUILD_DIR)/main.s

$(BUILD_DIR)/main.o: $(BUILD_DIR)/main.s $(BUILD_DIR)
	ca65 -t none $(BUILD_DIR)/main.s -o $(BUILD_DIR)/main.o


$(BUILD_DIR)/:
	mkdir -p $(BUILD_DIR)

	
arkanoid.bin: $(BUILD_DIR)/ $(BUILD_DIR)/main.o $(BUILD_DIR)/dlib.o $(DCLIB)/psv.lib $(DCLIB)/system.lib $(DCLIB)/glyph.lib $(DCLIB)/durango.lib
	ld65 -C $(CFG) $(BUILD_DIR)/main.o $(BUILD_DIR)/dlib.o $(DCLIB)/psv.lib $(DCLIB)/system.lib $(DCLIB)/glyph.lib $(DCLIB)/durango.lib -o arkanoid.bin	

clean:
	rm -Rf $(BUILD_DIR) arkanoid.bin
