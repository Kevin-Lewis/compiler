define i32 @string_len(i8* %str){
	Start:
		br label %Count
	Count:
		%size = phi i32 [0, %Start], [%inc, %Count]
		%inc = add i32 %size, 1
		%full = icmp eq i32 %size, 5
		br i1 %full, label %Break, label %Count
	Break:
		ret i32 %size;

}