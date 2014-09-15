@echo off
FOR /L %%T IN (1, 1, 6142) DO (
	FOR /L %%R IN (1, 1, 100) DO (
		:run
		echo 1 %%T %%R
		START /WAIT bin\cmd.exe 1 %%T %%R
		IF errorlevel == 1 (
			echo 1 %%T %%R - failed
			GOTO :run
		)
	)
)
