//// 명령어			// 옵션		// 원본 파일이 있는 위치			// 사본 파일을 저장할 위치
mkdir			.\Reference\Headers\
xcopy			/y			.\Engine\Bin\Engine.dll		.\Client\Bin\
xcopy			/y			.\Engine\Bin\Engine.dll		.\Tool\Bin\		
xcopy			/y			.\Engine\Bin\Engine.lib		.\Reference\Librarys\	
xcopy			/y/s		.\Engine\Public\*.h			.\Reference\Headers\
xcopy			/y/s		.\Engine\Public\*.hpp			.\Reference\Headers\
xcopy			/y/s		.\Engine\Public\*.inl			.\Reference\Headers\