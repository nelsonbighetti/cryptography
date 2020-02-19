project(cryptography)
file(GLOB SOURCES "*.h" "*.cpp")
add_executable(cryptography ${SOURCES})