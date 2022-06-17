Clear-Host

Push-Location build
cmake .. -G"Ninja"
cmake --build .
Move-Item -Force compile_commands.json ..
./game
Pop-Location
