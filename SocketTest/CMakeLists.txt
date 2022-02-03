# CMakeList.txt: CMake-Projektdatei der obersten Ebene. Führen Sie hier die globale Konfiguration aus,
# und schließen Sie Unterprojekte ein.
#
cmake_minimum_required (VERSION 3.8)

project ("SocketTest")

# Schließen Sie Unterprojekte ein.
add_subdirectory ("SocketTest")
target_include_directories(SocketTest PRIVATE "Networking")
target_include_directories(SocketTest PRIVATE "DateTime")

