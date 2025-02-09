
cmake_minimum_required(VERSION 3.10)

project(MainProject C)

# Defina os arquivos fonte
set(SOURCES
    main.c
    dtw.c
    sha-256.c
)

# Criação dos arquivos objeto
add_library(dtw_object OBJECT dtw.c)
add_library(sha256_object OBJECT sha-256.c)

# Criação do executável, incluindo os arquivos objeto
add_executable(main.out main.c $<TARGET_OBJECTS:dtw_object> $<TARGET_OBJECTS:sha256_object>)

# Não adicionar -Wall nem outras opções por padrão

