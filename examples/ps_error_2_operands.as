
; commands";
.define sz =1

;mov
mov r1,#2
mov r1,#sz

mov label,#1

mov array[sz],#sz

mov #2,#sz


;add
add r1,#2
add r1,#sz

add label,#1
add label,#sz

add array[-4],#6
add array[sz],#sz

add #2,#sz
add #sz,#+4



;sub
sub r1,#2
sub r1,#sz

sub label,#1
sub label,#sz

sub array[-4],#6
sub array[sz],#sz

sub #2,#sz
sub #sz,#+4



;lea 
lea r1,#2
lea r1,#sz

lea label,#1
lea label,#sz

lea array[-4],#6
lea array[sz],#sz

lea #2,#sz
lea #sz,#+4


lea array[4],
lea r4, #2
lea r4, label
lea r4, array[4]
lea r4, r4

lea r1,#2
lea r2,label
lea r3,array[5]
lea r4,r2

lea #4,#sz
lea #3,label
lea #sz,arr[5]
lea #3,#2



cmp #e, label



