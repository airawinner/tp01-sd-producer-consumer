# ==============================
# CONFIG
# ==============================

CXX = g++
CXXFLAGS = -std=c++20 -O2

TARGET = programa
SRC = pros_cons_pipe.cpp

# ==============================
# BUILD
# ==============================

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

# ==============================
# RUN (testes rápidos)
# ==============================

run: all
	@echo " Teste com 5 números"
	./$(TARGET) 5

	@echo "\n Teste com 10 números"
	./$(TARGET) 10

	@echo "\n Teste com 20 números"
	./$(TARGET) 20

# ==============================
# CLEAN
# ==============================

clean:
	rm -f $(TARGET)