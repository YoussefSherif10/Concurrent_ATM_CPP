cmake_minimum_required(VERSION 3.24)
project(ATM)

set(CMAKE_CXX_STANDARD 17)

add_executable(ATM main.cpp messaging.h sender.h recevier.h dispatcher.h templateDispatcher.h messages.h atm.h)
