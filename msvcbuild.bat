cl /nologo /Ox /Ot /EHsc /Febin\alpha-bleeding.exe src\alpha-bleeding.cpp src\png.cpp src\lodepng.cpp
cl /nologo /Ox /Ot /EHsc /Febin\alpha-remove.exe src\alpha-remove.cpp png.obj lodepng.obj
cl /nologo /Ox /Ot /EHsc /Febin\alpha-set.exe src\alpha-set.cpp png.obj lodepng.obj
del *.obj
