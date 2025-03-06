@echo off
echo Compiling M-Pesa Simulation...
g++ -o mpesa_sim.exe main.cpp menu.cpp transactions.cpp userdata.cpp utils.cpp network.cpp -std=c++17
if %errorlevel% neq 0 (
    echo Compilation failed!
    pause
    exit /b %errorlevel%
)
echo Compilation successful!
echo Running M-Pesa Simulation...
mpesa_sim.exe
pause 