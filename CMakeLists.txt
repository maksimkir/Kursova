cmake_minimum_required(VERSION 3.10)
project(Kursova_2kurs_)

set(CMAKE_CXX_STANDARD 20)

add_executable(Kursova_2kurs_ main.cpp
        IPrintable.h
        StoreItem.cpp
        StoreItem.h
        User.cpp
        User.h
        Admin.cpp
        Admin.h
        BasicUser.cpp
        BasicUser.h
        AuthManager.cpp
        AuthManager.h
        Configuration.cpp
        Configuration.h
        Receipt.cpp
        Receipt.h
        SaleProcessor.cpp
        SaleProcessor.h
        StoreManager.cpp
        StoreManager.h
        Product.h
        Logger.h
)
