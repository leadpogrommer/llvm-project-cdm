asect 0
main: ext
dc _start, 0
_start:
jsr main
halt

asect  0xF800
VRAM>

asect 0xFC00
BTN>

end.