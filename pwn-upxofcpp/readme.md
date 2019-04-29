# upxofcpp

## Idea
- Make an easy UAF challenge under Ubuntu 16.04
- Keep it fun

## Vulnerability
- Unpack the original file by `upx -d upxofcpp`
- Do code auditing, you will find a simple UAF bug

## Key points
- PIE is enabled and there is no function prepared for information leakage
- But it is packed by upx! Running the upx'ed program, you will find many `rwx` memory regions, including the heap
    - Refer to <https://github.com/upx/upx/issues/81>
    - This ELF file is packed by upx-3.91 installed via `apt install upx-ucl` on Ubuntu 16.04
    - If you focus on the unpacked one, you will miss the `rwx` memory regions

## Exploit
- Fake the vtable via `malloc_consolidate`, write your shellcode on the heap
- Trigger the UAF vulnerability by `remove` or `show`, then `call rax` will jump on the shellcode
- Details in `solve.py`
- It has nothing to do with libc :P 

## Flag
`*ctf{its_time_to_say_goodbye_to_ubuntu_16_04}`
