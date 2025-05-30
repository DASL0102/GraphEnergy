# Compilador y flags
CXX         = g++ -std=c++17
WARN        = -Wall -Wextra -Wcast-align -Wno-sign-compare -Wno-write-strings -Wno-parentheses -Wfloat-equal -pedantic
FLAGS       = -DDEBUG -g -O0 $(WARN)

# Directorios
SRC_DIR     = src
INC_DIR     = include
BUILD_DIR   = build
DSG_DIR     = DeSiGNAR
DSG_BUILD   = $(DSG_DIR)/build
DSG_INSTALL = $(DSG_DIR)/lib

# Archivos fuente
SRCS        = $(wildcard $(SRC_DIR)/*.cpp)
OBJS        = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
MAIN        = main.cpp

TARGET      = RedElectrica

# Bibliotecas
INCLUDES    = -I$(INC_DIR) -I$(DSG_DIR)/include
LIBS        = -L$(DSG_INSTALL) -lDesignar -lpthread
SFML_LIBS   = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# URLs del repositorio
DSG_GIT_URL = https://github.com/R3mmurd/DeSiGNAR.git

# Reglas principales
all: $(DSG_DIR) $(TARGET)

# Regla para clonar el repositorio si no existe
$(DSG_DIR):
	git clone $(DSG_GIT_URL) $(DSG_DIR)
	cd $(DSG_DIR) && \
	mkdir -p build && \
	cd build && \
	cmake .. && \
	make && \
	make install

# Crear directorio build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compilar objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(FLAGS) $(INCLUDES) -c $< -o $@

# Compilar programa principal
$(TARGET): $(MAIN) $(OBJS) | $(DSG_DIR)
	$(CXX) $(FLAGS) $(INCLUDES) $^ -o $@ $(LIBS) $(SFML_LIBS)

# Ejecutar el programa
run: $(TARGET)
	./$(TARGET)

# Limpiar
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Limpiar todo (incluyendo la biblioteca)
distclean: clean
	rm -rf $(DSG_DIR)

	

.PHONY: all clean distclean run