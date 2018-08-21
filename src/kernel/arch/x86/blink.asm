section .text
global blink_on
global blink_off
global blink_toggle

;Enable:  OR  with 0x08.
;Disable: AND with 0xF7. (~0x08)
;Toggle:  XOR with 0x08.

blink_common:
	;reset index by reading 0x03DA
	mov bx, 0x03DA
	in al, dx
	
	;write 0x30 to 0x03C0, setting register index to 0x30
	mov dx, 0x03C0
	mov al, 0x30
	out dx, al

	; Get register contents (by reading from 0x03C1)
	inc dx
	in al, dx

	;Set dx back to 0x03C0
	dec dx
	
	ret

bink_on:
	call blink_common
	or al, 0x08 ; enable
	out dx, al
	ret

blink_off:
	call blink_common
	and al, 0xF7 ;disable
	out dx, al
	ret

blink_toggle:
	call blink_common
	xor al, 0x08 ;toggle
	out dx, al
	ret