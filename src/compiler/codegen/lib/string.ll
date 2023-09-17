declare void @list_push(%struct.list*, i8*)
declare i8* @list_pop(%struct.list*)
declare i8* @list_get(%struct.list*, i32)
declare i32 @list_size(%struct.list*)