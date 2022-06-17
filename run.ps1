Clear-Host

Push-Location build
cmake .. -G"Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=True
cmake --build .
Move-Item compile_commands.json ..
./game
Pop-Location
