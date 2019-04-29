## yy

I learned Bison program on my Compiling Principle course. It provides a programable way to DIY your own parser, so I make a flag parser.  

You must input like `*CTF{word_word_word}`, or it will give you `syntax error`.  

And there is a `encrypt` function, it seems very complex and important.  

set a break on `encrypt`, and input a flag like `*CTF{word_word_word}`, you will find that program calls this function three times. And at the start of `encrypt`, input data xor data from an array, at the end of `encrypt`, transformed data is stored in this array again. If you know crypto, you will recognize that it's a part of AES with CBC. The key is static and program only stored the transformed table of key. There is two ways to solve this AES, one is very hard and another is very easy.   

### Easy way  
reverse the transformed table to get origin key. Then use AES library to decrypt origin data.  

### Hard way  
reverse the last part of AES because it's a linear transform. I use z3 to help me, and just run the `encrypt` function backwards.   

by the way, the origin data is not flag, but it's just a box transfrom, so here is my solutions:  
 


```python
from z3 import *
from pwn import *

with open("./table","r") as f:
    table = map(ord,f.read())

def _hash(x,y,z):
    return ( (27 * ((x^y) >> 7)) ^ ((x^y)<<1) ^ x ^ z ) & 0xff

def _hash_z3(x,y,z):
    return ( (27 * LShR(x^y,7)) ^ ((x^y)<<1) ^ x ^ z ) & 0xff


def hash(a,b,c,d):
    tmp = a^b^c^d
    return _hash(a,b,tmp),_hash(b,c,tmp),_hash(c,d,tmp),_hash(d,a,tmp)

def dehash(x,y,z,w):
    a=BitVec('a',8)
    b=BitVec('b',8)
    c=BitVec('c',8)
    d=BitVec('d',8)
    tmp = a^b^c^d
    s=Solver()
    s.add(_hash_z3(a,b,tmp) == x,\
        _hash_z3(b,c,tmp) == y,\
        _hash_z3(c,d,tmp) == z,\
        _hash_z3(d,a,tmp) == w)
    s.check()
    return int(s.model()[a].as_string()),\
        int(s.model()[b].as_string()),\
        int(s.model()[c].as_string()),\
        int(s.model()[d].as_string())

def AES_part2(_key,_flag):
    assert len(_flag) == 0x10
    flag = map(ord,_flag)
    key = map(ord,_key)
    flag = [ p^q for p,q in zip(flag,key[:0x10]) ]

    for i in range(0x10,0xa0,0x10):
        p = [table[x] for x in flag]
        flag[0:4] = hash(p[0],p[5],p[10],p[15])
        flag[4:8] = hash(p[4],p[9],p[14],p[3])
        flag[8:12] = hash(p[8],p[13],p[2],p[7])
        flag[12:16] = hash(p[12],p[1],p[6],p[11])
        flag = [p^q for p,q in zip(flag,key[i:i+0x10])]

    ret = ['']*0x10
    ret[0] = table[flag[0]]
    ret[1] = table[flag[5]]
    ret[2] = table[flag[10]]
    ret[3] = table[flag[15]]
    ret[4] = table[flag[4]]
    ret[5] = table[flag[9]]
    ret[6] = table[flag[14]]
    ret[7] = table[flag[3]]
    ret[8] = table[flag[8]]
    ret[9] = table[flag[13]]
    ret[10] = table[flag[2]]
    ret[11] = table[flag[7]]
    ret[12] = table[flag[12]]
    ret[13] = table[flag[1]]
    ret[14] = table[flag[6]]
    ret[15] = table[flag[11]]
    flag = [p^q for p,q in zip(ret,key[0xa0:0xb0])]
    return "".join(map(chr,flag))

def deAES_part2(_key,_flag):
    key = map(ord,_key)
    flag = map(ord,_flag)
    flag = [p^q for p,q in zip(flag,key[0xa0:0xb0])]
    ret = ['']*0x10
    ret[0] = table.index(flag[0])
    ret[5] = table.index(flag[1])
    ret[10] = table.index(flag[2])
    ret[15] = table.index(flag[3])
    ret[4] = table.index(flag[4])
    ret[9] = table.index(flag[5])
    ret[14] = table.index(flag[6])
    ret[3] = table.index(flag[7])
    ret[8] = table.index(flag[8])
    ret[13] = table.index(flag[9])
    ret[2] = table.index(flag[10])
    ret[7] = table.index(flag[11])
    ret[12] = table.index(flag[12])
    ret[1] = table.index(flag[13])
    ret[6] = table.index(flag[14])
    ret[11] = table.index(flag[15])
    flag = ret

    for i in range(0x90,0,-0x10):
        flag = [p^q for p,q in zip(flag,key[i:i+0x10])]
        p = [''] *0x10
        tmp = dehash(flag[0],flag[1],flag[2],flag[3])
        p[0],p[5],p[10],p[15] = dehash(*flag[0:4])
        p[4],p[9],p[14],p[3] = dehash(*flag[4:8])
        p[8],p[13],p[2],p[7] = dehash(*flag[8:12])
        p[12],p[1],p[6],p[11] = dehash(*flag[12:16])
        flag = [ table.index(x) for x in p]

    flag = [p^q for p,q in zip(flag,key[:0x10])]
    return "".join(map(chr,flag))


with open("./out","r") as f:
    data = f.read()

init_key = data[:0xb0]
result = data[-0x10:]
cmp=[
0,0,0x50cf402af81446ae,0x1212068c04fed331,0xc3d961e826c7fa23,0x3df0c71a70453ca9,
0xac376eab16bcbedf,0x78625df7949c8b14,0x5ad331b21d9816fc,0xa37bca9a86603adc,
0x09d2ffd9b2f1d5b5,0x021956c03dd777d4,0xe377a2e86c429bb6,0x862aa9914032ac99,
0x9b415cc33c47faf3,0x9e5a30d4d00705e8,0x44b6adfba39b528d,0x48fe77229c83723f,
0xffed4e00128486fe,0xca121f84231944ac
]
ct = "\xfe\x86\x84\x12\x00\x4e\xed\xff\xac\x44\x19\x23\x84\x1f\x12\xca"
#pt = "\x82"*16
#print pt.encode("hex")
#pt = "aaaaaaaaaaaasdfg"
#ct = AES_part2(init_key,pt)
#print ct.encode("hex")

box="\x82\x05\x86\x8a\x0b\x11\x96\x1d\x27\xa9\x2b\xb1\xf3\x5e\x37\x38\xc2\x47\x4e\x4f\xd6\x58\xde\xe2\xe5\xe6\x67\x6b\xec\xed\x6f\xf2\x73\xf5\x77\x7f"

pool="abcdefghijklmnopqrstuvwxyz0123456789"
for cnt in range(2,len(cmp),2):
    xor = deAES_part2(init_key,p64(cmp[cnt])+p64(cmp[cnt+1]))
    pt= "".join(map(chr,[ord(x)^ord(y) for x,y in zip(xor,p64(cmp[cnt-2])+p64(cmp[cnt-1]))]))
    s=""
    for p in pt:
        if p in box:
            s+=pool[box.find(p)]
    print s

# print AES_part2(init_key,pt2).encode("hex")


``` 
the output is  

```
yycl
funct10nl
1scl
h4rdl
andcl
n0cl
n33dl
tocl
r3v3rs3l
```
flag is `*CTF{yy_funct10n_1s_h4rd_and_n0_n33d_to_r3v3rs3}`

 