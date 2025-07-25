# Makefile para simulación de geodésicas con visualización 3D interactiva

# Configuración
CC = gcc
CFLAGS = -Wall -Wextra -O2
SRC = geodesic.c geodesic_f.c
TARGET = geodesic.x
LDFLAGS = -lm
PYTHON = python3
PLOT_SCRIPT = plot_gradient.py
DATA_FILE = geodesic.txt
HTML_OUTPUT = geodesic_plot.html

.PHONY: all clean run plot help

help:
	@echo "Available options:"
	@echo "  make all    : Compile the program"
	@echo "  make run    : Run the simulation and generate data file"
	@echo "  make plot   : Generate interactive HTML plot"
	@echo "  make clean  : Remove generated files"

all: run plot clean

$(TARGET): $(SRC)
	@echo "Compiling program..."
	@$(CC) $(CFLAGS) $(SRC) -o $@ $(LDFLAGS)

run: $(TARGET)
	@echo "Running simulation..."
	@./$(TARGET)
	@echo "Data saved to $(DATA_FILE)."

plot: $(DATA_FILE)
	@echo "Generating interactive plot..."
	@$(PYTHON) $(PLOT_SCRIPT)
	@echo "Plot saved as $(HTML_OUTPUT)."

clean:
	@echo "Cleaning generated files..."
	@rm -f $(TARGET) $(OBJ) $(DATA_FILE) $(HTML_OUTPUT)
