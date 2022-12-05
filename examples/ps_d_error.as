.define sz = 2
		mov #sz , r1
		mov #2 , r2
		
		mov #sz, label
		mov #2 , label

		mov #sz , arr[2]
		mov #2 ,  arr[sz]
                mov #sz, arr[sz]
                mov #2 , arr[+2]


		add #sz , r1
                add #2 , r2

                add #sz, label
                add #2 , label

                add #sz , arr[2]
                add #2 ,  arr[sz]
                add #sz, arr[sz]
                add #2 , arr[+2]


		sub #sz , r1
                sub #2 , r2

                sub #sz, label
                sub #2 , label

                sub #sz , arr[2]
                sub #2 ,  arr[sz]
		sub #sz, arr[sz]
                sub #2 , arr[+2]
	

		
		cmp #sz , r1
                cmp #2 , r2

                cmp #sz, label
                cmp #2 , label

                cmp #sz , arr[2]
                cmp #2 ,  arr[sz]
		cmp #sz, arr[sz]
		cmp #2 , arr[+2]		

		cmp #sz , #2
		cmp #1 , #sz

		cmp r1 , #2
		cmp r2 , #sz

		cmp label, #2
		cmp label, #sz

		cmp arr[2], #2
		cmp arr[sz], #sz
		cmp arr[+2], #sz
		cmp arr[2], #-2

		prn #-1
		prn #sz
		

label: .data 2
arr: .data 4,5,sz
