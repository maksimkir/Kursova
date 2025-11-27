cmake_minimum_required(VERSION 3.10)
project(Kursova_2kurs_)

set(CMAKE_CXX_STANDARD 20)

add_executable(Kursova_2kurs_ src/main.cpp
        include/IPrintable.h
        src/StoreItem.cpp
        include/StoreItem.h
        src/User.cpp
        include/User.h
        src/Admin.cpp
        include/Admin.h
        src/BasicUser.cpp
        include/BasicUser.h
        src/AuthManager.cpp
        include/AuthManager.h
        src/Configuration.cpp
        include/Configuration.h
        src/Receipt.cpp
        include/Receipt.h
        src/SaleProcessor.cpp
        include/SaleProcessor.h
        src/StoreManager.cpp
        include/StoreManager.h
        include/Product.h
        include/Logger.h
        cmake-build-debug/store_items.csv
)
