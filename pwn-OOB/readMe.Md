## OOB

It's a v8 challenge, related to my recently work.  

The patch added a new function to Array object. You can use it to read and write elements off by one. It's very easy to make a type confusion bug because the you can use oob function to modify an object's map, which records the shape of an object. Then read and write out of boundary, construct arbitrary read and write, hijack pc.


### solve 

#### step1
Alloc array A  

```
A = [1.1,1.2];
```
The memory layout of A:  

```
|         ...              |
|       slot 1             | <== A's element
|       slot 2             |
|     the map of A         | <== the pointer of struct A
|         ...              |
|     array length         |
|         ...              |
|the pointer of A's element|
|         ...              |
``` 

Alloc JSObject B  

```
B = {
	"x1":1.1,
	"x2":1.1,
	"x3":1.1,
	"x4":1.1,
}
```
The memory layout of B:  

```
|     the map of B         | <== the pointer of struct B
|        ...               | 
|       length             | 
|       solt1              | <== B's element
|       slot2              |
|       slot3              |
|       slot4              |
```
#### step2
set A's map to B's map.  
we can't use oob function to get B's map value, but the distance of A's map and B's map is a const value. Therefore, load A's map, add a const value and then store it to A's map.  


#### step3
now v8 will treat A type as B's type, read and write out of boundary.  
You can alloc a ArrayBuffer beside A, and overwrite its backingstore to construct arbitrary read and write.  

#### step4
Alloc a Builtin Array Object beside A, there is PIE information inside Array's code pointer. Leak it to bypass ASLR, and find libc address, stack address.

#### step5
write rop to stack and use nop slide to hijack pc.  

### Files
expolit: [./exp](./exp)  
background service: [./s3rvic3.py](./s3rvic3.py)  
environment script: [./limit.sh](./limit.sh)
 
 
 
Another detailed writeup [https://changochen.github.io/2019-04-29-starctf-2019.html](https://changochen.github.io/2019-04-29-starctf-2019.html)
