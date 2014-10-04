################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/G308_Geometry.cpp \
../src/G308_ImageLoader.cpp \
../src/Mass.cpp \
../src/Planet.cpp \
../src/Star.cpp \
../src/main.cpp \
../src/textfile.cpp 

OBJS += \
./src/G308_Geometry.o \
./src/G308_ImageLoader.o \
./src/Mass.o \
./src/Planet.o \
./src/Star.o \
./src/main.o \
./src/textfile.o 

CPP_DEPS += \
./src/G308_Geometry.d \
./src/G308_ImageLoader.d \
./src/Mass.d \
./src/Planet.d \
./src/Star.d \
./src/main.d \
./src/textfile.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


