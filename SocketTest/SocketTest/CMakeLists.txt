# CMakeList.txt: CMake-Projekt für "SocketTestCmake". Schließen Sie die Quelle ein, und definieren Sie
# projektspezifische Logik hier.
#
cmake_minimum_required (VERSION 3.8)

# Fügen Sie der ausführbaren Datei dieses Projekts eine Quelle hinzu.
add_executable (SocketTest 
"SocketTest.cpp"
"SocketTest.h"
"Networking/TcpListener.cpp"
"Networking/TcpListener.h"
"Networking/HttpResponder.cpp"
"Networking/HttpResponder.h"
"Networking/HttpStatusCode.h")

# TODO: Fügen Sie bei Bedarf Tests hinzu, und installieren Sie Ziele.
