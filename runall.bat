@echo off
REM run this file with the arguments 'filename'(int) 'number of tests'(int) 'number of repetitions'(int) 

FOR /L %%T IN (%1, 1, %2) DO (
	FOR /L %%R IN (1, 1, %3) DO (
		echo %1 %%T %%R
		START /WAIT bin\cmd.exe %1 %%T %%R
		IF errorlevel == 1 (
			echo 1 %%T %%R - failed
			START /WAIT bin\cmd.exe %1 %%T %%R
			)
		)
	)
)
