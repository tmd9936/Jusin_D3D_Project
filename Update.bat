//// ��ɾ�			// �ɼ�		// ���� ������ �ִ� ��ġ			// �纻 ������ ������ ��ġ
mkdir			.\Reference\Headers\
xcopy			/y			.\Engine\Bin\Engine.dll		.\Client\Bin\	
xcopy			/y			.\Engine\Bin\Engine.lib		.\Reference\Librarys\	
xcopy			/y/s		.\Engine\Public\*.h			.\Reference\Headers\