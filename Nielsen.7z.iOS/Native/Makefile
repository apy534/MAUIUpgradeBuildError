XBUILD=/Applications/Xcode.app/Contents/Developer/usr/bin/xcodebuild
TARGET_DIR_ROOT=../libs
PROJECT_ROOT=.
PROJECT=$(PROJECT_ROOT)/7za.xcodeproj
TARGET=7za

all: cleanUniversalLibrary CreateUniversalLibrary

lib$(TARGET)-i386.a:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphonesimulator -configuration Release
	-mv $(PROJECT_ROOT)/build/Release-iphonesimulator/lib$(TARGET).a $@

lib$(TARGET)-arm64.a:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphoneos -arch arm64 -configuration Release
	-mv $(PROJECT_ROOT)/build/Release-iphoneos/lib$(TARGET).a $@

CreateUniversalLibrary: lib$(TARGET)-i386.a lib$(TARGET)-arm64.a
	-mkdir -p $(TARGET_DIR_ROOT)
	xcrun -sdk iphoneos lipo -create -output $(TARGET_DIR_ROOT)/lib$(TARGET).a $^
	-rm -rf lib$(TARGET)-arm64.a lib$(TARGET)-i386.a

CreateIphoneSimulatorLibrary: lib$(TARGET)-i386.a
	-mkdir -p $(TARGET_DIR_ROOT)
	-mv lib$(TARGET)-i386.a $(TARGET_DIR_ROOT)/lib$(TARGET).a
	-rm -rf lib$(TARGET)-i386.a

CreateIphoneLibrary: lib$(TARGET)-arm64.a
	-mkdir -p $(TARGET_DIR_ROOT)
	-mv lib$(TARGET)-arm64.a $(TARGET_DIR_ROOT)/lib$(TARGET).a
	-rm -rf lib$(TARGET)-arm64.a

cleanXCodeBuild:
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphonesimulator clean
	$(XBUILD) -project $(PROJECT) -target $(TARGET) -sdk iphoneos clean

cleanUniversalLibrary:
	-rm -f $(TARGET_DIR_ROOT)/*.a

clean: cleanUniversalLibrary cleanXCodeBuild
