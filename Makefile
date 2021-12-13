BUILD_DIR := build

build:
	mkdir -p $(BUILD_DIR) 
	cd build && cmake .. && make -j$(shell nproc)
.PHONY: build

clean:
	rm -rf $(BUILD_DIR)
