set var=.\Animator

for /f "tokens=*" %%d in ('dir %var% /S /B') DO (
	xcopy /y/s	%%d\*.fbx	.\out\
)