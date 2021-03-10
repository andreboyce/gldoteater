if exist C:\cygwin64\bin\perl.exe goto yesfile
if not exist C:\cygwin64\bin\perl.exe goto nofile
goto end
:yesfile
echo Perl exists. Attempting to shrink Executable.
C:\cygwin64\bin\perl.exe upx.pl
goto end
:nofile
echo Perl does not exist in the path C:\cygwin64\bin\perl.exe.
goto end
:end
