BUILD_DIR := build

build:
	mkdir -p $(BUILD_DIR) 
	cd build && cmake .. && cmake --build .
.PHONY: build

clean:
	rm -rf $(BUILD_DIR)
