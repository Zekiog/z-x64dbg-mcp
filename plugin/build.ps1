# Z-x64dbg-MCP Plugin Build Script
# PowerShell

param(
    [string]$x64dbgDir = "C:\x64dbg",
    [string]$buildType = "Release"
)

Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Building Z-x64dbg-MCP Plugin" -ForegroundColor Cyan
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""

# Check for Visual Studio
$vsPath = & "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath

if (-not $vsPath) {
    Write-Host "Visual Studio with C++ tools not found!" -ForegroundColor Red
    exit 1
}

Write-Host "Visual Studio found: $vsPath" -ForegroundColor Green

# Import VC environment
Import-Module "${vsPath}\Common7\Tools\Microsoft.VisualStudio.DevShell.dll"
Enter-VsDevShell -VsInstallPath $vsPath -SkipAutomaticLocation -DevCmdArguments '-arch=x64'

# Create build directory
$buildDir = Join-Path $PSScriptRoot "build"
if (-not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

Set-Location $buildDir

# Configure
Write-Host ""
Write-Host "Configuring CMake..." -ForegroundColor Yellow
cmake .. -G "Visual Studio 17 2022" -A x64 -Dx64dbgDir="$x64dbgDir"

if ($LASTEXITCODE -ne 0) {
    Write-Host "CMake configuration failed!" -ForegroundColor Red
    exit 1
}

# Build x64
Write-Host ""
Write-Host "Building x64 plugin..." -ForegroundColor Yellow
cmake --build . --config $buildType --target z-x64dbg-mcp-x64

if ($LASTEXITCODE -ne 0) {
    Write-Host "x64 build failed!" -ForegroundColor Red
    exit 1
}

# Build x32
Write-Host ""
Write-Host "Building x32 plugin..." -ForegroundColor Yellow
cmake --build . --config $buildType --target z-x64dbg-mcp-x32

if ($LASTEXITCODE -ne 0) {
    Write-Host "x32 build failed!" -ForegroundColor Red
    exit 1
}

# Copy to x64dbg plugins folder
Write-Host ""
Write-Host "Installing plugins..." -ForegroundColor Yellow

$x64Plugins = Join-Path $x64dbgDir "x64\plugins"
$x32Plugins = Join-Path $x64dbgDir "x32\plugins"

if (-not (Test-Path $x64Plugins)) { New-Item -ItemType Directory -Path $x64Plugins | Out-Null }
if (-not (Test-Path $x32Plugins)) { New-Item -ItemType Directory -Path $x32Plugins | Out-Null }

Copy-Item "z-x64dbg-mcp.dp64" $x64Plugins -Force
Copy-Item "z-x64dbg-mcp.dp32" $x32Plugins -Force

Write-Host "  ✓ Plugin installed to $x64Plugins" -ForegroundColor Green
Write-Host "  ✓ Plugin installed to $x32Plugins" -ForegroundColor Green

Write-Host ""
Write-Host "========================================" -ForegroundColor Cyan
Write-Host "  Build Complete!" -ForegroundColor Green
Write-Host "========================================" -ForegroundColor Cyan
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Yellow
Write-Host "1. Restart x64dbg"
Write-Host "2. Check plugin menu for 'Z-x64dbg-MCP'"
Write-Host "3. Start debugging!"
Write-Host ""
