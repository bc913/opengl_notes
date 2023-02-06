@echo.
@echo Run Cmake Install
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


@echo Install
@echo.
cmake --install %BUILDDIR% --config %CONFIGURATION% --verbose