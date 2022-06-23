Clear-Host

function build()
{
  Push-Location build
  cmake .. -G"Ninja" -DCMAKE_BUILD_TYPE=Debug
  cmake --build . 
  Move-Item -Force compile_commands.json ..
  ./game
  Pop-Location
}
if (Test-Path -Path "build")
{
  build
} else
{
  New-Item -Type Directory -Name "build"
  build
}

