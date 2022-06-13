Clear-Host

Push-Location build
cmake .. -G"Ninja"
cmake --build .
./game
Pop-Location
