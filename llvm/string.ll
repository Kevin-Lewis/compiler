define i32 @string_len(i8* %str){
	Start:
		br label %Count
	Count:
		%size = phi i64 [0, %Start], [%inc, %Count]
		%inc = add i64 %size, 1
		%charstart = ptrtoint i8* %str to i64
		%charinc = add i64 %charstart, %size
		%pos = inttoptr i64 %charinc to i8*
		%char = load i8, i8* %pos
		%end = icmp eq i8 %char, 0
		br i1 %end, label %Break, label %Count
	Break:
		%rvalue = trunc i64 %size to i32
		ret i32 %rvalue

}

define i32 @string_cmp(i8* %str1, i8* %str2){
	Start:
		br label %Compare
	Compare:
		%size = phi i64 [0, %Start], [%inc, %Continue]
		%inc = add i64 %size, 1

		%charstart1 = ptrtoint i8* %str1 to i64
		%charinc1 = add i64 %charstart1, %size
		%pos1 = inttoptr i64 %charinc1 to i8*
		%char1 = load i8, i8* %pos1

		%charstart2 = ptrtoint i8* %str2 to i64
		%charinc2 = add i64 %charstart2, %size
		%pos2 = inttoptr i64 %charinc2 to i8*
		%char2 = load i8, i8* %pos2

		%end1 = icmp eq i8 %char1, 0
		%end2 = icmp eq i8 %char2, 0
		%sum = add i8 %char1, %char2
		%end = icmp eq i8 %sum, 0

		br i1 %end, label %Break, label %Continue

	Continue:
		%compare = icmp eq i8 %char1, %char2
		br i1 %compare, label %Compare, label %Break
	Break:
		%val1 = zext i8 %char1 to i32
		%val2 = zext i8 %char2 to i32
		%rvalue = sub i32 %val1, %val2
		ret i32 %rvalue
}

define i8* @string_chr(i8* %str, i8 %c){
	Start:
		br label %Compare
	Compare:
		%size = phi i64 [0, %Start], [%inc, %Compare]
		%inc = add i64 %size, 1
		%charstart = ptrtoint i8* %str to i64
		%charinc = add i64 %charstart, %size
		%pos = inttoptr i64 %charinc to i8*
		%char = load i8, i8* %pos
		%equal = icmp eq i8 %c, %char

		br i1 %equal, label %Break, label %Compare
	Break:
		ret i8* %pos
}

define i8* @string_cpy(i8* %dest, i8* %src){
	Start:
		br label %Copy
	Copy:
		%size = phi i64 [0, %Start], [%inc, %Copy]
		%inc = add i64 %size, 1

		%charstart = ptrtoint i8* %src to i64
		%charinc = add i64 %charstart, %size
		%pos = inttoptr i64 %charinc to i8*
		%char = load i8, i8* %pos

		%ncharstart = ptrtoint i8* %dest to i64
		%ncharinc = add i64 %ncharstart, %size
		%npos = inttoptr i64 %ncharinc to i8*
		store i8 %char, i8* %npos

		%end = icmp eq i8 %char, 0
		br i1 %end, label %Break, label %Copy
	Break:
		ret i8* %dest

}

define i8* @string_cat(i8* %dest, i8* %src){
	Start:
		br label %Copy
	Copy:
		%size = phi i64 [0, %Start], [%inc, %Copy]
		%inc = add i64 %size, 1

		%charstart = ptrtoint i8* %dest to i64
		%charinc = add i64 %charstart, %size
		%pos = inttoptr i64 %charinc to i8*
		%char = load i8, i8* %pos

		%end = icmp eq i8 %char, 0
		br i1 %end, label %Cat, label %Copy
	Cat:
		%sizecat = phi i64 [%size, %Copy], [%inccat, %Cat]
		%sizesrc = phi i64 [0, %Copy], [%incsrc, %Cat]
		
		%inccat = add i64 %sizecat, 1
		%incsrc = add i64 %sizesrc, 1

		%ocharstart = ptrtoint i8* %src to i64
		%ocharinc = add i64 %ocharstart, %sizesrc
		%opos = inttoptr i64 %ocharinc to i8*
		%ochar = load i8, i8* %opos

		%ncharstart = ptrtoint i8* %dest to i64
		%ncharinc = add i64 %ncharstart, %sizecat
		%npos = inttoptr i64 %ncharinc to i8*
		store i8 %ochar, i8* %npos

		%nend = icmp eq i8 %ochar, 0
		br i1 %nend, label %Break, label %Cat
	Break:
		ret i8* %dest
		
}