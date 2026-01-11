@echo off
cd 286
call buildall.bat
cd ..

cd 386
call buildall.bat
cd ..

cd minidriv
call buildall.bat
cd ..

cd multimed
call buildall.bat
cd ..

cd samples
call buildall.bat
cd ..

