param(  [string]$CMakeCommand="Configure",
        [string]$Configuration="Debug", 
        [string]$Platform="x64", 
        [bool]$CleanBuild=$true
)

# check values
if([string]::IsNullOrEmpty($CMakeCommand) )
{
    Write-Error -ForegroundColor Red "CMakeCommand parameter can not be null or empty."
}

if([string]::IsNullOrEmpty($Configuration) -or [string]::IsNullOrEmpty($Platform) )
{
    Write-Error -ForegroundColor Red "Configuration and Platform parameters can not be null or empty."
}

switch ($Configuration) 
{
    "Release" { break }
    "Debug" { break }
    "MinSizeRel"{ break }
    "RelWithDebInfo" { break }
    Default { Write-Error -ForegroundColor Red "Unsupported build configuration. It should be either Release or Debug or MinSizeRel or RelWithDebInfo"}
}

switch ($Platform) 
{
    "x64" { break }
    "x86" { break }
    "Win32" { break }
    Default {Write-Error -ForegroundColor Red "Platform name can only be x64, x86 or Win32"}
}

$BuildDirName="out/build/win_" + $Platform
$InstallDirName="out/install/win_" + $Platform

# Specify current dir
$currentPath = $MyInvocation.MyCommand.Path
$currentDir = Split-Path -parent $currentPath
Push-Location $currentDir
Write-Host "CurrentDir $currentDir" -ForegroundColor Green

# Set up global build directory
$BuildDir = Join-Path -Path $currentDir -ChildPath $BuildDirName
Write-Host "Build directory[Full Path]: $BuildDir"
# -AdditionalChildPath does not exist for this powershell version
$InstallDir = Join-Path -Path $currentDir -ChildPath $InstallDirName | Join-Path -ChildPath $Configuration
Write-Host "Install directory[Full Path]: $InstallDir"

if(Test-Path $BuildDir)
{
    if($CleanBuild -and $CMakeCommand -eq "Configure")
    {
        Remove-Item $BuildDir -Recurse -Force
    }
}
else
{
    New-Item -Path $currentDir -Name $BuildDirName -ItemType "directory"
}

Write-Host "================================" -ForegroundColor Yellow
Write-Host "       System Info              " -ForegroundColor Yellow
Write-Host "================================" -ForegroundColor Yellow

$IsWin = $false
$IsUnix = $false
$operatingSystem = [System.Environment]::OSVersion.Platform
switch ($operatingSystem)
{
    $operatingSystem::Win32NT { $IsWin = $true }
    $operatingSystem::Win32S {$IsWin = $true }
    $operatingSystem::Win32Windows { $IsWin = $true }
    $operatingSystem::WinCE { $IsWin = $true }
    $operatingSystem::Unix { $IsUnix = $true }
    Default {Write-Error "Can not specify the operating system" -ForegroundColor Red }
}

$systemInfo = [System.Environment]::OSVersion 
Write-Host "System Info: $systemInfo"

Write-Host "================================" -ForegroundColor Yellow
Write-Host "       $CMakeCommand            " -ForegroundColor Yellow
Write-Host "================================" -ForegroundColor Yellow
if ($IsWin) 
{
    switch ($CMakeCommand) {
        "Configure" 
        {
            .\scripts\win\cmake_configure.bat "Visual Studio 16 2019" $Platform $Configuration $currentDir $BuildDir $InstallDir
            break
        }
        "Build" 
        {
            .\scripts\win\cmake_build.bat "Visual Studio 16 2019" $Platform $Configuration $currentDir $BuildDir $InstallDir
            break
        }
        "Install" 
        {
            .\scripts\win\cmake_install.bat "Visual Studio 16 2019" $Platform $Configuration $currentDir $BuildDir $InstallDir
            break
        }
        "Run"
        {
            $ExecutionDir = Join-Path -Path $InstallDir -ChildPath "bin" | Join-Path -ChildPath "opengl_notes_ui.exe"
            &$ExecutionDir
        }
        Default {Write-Error -ForegroundColor Red "Unsupported cmake command"}
    }
    
}
else
{
    Write-Warning "This is a Unix system so use unix_build.sh bash script to run the build."
}

Pop-Location
