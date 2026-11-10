
CC = gcc

CFLAGS = -Wall -Wextra -std=c11 -Iagent/include

DEPFLAGS = -MMD -MP

TARGET = agent/bin/linux_edge_agent

TEST_TARGET = agent/bin/test_cpu_monitor

MEMORY_TEST_TARGET = agent/bin/test_memory_monitor

DISK_SPACE_TEST_TARGET = agent/bin/test_disk_space_monitor

DISK_IO_TEST_TARGET = agent/bin/test_disk_io_monitor

OBJ_DIR = agent/obj

SOURCES = agent/src/main.c \
          agent/src/cpu_monitor.c \
          agent/src/memory_monitor.c \
          agent/src/disk_space_monitor.c \
          agent/src/disk_io_monitor.c \
          agent/src/json_output.c

OBJECTS = $(SOURCES:agent/src/%.c=$(OBJ_DIR)/%.o)

DEPFILES = $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

$(OBJ_DIR)/%.o: agent/src/%.c
	$(CC) $(CFLAGS) $(DEPFLAGS) -c $< -o $@

test: $(TEST_TARGET) $(MEMORY_TEST_TARGET) $(DISK_SPACE_TEST_TARGET) $(DISK_IO_TEST_TARGET)
	./$(TEST_TARGET)
	./$(MEMORY_TEST_TARGET)
	./$(DISK_SPACE_TEST_TARGET)
	./$(DISK_IO_TEST_TARGET)
	python3 tests/validate_metrics.py

$(TEST_TARGET): tests/test_cpu_monitor.c agent/src/cpu_monitor.c agent/include/cpu_monitor.h
	$(CC) $(CFLAGS) tests/test_cpu_monitor.c agent/src/cpu_monitor.c -o $@

$(MEMORY_TEST_TARGET): tests/test_memory_monitor.c agent/src/memory_monitor.c agent/include/memory_monitor.h
	$(CC) $(CFLAGS) tests/test_memory_monitor.c agent/src/memory_monitor.c -o $@

$(DISK_SPACE_TEST_TARGET): tests/test_disk_space_monitor.c agent/src/disk_space_monitor.c agent/include/disk_space_monitor.h
	$(CC) $(CFLAGS) tests/test_disk_space_monitor.c agent/src/disk_space_monitor.c -o $@

$(DISK_IO_TEST_TARGET): tests/test_disk_io_monitor.c agent/src/disk_io_monitor.c agent/include/disk_io_monitor.h
	$(CC) $(CFLAGS) tests/test_disk_io_monitor.c agent/src/disk_io_monitor.c -o $@

-include $(DEPFILES)

clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*.d $(TARGET) $(TEST_TARGET) $(MEMORY_TEST_TARGET) $(DISK_SPACE_TEST_TARGET) $(DISK_IO_TEST_TARGET)

.PHONY: all test clean

