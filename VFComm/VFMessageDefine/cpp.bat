
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./VFDefine.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./VFMsgBase.proto 
protoc.exe -I=./ --cpp_out=dllexport_decl=LIBPROTOC_EXPORT:./ ./VFMsgPreGame.proto

pause