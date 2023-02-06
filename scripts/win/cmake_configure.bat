@echo.
@echo Configuring CMake
@echo.
@echo off

SET GENERATOR=%1
SET PLATFORM=%2
SET CONFIGURATION=%3
SET SOURCEDIR=%4
SET BUILDDIR=%5
SET INSTALLDIR=%6

@echo Variables:
@echo Generator: %GENERATOR%
@echo Platform: %PLATFORM%
@echo Configuration: %CONFIGURATION%
@echo BuildDir: %BUILDDIR%
@echo SourceDir: %SOURCEDIR%
@echo InstallDir: %INSTALLDIR%
@echo.

@echo Generate
@echo.
cmake -S %SOURCEDIR% -B %BUILDDIR% -G %GENERATOR% -A %PLATFORM% -DCMAKE_BUILD_TYPE=%CONFIGURATION% -DCMAKE_INSTALL_PREFIX=%INSTALLDIR%
@echo.