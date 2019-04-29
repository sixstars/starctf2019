import sys
sys.path.append("/usr/local/lib/python2.7/dist-packages/")
from z3 import *
from sage.all import *
import random

pool = set()

def get_rand():
    while True:
        a = random.randint(0,0xffffffff)
        if a not in pool:
            pool.add(a)
            return a
    assert(0)

all_ans=[] # save origin number
all_max=[] # save add result
all_sum=[] # save multi coefficients
for _ in range(6):
    a = [get_rand() for _ in range(9)]
    b = []
    b.append((a[0] + a[2] + a[4] + a[6] + a[8])&0xffffffff)
    b.append((a[1] + a[3] + a[4] + a[5] + a[7])&0xffffffff)
    all_ans.append(a)
    all_max.append(b)

flag ="*CTF{7h1S_Cu63_is_m4g1c}"
for idx in range(6):
    va=BitVec('a',32)
    vb=BitVec('b',32)
    vc=BitVec('c',32)
    vd=BitVec('d',32)
    ve=BitVec('e',32)
    vf=BitVec('f',32)
    vg=BitVec('g',32)
    vh=BitVec('h',32)
    vi=BitVec('i',32)

    s=Solver()
    r = all_ans[idx]
    s.add((va*r[0] + vb*r[1] + vc*r[2] + vd*r[3] + \
            ve*r[4] + vf*r[5] + vg*r[6] + vh*r[7] +\
            vi*r[8])&0xffffffff == int(flag[idx*4:idx*4+4][::-1].encode("hex"),16))
    s.add((va+vb+vc+vd+ve+vf+vg+vh+vi)&0xffffffff == 0xffffffff)
    s.add(va!=0,vb!=0,vc!=0,vd!=0,ve!=0,vf!=0,vg!=0,vh!=0,vi!=0)
    if s.check() == sat:
        tmp = s.model()
        all_sum.append([
            tmp[va].as_long(),
            tmp[vb].as_long(),
            tmp[vc].as_long(),
            tmp[vd].as_long(),
            tmp[ve].as_long(),
            tmp[vf].as_long(),
            tmp[vg].as_long(),
            tmp[vh].as_long(),
            tmp[vi].as_long()])
    else:
        raise "error"

f = open("./matrix.h","w+")
def wl(code):
    f.write(code+"\n")
def wc(code):
    f.write(code)

fmt = """uint32_t %s[EDGE][EDGE]={
    {0x%08x,0x%08x,0x%08x},
    {0x%08x,0x%08x,0x%08x},
    {0x%08x,0x%08x,0x%08x}
};
"""
fmt_m = """uint32_t surfaces_m[6][2]={
    {0x%08x,0x%08x},
    {0x%08x,0x%08x},
    {0x%08x,0x%08x},
    {0x%08x,0x%08x},
    {0x%08x,0x%08x},
    {0x%08x,0x%08x}
};  
"""
fmt_s = """uint32_t %s_s[EDGE][EDGE]={
    {0x%08x,0x%08x,0x%08x},
    {0x%08x,0x%08x,0x%08x},
    {0x%08x,0x%08x,0x%08x}
};
"""
faces = ["Front","Behind","Left","Right",
        "Up","Down"]

wl("""
#ifndef MATRIX_H
#define MATRIX_H

#ifdef DEBUG
""")
for i in range(6):
    tmp = tuple([faces[i]]+all_ans[i])
    wl(fmt%tmp)
wl("""
#else

#endif
""")

for i in range(6):
    tmp = tuple([faces[i]]+all_sum[i])
    wl(fmt_s%tmp)
l=[]
for i in all_max:
    l+=i
l=tuple(l)
wl(fmt_m%l)
wl("")

wl("")
wl("""
typedef uint32_t (*uPtr)[EDGE];

uPtr surfaces[]={
    Front,Behind,Left,Right,Up,Down
};
uPtr surfaces_s[]={
    Front_s,Behind_s,Left_s,Right_s,Up_s,Down_s
};
""")
wl("#endif")
f.close()
