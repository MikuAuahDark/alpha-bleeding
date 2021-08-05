cl /nologo /Ox /Ot /EHsc /Febin\alpha-bleeding.exe src\alpha-bleeding.cpp src\png.cpp src\lodepng.cpp || exit /b
cl /nologo /Ox /Ot /EHsc /Febin\alpha-remove.exe src\alpha-remove.cpp png.obj lodepng.obj || exit /b
cl /nologo /Ox /Ot /EHsc /Febin\alpha-set.exe src\alpha-set.cpp png.obj lodepng.obj || exit /b
cl /nologo /Ox /Ot /EHsc /Febin\alpha-trim.exe src\alpha-trim.cpp png.obj lodepng.obj || exit /b
del *.obj
